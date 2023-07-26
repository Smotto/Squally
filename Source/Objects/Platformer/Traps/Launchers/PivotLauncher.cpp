#include "PivotLauncher.h"

#include "cocos/2d/CCActionInstant.h"
#include "cocos/2d/CCActionInterval.h"
#include "cocos/2d/CCSprite.h"
#include "cocos/base/CCDirector.h"
#include "cocos/base/CCValue.h"

#include "Engine/Animations/AnimationPart.h"
#include "Engine/Animations/SmartAnimationNode.h"
#include "Engine/Camera/GameCamera.h"
#include "Engine/Events/ObjectEvents.h"
#include "Engine/Hackables/HackableCode.h"
#include "Engine/Optimization/LazyNode.h"
#include "Engine/Physics/CollisionObject.h"
#include "Engine/Utils/GameUtils.h"
#include "Engine/Utils/MathUtils.h"
#include "Entities/Platformer/PlatformerEntity.h"
#include "Entities/Platformer/Squally/Squally.h"
#include "Objects/Platformer/Projectiles/Projectile.h"
#include "Objects/Platformer/Projectiles/ProjectilePool.h"
#include "Scenes/Platformer/Hackables/HackFlags.h"

#include "Resources/ObjectResources.h"
#include "Resources/UIResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

#define LOCAL_FUNC_ID_LAUNCH_TIMER 1

const std::string PivotLauncher::HackIdentifierLaunchTimer = "pivot-launch-timer";
const std::string PivotLauncher::PivotBone = "pivot_bone";
const std::string PivotLauncher::PropertyLaunchSpeed = "speed";
const std::string PivotLauncher::PropertyPivotTarget = "pivot-target";
const std::string PivotLauncher::PropertyFixed = "fixed";
const std::string PivotLauncher::PropertyEnable3D = "z-enabled";
const float PivotLauncher::DefaultLaunchSpeed = 320.0f;
const float PivotLauncher::LaunchCooldownMin = 2.0f;
const float PivotLauncher::LaunchCooldownMax = 4.0f;

PivotLauncher::PivotLauncher(ValueMap& properties, std::string animationResource, bool skipRegisterHackables, int projectilePoolCapacity) : super(properties)
{
	this->containerNode = Node::create();
	this->launcherAnimations = SmartAnimationNode::create(animationResource);
	this->projectilePool = ProjectilePool::create([=](){ return this->createProjectile(); }, projectilePoolCapacity);
	this->cannon = this->launcherAnimations->getAnimationPart(PivotLauncher::PivotBone);
	this->targetQueryKey = GameUtils::getKeyOrDefault(this->properties, PivotLauncher::PropertyPivotTarget, Value("")).asString();
	this->isFixed = GameUtils::keyExists(this->properties, PivotLauncher::PropertyFixed);
	this->is3DEnabled = GameUtils::getKeyOrDefault(this->properties, PivotLauncher::PropertyEnable3D, Value(false)).asBool();
	this->fixedAngle = GameUtils::getKeyOrDefault(this->properties, PivotLauncher::PropertyFixed, Value(0.0f)).asFloat();
	this->launchSpeed = GameUtils::getKeyOrDefault(this->properties, PivotLauncher::PropertyLaunchSpeed, Value(320.0f)).asFloat();
	this->currentAngle = this->fixedAngle;
	this->skipRegisterHackables = skipRegisterHackables;

	this->setContentSize(CSize(128.0f, 192.0f));

	this->launcherAnimations->playAnimation();
	this->launcherAnimations->setPositionY(-GameUtils::getKeyOrDefault(this->properties, GameObject::MapKeyHeight, Value(0.0f)).asFloat() / 2.0f);
	this->launcherAnimations->setFlippedX(GameUtils::getKeyOrDefault(this->properties, GameObject::MapKeyFlipX, Value(false)).asBool());
	this->launcherAnimations->setFlippedY(GameUtils::getKeyOrDefault(this->properties, GameObject::MapKeyFlipY, Value(false)).asBool());

	this->faceTarget();

	this->containerNode->addChild(this->projectilePool);
	this->containerNode->addChild(this->launcherAnimations);
	this->addChild(this->containerNode);
}

PivotLauncher::~PivotLauncher()
{
}

void PivotLauncher::onEnter()
{
	super::onEnter();

	this->listenForMapEvent(this->getListenEvent(), [=](ValueMap args)
	{
		this->shoot();
	});

	ObjectEvents::WatchForObject<PlatformerEntity>(this, [=](PlatformerEntity* platformerEntity)
	{
		this->target = platformerEntity;
	}, this->targetQueryKey);

	this->scheduleUpdate();
}

void PivotLauncher::initializePositions()
{
	super::initializePositions();
}

void PivotLauncher::update(float dt)
{
	super::update(dt);

	this->faceTarget();

	// if (this->rangeCheck())
	if (this->isAutoLaunch)
	{
		this->updateShootTimer(dt);
	}
}

Vec2 PivotLauncher::getButtonOffset()
{
	const float OffsetDistance = 124.0f;
	const float Angle = float(M_PI) * this->currentAngle / 180.0f;

	return Vec2(std::cos(Angle) * OffsetDistance, std::sin(Angle) * OffsetDistance);
}

