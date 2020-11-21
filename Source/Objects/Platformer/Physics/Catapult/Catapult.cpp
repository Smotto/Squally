#include "Catapult.h"

#include "cocos/2d/CCActionInstant.h"
#include "cocos/2d/CCActionInterval.h"
#include "cocos/2d/CCActionEase.h"
#include "cocos/2d/CCSprite.h"
#include "cocos/base/CCValue.h"

#include "Engine/Animations/AnimationPart.h"
#include "Engine/Animations/SmartAnimationNode.h"
#include "Engine/Events/ObjectEvents.h"
#include "Engine/Input/Input.h"
#include "Engine/Localization/LocalizedString.h"
#include "Engine/Hackables/HackableCode.h"
#include "Engine/Physics/CollisionObject.h"
#include "Engine/Utils/GameUtils.h"
#include "Engine/Utils/MathUtils.h"
#include "Objects/Platformer/Physics/Catapult/CatapultBall.h"
#include "Objects/Platformer/Physics/Catapult/CatapultApplyPowerPreview.h"
#include "Objects/Platformer/Physics/Catapult/CatapultGenericPreview.h"
#include "Scenes/Platformer/Hackables/HackFlags.h"
#include "Scenes/Platformer/Level/Physics/PlatformerCollisionType.h"

#include "Resources/ObjectResources.h"
#include "Resources/UIResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

#define LOCAL_FUNC_ID_APPLY_POWER 1

const std::string Catapult::MapKey = "catapult";
const float Catapult::InteractCooldown = 3.0f;
const float Catapult::LaunchPowerDefault = 0.90f;
const Vec2 Catapult::LaunchVelocityBase = Vec2(1200.0f, 1200.0f);

Catapult* Catapult::create(ValueMap& properties)
{
	Catapult* instance = new Catapult(properties);
	
	instance->autorelease();

	return instance;
}

Catapult::Catapult(ValueMap& properties) : super(properties)
{
	this->catapultAnimations = SmartAnimationNode::create(ObjectResources::Traps_Catapult_Animations);
	this->catapultCollision = CollisionObject::create(CollisionObject::createBox(Size(512.0f, 320.0f)), (CollisionType)PlatformerCollisionType::Physics, CollisionObject::Properties(false, false));
	this->ballAnimationPart = this->catapultAnimations->getAnimationPart("BALL");
	this->launchPower = Catapult::LaunchPowerDefault;
	this->interactionEnabled = false;
	this->currentCooldown = 0.0f;

	float height = GameUtils::getKeyOrDefault(this->properties, GameObject::MapKeyHeight, Value(0.0f)).asFloat();
	this->catapultAnimations->setPositionY(-height / 2.0f);
	this->catapultAnimations->setScale(1.75f);

	this->catapultCollision->addChild(this->catapultAnimations);
	this->addChild(this->catapultCollision);
}

Catapult::~Catapult()
{
}

void Catapult::onEnter()
{
	super::onEnter();

	this->interactionEnabled = false;
	this->currentCooldown = 0.0f;

	this->scheduleUpdate();
}

void Catapult::initializePositions()
{
	super::initializePositions();
}

void Catapult::initializeListeners()
{
	super::initializeListeners();

	this->catapultCollision->whenCollidesWith({ (int)PlatformerCollisionType::Physics, (int)PlatformerCollisionType::Solid, (int)PlatformerCollisionType::Player, (int)PlatformerCollisionType::Force }, [=](CollisionData collisionData)
	{
		return CollisionResult::DoNothing;
	});

	this->catapultCollision->whenCollidesWith({ (int)PlatformerCollisionType::Player }, [=](CollisionData collisionData)
	{
		this->interactionEnabled = true;

		return CollisionResult::DoNothing;
	});

	this->catapultCollision->whenStopsCollidingWith({ (int)PlatformerCollisionType::Player }, [=](CollisionData collisionData)
	{
		this->interactionEnabled = false;

		return CollisionResult::DoNothing;
	});

	this->whenKeyPressed({ InputEvents::KeyCode::KEY_V }, [=](InputEvents::KeyboardEventArgs* args)
	{
		if (this->interactionEnabled && this->currentCooldown <= 0.0f)
		{
			this->launchCatapult();
		}
	});
}

