#include "ElectricityBeam.h"

#include "cocos/2d/CCActionInstant.h"
#include "cocos/2d/CCActionInterval.h"
#include "cocos/2d/CCSprite.h"
#include "cocos/base/CCValue.h"

#include "Engine/Animations/SmartAnimationSequenceNode.h"
#include "Engine/Hackables/HackableCode.h"
#include "Engine/Optimization/LazyNode.h"
#include "Engine/Physics/CollisionObject.h"
#include "Engine/Utils/GameUtils.h"
#include "Objects/Platformer/Traps/ElectricityBeam/ElectricityBeamCountDownPreview.h"
#include "Objects/Platformer/Traps/ElectricityBeam/ElectricityBeamGenericPreview.h"
#include "Scenes/Platformer/Hackables/HackFlags.h"
#include "Scenes/Platformer/Level/Physics/PlatformerPhysicsTypes.h"

#include "Resources/ObjectResources.h"
#include "Resources/UIResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

#define LOCAL_FUNC_ID_INCREMENT_ANIMATION_FRAME 1

const std::string ElectricityBeam::MapKey = "electricity-beam";
const std::string ElectricityBeam::PropertyVertical = "vertical";
const std::string ElectricityBeam::PropertyDisabled = "disabled";
const std::string ElectricityBeam::PropertyDisableSave = "disable-save";
const std::string ElectricityBeam::SaveKeyDisabled = "SAVE_KEY_DISABLED";

ElectricityBeam* ElectricityBeam::create(ValueMap& properties)
{
	ElectricityBeam* instance = new ElectricityBeam(properties);

	instance->autorelease();

	return instance;
}

ElectricityBeam::ElectricityBeam(ValueMap& properties) : super(properties)
{
	this->currentElectricityBeamCountDown = RandomHelper::random_real(0.0f, 3.0f);
	this->isVertical = GameUtils::getKeyOrDefault(this->properties, ElectricityBeam::PropertyVertical, Value(false)).asBool();
	this->isDisabled = GameUtils::getKeyOrDefault(this->properties, ElectricityBeam::PropertyDisabled, Value(false)).asBool();
	this->disableSave = GameUtils::getKeyOrDefault(this->properties, ElectricityBeam::PropertyDisableSave, Value(false)).asBool();

	this->electricityAnimation = SmartAnimationSequenceNode::create();
	this->electricityCollision = CollisionObject::create(CollisionObject::createBox(CSize(this->isVertical ? 64.0f : 468.0f, this->isVertical ? 468.0f : 64.0f)), (CollisionType)PlatformerCollisionType::Damage, CollisionObject::Properties(false, false));
	this->ballLeft = Sprite::create(ObjectResources::Traps_ElectricBeam_Ball);
	this->ballRight = Sprite::create(ObjectResources::Traps_ElectricBeam_Ball);

	this->electricityAnimation->setRotation(this->isVertical ? 90.0f : 0.0f);
	this->electricityCollision->setPhysicsFlagEnabled(false);

	this->addChild(this->electricityCollision);
	this->addChild(this->electricityAnimation);
	this->addChild(this->ballLeft);
	this->addChild(this->ballRight);
}

ElectricityBeam::~ElectricityBeam()
{
}

void ElectricityBeam::onEnter()
{
	super::onEnter();

	if (!this->disableSave)
	{
		this->isDisabled = this->loadObjectStateOrDefault(ElectricityBeam::SaveKeyDisabled, Value(this->isDisabled)).asBool();
	}

	this->scheduleUpdate();
}

void ElectricityBeam::initializePositions()
{
	super::initializePositions();

	this->electricityCollision->setPosition(Vec2::ZERO);
	this->ballLeft->setPosition(Vec2(this->isVertical ? 0.0f : -256.0f, this->isVertical ? -256.0f : 0.0f));
	this->ballRight->setPosition(Vec2(this->isVertical ? 0.0f : 256.0f, this->isVertical ? 256.0f : 0.0f));
}