void PivotLauncher::registerHackables()
{
	super::registerHackables();

	if (this->skipRegisterHackables)
	{
		return;
	}
	
	HackableCode::CodeInfoMap codeInfoMap =
	{
		{
			LOCAL_FUNC_ID_LAUNCH_TIMER,
			HackableCode::HackableCodeInfo(
				PivotLauncher::HackIdentifierLaunchTimer,
				Strings::Menus_Hacking_Objects_PivotLauncher_UpdateLaunchTimer_UpdateLaunchTimer::create(),
				HackableBase::HackBarColor::Purple,
				UIResources::Menus_Icons_CrossHair,
				LazyNode<HackablePreview>::create([=](){ return this->getTimerPreview(); }),
				{
					{
						HackableCode::Register::zax, Strings::Menus_Hacking_Objects_PivotLauncher_UpdateLaunchTimer_RegisterEax::create(), HackableDataType::Float
					},
					{
						HackableCode::Register::xmm0, Strings::Menus_Hacking_Objects_PivotLauncher_UpdateLaunchTimer_RegisterXmm0::create(), HackableDataType::Float
					},
					{
						HackableCode::Register::xmm1, Strings::Menus_Hacking_Objects_PivotLauncher_UpdateLaunchTimer_RegisterXmm1::create(), HackableDataType::Float
					},
				},
				int(HackFlags::None),
				12.0f,
				0.0f
			)
		},
	};

	auto swingFunc = &PivotLauncher::updateShootTimer;
	std::vector<HackableCode*> hackables = HackableCode::create((void*&)swingFunc, codeInfoMap);

	for (HackableCode* next : hackables)
	{
		this->registerCode(next);
	}
}

HackablePreview* PivotLauncher::getTimerPreview()
{
	return nullptr;
}

Vec2 PivotLauncher::getProjectileSpawnPosition()
{
	return Vec2::ZERO;
}

void PivotLauncher::setAutoLaunch(bool isAutoLaunch)
{
	this->isAutoLaunch = isAutoLaunch;
}

bool PivotLauncher::rangeCheck()
{
	// It may be ideal for this to be configurable. Fixed-angle launchers can get away with this being a high value, but player-tracking launchers would want this to be low.
	static const CSize Padding = CSize(1024.0f, 512.0f);
	static const CRect CameraRect = CRect(Vec2::ZERO, Director::getInstance()->getVisibleSize());
	CRect thisRect = GameUtils::getScreenBounds(this, Padding);

	return CameraRect.intersectsRect(thisRect);
}

void PivotLauncher::shoot()
{
	if (this->target == nullptr)
	{
		return;
	}
	
	float thisDepth = GameUtils::getDepth(this);
	float targetDepth = GameUtils::getDepth(this->target);

	if (!this->is3DEnabled && std::abs(thisDepth - targetDepth) > 24.0f)
	{
		return;
	}
	
	Projectile* projectile = this->projectilePool->getNextProjectile();
	
	projectile->setMovementMode(this->is3DEnabled ? Projectile::MovementMode::Kinematic : Projectile::MovementMode::RotationVelocity);
	projectile->setPosition(projectile->getPosition() + this->getProjectileSpawnPosition());
	projectile->setProjectileRotation(this->currentAngle);

	if (this->is3DEnabled)
	{
		float speed = 256.0f / (this->launchSpeed <= 0.0f ? 320.0f : this->launchSpeed);
		
		projectile->launchTowardsTarget3D(target, target->getEntityCenterPoint(), 0.0f, Vec3(speed, speed, speed), Vec3::ZERO);
	}
	else
	{
		projectile->setLaunchVelocity(Vec3(this->launchSpeed, 0.0f, 0.0f));
	}
}

void PivotLauncher::faceTarget()
{
	if (this->isFixed)
	{
		this->currentAngle = this->fixedAngle;
	}
	else if (this->target != nullptr)
	{
		float thisDepth = GameUtils::getDepth(this);
		float targetDepth = GameUtils::getDepth(this->target);

		if (!this->is3DEnabled && std::abs(thisDepth - targetDepth) > 24.0f)
		{
			return;
		}

		Vec3 thisCoords = GameUtils::getWorldCoords3D(this->launcherAnimations);
		Vec3 targetCoords = GameUtils::getWorldCoords3D(this->target);

		const float angleBetween = -std::atan2(thisCoords.y - targetCoords.y, thisCoords.x - targetCoords.x) + (this->launcherAnimations->getFlippedX() ? float(M_PI) : 0.0f);
		const float angleNormalized = MathUtils::wrappingNormalize(float(M_PI) + angleBetween, 0.0f, 2.0f * float(M_PI));

		this->currentAngle = -angleNormalized * 180.0f / float(M_PI);

		this->repositionHackButtons();
	}

	cannon->setRotation(this->currentAngle);
}

NO_OPTIMIZE void PivotLauncher::updateShootTimer(float dt)
{
	if (this->target == nullptr)
	{
		return;
	}

	if (this->launchTimer <= 0.0f)
	{
		this->launchTimer = RandomHelper::random_real(PivotLauncher::LaunchCooldownMin, PivotLauncher::LaunchCooldownMax);

		this->shoot();
	}
	
	static volatile float* timePtr;
	static volatile float* dtPtr = &dt;

	timePtr = &this->launchTimer;
	dtPtr = &dt;

	ASM(push ZAX);
	ASM(push ZBX);

	ASM_MOV_REG_PTR(ZAX, timePtr);
	ASM_MOV_REG_PTR(ZBX, dtPtr);
	ASM(movss xmm0, [ZAX])
	ASM(movss xmm1, [ZBX])

	HACKABLE_CODE_BEGIN(LOCAL_FUNC_ID_LAUNCH_TIMER);
	ASM(subps xmm0, xmm1);
	ASM(movss [ZAX], xmm0);
	ASM_NOP10();
	HACKABLE_CODE_END();

	ASM(pop ZBX);
	ASM(pop ZAX);

	HACKABLES_STOP_SEARCH();
}
END_NO_OPTIMIZE
