#include "CurseOfTongues.h"

#include "cocos/2d/CCActionInstant.h"
#include "cocos/2d/CCActionInterval.h"
#include "cocos/2d/CCSprite.h"

#include "Engine/Animations/SmartAnimationSequenceNode.h"
#include "Engine/Events/ObjectEvents.h"
#include "Engine/Hackables/HackableCode.h"
#include "Engine/Hackables/HackableObject.h"
#include "Engine/Hackables/Menus/HackablePreview.h"
#include "Engine/Localization/ConcatString.h"
#include "Engine/Localization/ConstantString.h"
#include "Engine/Optimization/LazyNode.h"
#include "Engine/Particles/SmartParticles.h"
#include "Engine/Localization/ConstantFloat.h"
#include "Engine/Sound/WorldSound.h"
#include "Engine/Utils/GameUtils.h"
#include "Engine/Utils/MathUtils.h"
#include "Entities/Platformer/PlatformerEntity.h"
#include "Events/CombatEvents.h"
#include "Events/PlatformerEvents.h"
#include "Scenes/Platformer/Hackables/HackFlags.h"
#include "Scenes/Platformer/Level/Combat/Attacks/Debuffs/CurseOfTongues/CurseOfTonguesGenericPreview.h"
#include "Scenes/Platformer/Level/Combat/CombatMap.h"
#include "Scenes/Platformer/Level/Combat/TimelineEvent.h"
#include "Scenes/Platformer/Level/Combat/TimelineEventGroup.h"

#include "Resources/FXResources.h"
#include "Resources/ObjectResources.h"
#include "Resources/ParticleResources.h"
#include "Resources/SoundResources.h"
#include "Resources/UIResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

#define LOCAL_FUNC_ID_CURSE_OF_TONGUES 1

const std::string CurseOfTongues::CurseOfTonguesIdentifier = "curse-of-tongues";

// Note: UI sets precision on these to 1 digit
const float CurseOfTongues::MinSpeed = -1.25f;
const float CurseOfTongues::DefaultSpeed = -1.25f;
const float CurseOfTongues::DefaultHackSpeed = -0.5f; // Keep in sync with the asm
const float CurseOfTongues::MaxSpeed = 1.0f;
const float CurseOfTongues::Duration = 6.0f;

// Static to prevent GCC optimization issues
volatile float CurseOfTongues::currentSpeed = 0.0f;

CurseOfTongues* CurseOfTongues::create(PlatformerEntity* caster, PlatformerEntity* target)
{
	CurseOfTongues* instance = new CurseOfTongues(caster, target);

	instance->autorelease();

	return instance;
}

CurseOfTongues::CurseOfTongues(PlatformerEntity* caster, PlatformerEntity* target)
	: super(caster, target, UIResources::Menus_Icons_Voodoo, AbilityType::Shadow, BuffData(CurseOfTongues::Duration, CurseOfTongues::CurseOfTonguesIdentifier))
{
	this->spellEffect = SmartParticles::create(ParticleResources::Platformer_Combat_Abilities_Curse);
	this->spellAura = Sprite::create(FXResources::Auras_ChantAura);

	this->spellAura->setColor(Color3B::MAGENTA);
	this->spellAura->setOpacity(0);

	this->addChild(this->spellEffect);
	this->addChild(this->spellAura);
}

CurseOfTongues::~CurseOfTongues()
{
}

void CurseOfTongues::onEnter()
{
	super::onEnter();

	this->spellEffect->setPositionY(this->owner->getEntityBottomPointRelative().y);
	this->spellEffect->start();

	this->spellAura->runAction(Sequence::create(
		FadeTo::create(0.25f, 255),
		DelayTime::create(0.5f),
		FadeTo::create(0.25f, 0),
		nullptr
	));

	CombatEvents::TriggerHackableCombatCue();
}

void CurseOfTongues::initializePositions()
{
	super::initializePositions();
}