void Catapult::update(float dt)
{
	super::update(dt);

	if (this->currentCooldown > 0.0f)
	{
		this->currentCooldown -= dt;
	}
}

Vec2 Catapult::getButtonOffset()
{
	return Vec2(0.0f, 0.0f);
}

void Catapult::registerHackables()
{
	super::registerHackables();

	HackableCode::CodeInfoMap codeInfoMap =
	{
		{
			LOCAL_FUNC_ID_APPLY_POWER,
			HackableCode::HackableCodeInfo(
				Catapult::MapKey,
				Strings::Menus_Hacking_Objects_Catapult_ApplyPower_ApplyPower::create(),
				HackableBase::HackBarColor::Purple,
				UIResources::Menus_Icons_Meteor,
				CatapultApplyPowerPreview::create(),
				{
					{ HackableCode::Register::zax, Strings::Menus_Hacking_Objects_Catapult_ApplyPower_RegisterEax::create() },
					{ HackableCode::Register::xmm0, Strings::Menus_Hacking_Objects_Catapult_ApplyPower_RegisterXmm0::create() },
					{ HackableCode::Register::xmm1, Strings::Menus_Hacking_Objects_Catapult_ApplyPower_RegisterXmm1::create() },
				},
				int(HackFlags::None),
				20.0f,
				0.0f
			)
		},
	};

	auto applyLaunchFunc = &Catapult::applyLaunchPower;
	std::vector<HackableCode*> hackables = HackableCode::create((void*&)applyLaunchFunc, codeInfoMap);

	for (auto next : hackables)
	{
		this->registerCode(next);
	}
}

HackablePreview* Catapult::createDefaultPreview()
{
	return CatapultGenericPreview::create();
}


void Catapult::launchCatapult()
{
	this->currentCooldown = Catapult::InteractCooldown;
	this->catapultAnimations->playAnimation("Launch");

	this->runAction(Sequence::create(
		DelayTime::create(0.2f),
		CallFunc::create([=]()
		{
			this->launchBall();
		}),
		nullptr
	));
}

void Catapult::launchBall()
{
	CatapultBall* catapultBall = CatapultBall::create(ValueMap(), this->applyLaunchPower(Catapult::LaunchVelocityBase));

	this->ballAnimationPart->replaceWithObject(catapultBall, 0.0f, 0.0f);
	catapultBall->setScale(this->catapultAnimations->getScale());
	catapultBall->setRotation(0.0f);

	ObjectEvents::TriggerObjectSpawn(RequestObjectSpawnArgs(
		this->ballAnimationPart,
		catapultBall,
		SpawnMethod::Above,
		PositionMode::SetToOwner,
		[&]()
		{
		},
		[&]()
		{
			catapultBall = nullptr;
		}
	));
}

NO_OPTIMIZE cocos2d::Vec2 Catapult::applyLaunchPower(cocos2d::Vec2 baseSpeed)
{
	volatile static float* freeMemoryForUser = new float[16];
	volatile float resultSpeedY = this->launchPower;
	volatile float* launchPowerPtr = &resultSpeedY;
	volatile float* ySpeedPtr = &baseSpeed.y;

	// Prepare variables (initialize xmm0 with return value, xmm1 with loaded density)
	ASM(push ZAX);
	ASM_MOV_REG_PTR(ZAX, ySpeedPtr);
	ASM(movss xmm0, dword ptr [ZAX]);
	ASM_MOV_REG_PTR(ZAX, launchPowerPtr);
	ASM(movss xmm1, dword ptr [ZAX]);
	ASM_MOV_REG_PTR(ZAX, freeMemoryForUser);

	HACKABLE_CODE_BEGIN(LOCAL_FUNC_ID_APPLY_POWER);
	ASM(mulss xmm0, xmm1);
	ASM_NOP16();
	HACKABLE_CODE_END();
	
	// Copy from xmm0 to the output variable
	ASM_MOV_REG_PTR(ZAX, launchPowerPtr);
	ASM(movss dword ptr [ZAX], xmm0);
	ASM(pop ZAX);

	HACKABLES_STOP_SEARCH();

	return Vec2(baseSpeed.x, MathUtils::clamp(resultSpeedY, -Catapult::LaunchVelocityBase.y, Catapult::LaunchVelocityBase.y));
}
END_NO_OPTIMIZE