void ElectricityBeam::initializeListeners()
{
	super::initializeListeners();

	this->listenForMapEvent(this->listenEvent, [=](ValueMap args)
	{
		this->toggleDisabled();
	});
}

void ElectricityBeam::toggleDisabled()
{
	this->isDisabled = !this->isDisabled;

	if (!this->disableSave)
	{
		this->saveObjectState(ElectricityBeam::SaveKeyDisabled, Value(this->isDisabled));
	}
}

void ElectricityBeam::update(float dt)
{
	super::update(dt);

	if (this->isDisabled)
	{
		return;
	}

	this->storedDt = dt;
	this->updateElectricityBeam();

	if (this->currentElectricityBeamCountDown <= 0.0f)
	{
		this->isRunningAnimation = true;
		this->currentElectricityBeamCountDown = this->maxElectricityBeamCountDown - RandomHelper::random_real(0.0f, 0.5f);

		this->electricityAnimation->playAnimation(ObjectResources::Traps_ElectricBeam_BeamFire_0000, 0.05f, true);

		this->runAction(Sequence::create(
			DelayTime::create(0.15f),
			CallFunc::create([=]()
			{
				this->electricityCollision->setPhysicsFlagEnabled(true);
			}),
			DelayTime::create(0.35f),
			CallFunc::create([=]()
			{
				this->electricityCollision->setPhysicsFlagEnabled(false);
				this->isRunningAnimation = false;
			}),
			nullptr
		));
	}
}

Vec2 ElectricityBeam::getButtonOffset()
{
	return Vec2(0.0f, 0.0f);
}

void ElectricityBeam::registerHackables()
{
	super::registerHackables();
	
	HackableCode::CodeInfoMap codeInfoMap =
	{
		{
			LOCAL_FUNC_ID_INCREMENT_ANIMATION_FRAME,
			HackableCode::HackableCodeInfo(
				ElectricityBeam::MapKey,
				Strings::Menus_Hacking_Objects_ElectricityBeam_UpdateCountDown_UpdateCountDown::create(),
				HackableBase::HackBarColor::Blue,
				UIResources::Menus_Icons_Lightning,
				LazyNode<HackablePreview>::create([=](){ return ElectricityBeamCountDownPreview::create(); }),
				{
					{ HackableCode::Register::zbx, Strings::Menus_Hacking_Objects_ElectricityBeam_UpdateCountDown_RegisterSt0::create() },
				},
				int(HackFlags::Lightning),
				20.0f,
				0.0f
			)
		},
	};

	auto updateElectricityBeamFunc = &ElectricityBeam::updateElectricityBeam;
	std::vector<HackableCode*> hackables = HackableCode::create((void*&)updateElectricityBeamFunc, codeInfoMap);

	for (HackableCode* next : hackables)
	{
		this->registerCode(next);
	}
}

HackablePreview* ElectricityBeam::createDefaultPreview()
{
	return ElectricityBeamGenericPreview::create();
}

NO_OPTIMIZE void ElectricityBeam::updateElectricityBeam()
{
	if (this->isRunningAnimation)
	{
		return;
	}

	static volatile float* countDownPtr;
	static volatile float* deltaTimePtr;

	countDownPtr = &this->currentElectricityBeamCountDown;
	deltaTimePtr = &this->storedDt;

	ASM(push ZAX);
	ASM(push ZBX);

	ASM_MOV_REG_PTR(ZAX, countDownPtr);
	ASM_MOV_REG_PTR(ZBX, deltaTimePtr);

	ASM(fld dword ptr [ZAX]);

	HACKABLE_CODE_BEGIN(LOCAL_FUNC_ID_INCREMENT_ANIMATION_FRAME);
	ASM(fsub dword ptr [ZBX]);
	ASM_NOP12();
	HACKABLE_CODE_END();

	ASM(fstp dword ptr [ZAX])

	ASM(pop ZBX);
	ASM(pop ZAX);

	HACKABLES_STOP_SEARCH();
}
END_NO_OPTIMIZE
