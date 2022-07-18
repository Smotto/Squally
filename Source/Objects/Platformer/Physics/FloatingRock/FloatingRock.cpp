#include "FloatingRock.h"

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

#include "Objects/Platformer/Physics/FloatingRock/FloatingRockGenericPreview.h"
#include "Objects/Platformer/Physics/FloatingRock/FloatingRockGetDensityPreview.h"
#include "Scenes/Platformer/Hackables/HackFlags.h"
#include "Scenes/Platformer/Level/Physics/PlatformerPhysicsTypes.h"

#include "Resources/ObjectResources.h"
#include "Resources/UIResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

#define LOCAL_FUNC_ID_GET_DENSITY 1

const std::string FloatingRock::MapKey = "floating-rock";

FloatingRock* FloatingRock::create(ValueMap& properties)
{
	FloatingRock* instance = new FloatingRock(properties);
	
	instance->autorelease();

	return instance;
}

FloatingRock::FloatingRock(ValueMap& properties) : super(properties)
{
	this->sprite = Sprite::create(ObjectResources::Physics_RollingStone_RollingStone);
	this->collision = CollisionObject::create(CollisionObject::createCircle(96.0f), (CollisionType)PlatformerCollisionType::Physics, CollisionObject::Properties(false, true));

	this->collision->whenCollidesWith({ (int)PlatformerCollisionType::Physics, (int)PlatformerCollisionType::Solid, (int)PlatformerCollisionType::Player, (int)PlatformerCollisionType::Force }, [=](CollisionData collisionData)
	{
		return CollisionResult::CollideWithPhysics;
	});

	this->collision->addChild(this->sprite);
	this->contentNode->addChild(this->collision);
}

FloatingRock::~FloatingRock()
{
}

void FloatingRock::onEnter()
{
	super::onEnter();

	this->scheduleUpdate();
}

void FloatingRock::initializePositions()
{
	super::initializePositions();
}

void FloatingRock::update(float dt)
{
	super::update(dt);
}

Vec2 FloatingRock::getButtonOffset()
{
	return Vec2(0.0f, 0.0f);
}

void FloatingRock::registerHackables()
{
	super::registerHackables();

	HackableCode::CodeInfoMap codeInfoMap =
	{
		{
			LOCAL_FUNC_ID_GET_DENSITY,
			HackableCode::HackableCodeInfo(
				FloatingRock::MapKey,
				Strings::Menus_Hacking_Objects_FloatingObjects_GetDensity_GetDensity::create(),
				HackableBase::HackBarColor::Purple,
				UIResources::Menus_Icons_Anvil,
				LazyNode<HackablePreview>::create([=](){ return FloatingRockGetDensityPreview::create(); }),
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

	auto densityFunc = &FloatingRock::getDensityNonVirtual;
	std::vector<HackableCode*> hackables = HackableCode::create((void*&)densityFunc, codeInfoMap);

	for (HackableCode* next : hackables)
	{
		this->registerCode(next);
	}
}

HackablePreview* FloatingRock::createDefaultPreview()
{
	return FloatingRockGenericPreview::create();
}

float FloatingRock::getDensity()
{
	return this->getDensityNonVirtual();
}

NO_OPTIMIZE float FloatingRock::getDensityNonVirtual()
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

float FloatingRock::getObjectHeight()
{
	return this->sprite == nullptr ? 0.0f : this->sprite->getContentSize().height;
}