void CurseOfTongues::registerHackables()
{
	super::registerHackables();

	if (this->owner == nullptr)
	{
		return;
	}

	HackableCode::CodeInfoMap codeInfoMap =
	{
		{
			LOCAL_FUNC_ID_CURSE_OF_TONGUES,
			HackableCode::HackableCodeInfo(
				CurseOfTongues::CurseOfTonguesIdentifier,
				Strings::Menus_Hacking_Abilities_Debuffs_CurseOfTongues_CurseOfTongues::create(),
				HackableBase::HackBarColor::Purple,
				UIResources::Menus_Icons_Voodoo,
				LazyNode<HackablePreview>::create([=](){ return CurseOfTonguesGenericPreview::create(); }),
				{
					{
						HackableCode::Register::zsi, Strings::Menus_Hacking_Abilities_Debuffs_CurseOfTongues_RegisterEsi::create(), true
					},
					{
						HackableCode::Register::xmm3, Strings::Menus_Hacking_Abilities_Debuffs_CurseOfTongues_RegisterXmm3::create()
							->setStringReplacementVariables(ConstantFloat::create(CurseOfTongues::DefaultSpeed, 2)), true
					}
				},
				int(HackFlags::None),
				this->getRemainingDuration(),
				0.0f,
				{
					HackableCode::ReadOnlyScript(
						Strings::Menus_Hacking_Abilities_Debuffs_CurseOfTongues_ReduceCurse::create(),
						// x86
						ConcatString::create({
							Strings::Menus_Hacking_Abilities_Debuffs_CurseOfTongues_CommentSpeed::create()
								->setStringReplacementVariables({ ConstantFloat::create(CurseOfTongues::DefaultHackSpeed, 1), ConstantFloat::create(CurseOfTongues::DefaultSpeed, 2) }),
							Strings::Menus_Hacking_Abilities_Debuffs_CurseOfTongues_CommentGainInstead::create(),
							ConstantString::create("mov dword ptr [esi], -0.5f\n\n"),
							Strings::Menus_Hacking_Abilities_Generic_CommentBreak::create(),
							Strings::Menus_Hacking_Abilities_Generic_CommentFloatPt1::create(),
							Strings::Menus_Hacking_Abilities_Generic_CommentFloatPt2::create(),
							Strings::Menus_Hacking_Abilities_Generic_CommentFloatPt3::create(),
							Strings::Menus_Hacking_Abilities_Generic_CommentFloatPt4::create(),
							Strings::Menus_Hacking_Abilities_Generic_CommentBreak::create()
						})
						, // x64
						ConcatString::create({
							Strings::Menus_Hacking_Abilities_Debuffs_CurseOfTongues_CommentSpeed::create()
								->setStringReplacementVariables({ ConstantFloat::create(CurseOfTongues::DefaultHackSpeed, 1), ConstantFloat::create(CurseOfTongues::DefaultSpeed, 2) }),
							Strings::Menus_Hacking_Abilities_Debuffs_CurseOfTongues_CommentGainInstead::create(),
							ConstantString::create("mov dword ptr [rsi], -0.5f\n\n"),
							Strings::Menus_Hacking_Abilities_Generic_CommentBreak::create(),
							Strings::Menus_Hacking_Abilities_Generic_CommentFloatPt1::create(),
							Strings::Menus_Hacking_Abilities_Generic_CommentFloatPt2::create(),
							Strings::Menus_Hacking_Abilities_Generic_CommentFloatPt3::create(),
							Strings::Menus_Hacking_Abilities_Generic_CommentFloatPt4::create(),
							Strings::Menus_Hacking_Abilities_Generic_CommentBreak::create()
						})
					)
				}
			)
		},
	};

	this->hackables = CREATE_HACKABLES(CurseOfTongues::applyCurseOfTongues, codeInfoMap);

	for (HackableCode* next : this->hackables)
	{
		this->owner->registerCode(next);
	}
}

void CurseOfTongues::onModifyTimelineSpeed(CombatEvents::ModifiableTimelineSpeedArgs* speed)
{
	super::onModifyTimelineSpeed(speed);
	
	this->currentSpeed = *(speed->speed);

	this->applyCurseOfTongues();

	*(speed->speed) = this->currentSpeed;
}

NO_OPTIMIZE void CurseOfTongues::applyCurseOfTongues()
{
	static volatile float speedBonus;
	static volatile float increment = 0.0f;
	static volatile float* speedBonusPtr;
	static volatile float* incrementPtr;

	speedBonus = 0.0f;
	increment = CurseOfTongues::DefaultSpeed;
	speedBonusPtr = &speedBonus;
	incrementPtr = &increment;

	ASM_PUSH_EFLAGS();
	ASM(push ZSI);
	ASM(push ZBX);
	ASM_MOV_REG_PTR(ZSI, speedBonusPtr);
	ASM_MOV_REG_PTR(ZBX, incrementPtr);
	ASM(movss xmm3, [ZBX]);

	HACKABLE_CODE_BEGIN(LOCAL_FUNC_ID_CURSE_OF_TONGUES);
	ASM(movss [ZSI], xmm3);
	ASM_NOP16();
	HACKABLE_CODE_END();

	ASM(pop ZBX);
	ASM(pop ZSI);
	ASM_POP_EFLAGS();

	this->currentSpeed = this->currentSpeed + MathUtils::clamp(speedBonus, CurseOfTongues::MinSpeed, CurseOfTongues::MaxSpeed);

	HACKABLES_STOP_SEARCH();
}
END_NO_OPTIMIZE
