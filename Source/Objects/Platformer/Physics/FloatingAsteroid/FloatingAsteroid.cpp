#include "FloatingAsteroid.h"

#include "cocos/2d/CCActionInstant.h"
#include "cocos/2d/CCActionInterval.h"
#include "cocos/2d/CCActionEase.h"
#include "cocos/2d/CCSprite.h"
#include "cocos/base/CCValue.h"

#include "Engine/Localization/LocalizedString.h"
#include "Engine/Hackables/HackableCode.h"
#include "Engine/Optimization/LazyNode.h"
#include "Engine/Physics/CollisionObject.h"
#include "Engine/Utils/GameUtils.h"
#include "Engine/Utils/MathUtils.h"

#include "Objects/Platformer/Physics/FloatingAsteroid/FloatingAsteroidGenericPreview.h"
#include "Objects/Platformer/Physics/FloatingAsteroid/FloatingAsteroidGetDensityPreview.h"
#include "Scenes/Platformer/Hackables/HackFlags.h"
#include "Scenes/Platformer/Level/Physics/PlatformerCollisionType.h"

#include "Resources/ObjectResources.h"
#include "Resources/UIResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

#define LOCAL_FUNC_ID_GET_DENSITY 1

const std::string FloatingAsteroid::MapKey = "floating-asteroid";

FloatingAsteroid* FloatingAsteroid::create(ValueMap& properties)
{
	FloatingAsteroid* instance = new FloatingAsteroid(properties);
	
	instance->autorelease();

	return instance;
}

FloatingAsteroid::FloatingAsteroid(ValueMap& properties) : super(properties)
{
	this->sprite = Sprite::create(ObjectResources::Physics_Asteroid_Asteroid);
	this->collision = CollisionObject::create(CollisionObject::createCircle(96.0f), (CollisionType)PlatformerCollisionType::Physics, CollisionObject::Properties(false, true));

	this->collision->whenCollidesWith({ (int)PlatformerCollisionType::Physics, (int)PlatformerCollisionType::Solid, (int)PlatformerCollisionType::Player, (int)PlatformerCollisionType::Force }, [=](CollisionData collisionData)
	{
		return CollisionResult::CollideWithPhysics;
	});

	this->collision->addChild(this->sprite);
	this->contentNode->addChild(this->collision);
}

FloatingAsteroid::~FloatingAsteroid()
{
}

void FloatingAsteroid::onEnter()
{
	super::onEnter();

	this->scheduleUpdate();
}

void FloatingAsteroid::initializePositions()
{
	super::initializePositions();
}

void FloatingAsteroid::update(float dt)
{
	super::update(dt);
}

Vec2 FloatingAsteroid::getButtonOffset()
{
	return Vec2(0.0f, 0.0f);
}

void FloatingAsteroid::registerHackables()
{
	super::registerHackables();

	HackableCode::CodeInfoMap codeInfoMap =
	{
		{
			LOCAL_FUNC_ID_GET_DENSITY,
			HackableCode::HackableCodeInfo(
				FloatingAsteroid::MapKey,
				Strings::Menus_Hacking_Objects_FloatingObjects_GetDensity_GetDensity::create(),
				HackableBase::HackBarColor::Purple,
				UIResources::Menus_Icons_Anvil,
				LazyNode<HackablePreview>::create([=](){ return FloatingAsteroidGetDensityPreview::create(); }),
				{
					{ HackableCode::Register::zax, Strings::Menus_Hacking_Objects_FloatingObjects_GetDensity_RegisterEax::create() },
					{ HackableCode::Register::xmm0, Strings::Menus_Hacking_Objects_FloatingObjects_GetDensity_RegisterXmm0::create() },
					{ HackableCode::Register::xmm1, Strings::Menus_Hacking_Objects_FloatingObjects_GetDensity_RegisterXmm1::create() },
				},
				int(HackFlags::None),
				8.0f,
				0.0f
			)
		},
	};

	auto densityFunc = &FloatingAsteroid::getDensityNonVirtual;
	std::vector<HackableCode*> hackables = HackableCode::create((void*&)densityFunc, codeInfoMap);

	for (HackableCode* next : hackables)
	{
		this->registerCode(next);
	}
}

HackablePreview* FloatingAsteroid::createDefaultPreview()
{
	return FloatingAsteroidGenericPreview::create();
}

float FloatingAsteroid::getDensity()
{
	return this->getDensityNonVirtual();
}

NO_OPTIMIZE float FloatingAsteroid::getDensityNonVirtual()
{
	volatile static float* freeMemoryForUser = new float[16];
	volatile float densityRet = 0.5f;
	volatile float* densityRetPtr = &densityRet;
	volatile float densityCopy = this->loadedDensity;
	volatile float* densityCopyPtr = &densityCopy;

	// Prepare variables (initialize xmm0 with return value, xmm1 with loaded density)
	ASM(push ZAX);
	ASM_MOV_REG_PTR(ZAX, densityRetPtr);
	ASM(movss xmm0, dword ptr [ZAX]);
	ASM_MOV_REG_PTR(ZAX, densityCopyPtr);
	ASM(movss xmm1, dword ptr [ZAX]);
	ASM_MOV_REG_PTR(ZAX, freeMemoryForUser);

	HACKABLE_CODE_BEGIN(LOCAL_FUNC_ID_GET_DENSITY);
	ASM(movss xmm0, xmm1);
	ASM_NOP16();
	HACKABLE_CODE_END();

	// Copy from xmm0 to the output variable
	ASM_MOV_REG_PTR(ZAX, densityRetPtr);
	ASM(movss dword ptr [ZAX], xmm0);
	ASM(pop ZAX);

	HACKABLES_STOP_SEARCH();

	return MathUtils::clamp(densityRet, 0.0f, 1.0f);
}
END_NO_OPTIMIZE

float FloatingAsteroid::getObjectHeight()
{
	return this->sprite == nullptr ? 0.0f : this->sprite->getContentSize().height;
}
