#include "SquallySwimHackBehavior.h"

#include "cocos/2d/CCActionInstant.h"
#include "cocos/2d/CCActionInterval.h"
#include "cocos/2d/CCActionEase.h"
#include "cocos/base/CCEventCustom.h"
#include "cocos/base/CCEventListenerCustom.h"

#include "Engine/Animations/AnimationPart.h"
#include "Engine/Animations/SmartAnimationNode.h"
#include "Engine/Animations/SmartAnimationSequenceNode.h"
#include "Engine/Hackables/HackableCode.h"
#include "Engine/Optimization/LazyNode.h"
#include "Entities/Platformer/EntityPreview.h"
#include "Entities/Platformer/Squally/Squally.h"
#include "Scenes/Platformer/Hackables/HackFlags.h"
#include "Scenes/Platformer/State/StateKeys.h"

#include "Resources/EntityResources.h"
#include "Resources/UIResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

#define LOCAL_FUNC_ID_CAN_SWIM 1

const std::string SquallySwimHackBehavior::MapKey = "squally-is-swimming";
const std::string SquallySwimHackBehavior::HackIdentifierIsSwimming = "squally-is-swimming";

SquallySwimHackBehavior* SquallySwimHackBehavior::create(GameObject* owner)
{
	SquallySwimHackBehavior* instance = new SquallySwimHackBehavior(owner);

	instance->autorelease();

	return instance;
}

SquallySwimHackBehavior::SquallySwimHackBehavior(GameObject* owner) : super(owner)
{
	this->squally = dynamic_cast<Squally*>(owner);

	if (this->squally == nullptr)
	{
		this->invalidate();
	}
}

SquallySwimHackBehavior::~SquallySwimHackBehavior()
{
}

void SquallySwimHackBehavior::update(float dt)
{
	super::update(dt);

	if (this->canSwimHack())
	{
		this->squally->controlStateOverride = PlatformerEntity::ControlState::Swimming;
	}
	else if (this->squally->controlStateOverride == PlatformerEntity::ControlState::Swimming)
	{
		this->squally->controlStateOverride = PlatformerEntity::ControlState::None;
	}
}

void SquallySwimHackBehavior::onLoad()
{
	this->registerHackables();
	this->scheduleUpdate();
}

void SquallySwimHackBehavior::onDisable()
{
	super::onDisable();
}

void SquallySwimHackBehavior::registerHackables()
{
	HackableCode::CodeInfoMap codeInfoMap =
	{
		{
			LOCAL_FUNC_ID_CAN_SWIM,
			HackableCode::HackableCodeInfo(
				SquallySwimHackBehavior::HackIdentifierIsSwimming,
				Strings::Menus_Hacking_Abilities_Squally_IsSwimming::create(),
				HackableBase::HackBarColor::Teal,
				UIResources::Menus_Icons_Bubbles,
				LazyNode<HackablePreview>::create([=](){ return (HackablePreview*)EntityPreview::create(this->squally); }),
				{
					{ HackableCode::Register::zax, Strings::Menus_Hacking_ClippyHelp_Abilities_IsSwimming_RegisterEax::create() },
				},
				int(HackFlags::Water),
				1.0f,
				2.0f
			)
		},
	};

	this->hackables = CREATE_HACKABLES(SquallySwimHackBehavior::canSwimHack, codeInfoMap);

	for (HackableCode* next : hackables)
	{
		this->squally->registerCode(next);
	}
}

NO_OPTIMIZE bool SquallySwimHackBehavior::canSwimHack()
{
	static volatile int canSwim = 0;

	ASM(PUSH ZAX);

	HACKABLE_CODE_BEGIN(LOCAL_FUNC_ID_CAN_SWIM);
	ASM(mov ZAX, 0);
	ASM_NOP16();
	HACKABLE_CODE_END();

	ASM_MOV_VAR_REG(canSwim, eax);
	ASM(POP ZAX);
	HACKABLES_STOP_SEARCH();

	return canSwim;
}
END_NO_OPTIMIZE
