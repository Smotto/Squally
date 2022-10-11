#include "StoneSkin.h"

#include "cocos/2d/CCActionInstant.h"
#include "cocos/2d/CCActionInterval.h"
#include "cocos/2d/CCSprite.h"

#include "Engine/Animations/SmartAnimationSequenceNode.h"
#include "Engine/Events/ObjectEvents.h"
#include "Engine/Hackables/HackableCode.h"
#include "Engine/Hackables/HackableObject.h"
#include "Engine/Hackables/Menus/HackablePreview.h"
#include "Engine/Optimization/LazyNode.h"
#include "Engine/Particles/SmartParticles.h"
#include "Engine/Localization/ConstantFloat.h"
#include "Engine/Localization/ConstantString.h"
#include "Engine/Sound/WorldSound.h"
#include "Engine/Utils/GameUtils.h"
#include "Engine/Utils/MathUtils.h"
#include "Entities/Platformer/PlatformerEntity.h"
#include "Events/CombatEvents.h"
#include "Events/PlatformerEvents.h"
#include "Scenes/Platformer/Hackables/HackFlags.h"
#include "Scenes/Platformer/Level/Combat/Attacks/Buffs/StoneSkin/StoneSkinGenericPreview.h"
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

#define LOCAL_FUNC_ID_STONE_SKIN 1

const std::string StoneSkin::StoneSkinIdentifier = "stone-skin";

const int StoneSkin::MaxMultiplier = 4;
const float StoneSkin::Duration = 16.0f;

// Static to prevent GCC optimization issues
volatile int StoneSkin::currentDamageTaken = 0;

StoneSkin* StoneSkin::create(PlatformerEntity* caster, PlatformerEntity* target)
{
	StoneSkin* instance = new StoneSkin(caster, target);

	instance->autorelease();

	return instance;
}

StoneSkin::StoneSkin(PlatformerEntity* caster, PlatformerEntity* target)
	: super(caster, target, UIResources::Menus_Icons_ShieldBroken, AbilityType::Physical, BuffData(StoneSkin::Duration, StoneSkin::StoneSkinIdentifier))
{
	this->spellEffect = SmartParticles::create(ParticleResources::Platformer_Combat_Abilities_Gray);
	this->bubble = Sprite::create(FXResources::Auras_DefendAura);
	this->spellAura = Sprite::create(FXResources::Auras_ChantAura2);
	this->currentDamageTaken = 0;

	this->bubble->setOpacity(0);
	this->spellAura->setColor(Color3B::YELLOW);
	this->spellAura->setOpacity(0);

	this->addChild(this->spellEffect);
	this->addChild(this->bubble);
	this->addChild(this->spellAura);
}

StoneSkin::~StoneSkin()
{
}

void StoneSkin::onEnter()
{
	super::onEnter();

	this->spellEffect->setPositionY(this->owner->getEntityBottomPointRelative().y);
	this->spellEffect->start();

	this->bubble->runAction(FadeTo::create(0.25f, 255));

	this->spellAura->runAction(Sequence::create(
		FadeTo::create(0.25f, 255),
		DelayTime::create(0.5f),
		FadeTo::create(0.25f, 0),
		nullptr
	));

	CombatEvents::TriggerHackableCombatCue();
}

void StoneSkin::initializePositions()
{
	super::initializePositions();
}

void StoneSkin::registerHackables()
{
	super::registerHackables();

	if (this->owner == nullptr)
	{
		return;
	}

	HackableCode::CodeInfoMap codeInfoMap =
	{
		{
			LOCAL_FUNC_ID_STONE_SKIN,
			HackableCode::HackableCodeInfo(
				StoneSkin::StoneSkinIdentifier,
				Strings::Menus_Hacking_Abilities_Buffs_StoneSkin_StoneSkin::create(),
				HackableBase::HackBarColor::Gray,
				UIResources::Menus_Icons_ShieldBroken,
				LazyNode<HackablePreview>::create([=](){ return StoneSkinGenericPreview::create(); }),
				{
					{
						HackableCode::Register::zax, Strings::Menus_Hacking_Abilities_Buffs_StoneSkin_RegisterEax::create()->setStringReplacementVariables(
							{
								Strings::Common_ConstantTimes::create()->setStringReplacementVariables(ConstantString::create(std::to_string(-StoneSkin::MaxMultiplier))),
								Strings::Common_ConstantTimes::create()->setStringReplacementVariables(ConstantString::create(std::to_string(StoneSkin::MaxMultiplier))),
							})
					},
					{
						HackableCode::Register::zcx, Strings::Menus_Hacking_Abilities_Buffs_StoneSkin_RegisterEcx::create()
					},
					{
						HackableCode::Register::zdx, Strings::Menus_Hacking_Abilities_Buffs_StoneSkin_RegisterEdx::create()
					},
				},
				int(HackFlags::None),
				this->getRemainingDuration(),
				0.0f,
				{
					HackableCode::ReadOnlyScript(
						Strings::Menus_Hacking_CodeEditor_OriginalCode::create(),
						// x86
						COMMENT(Strings::Menus_Hacking_Abilities_Buffs_StoneSkin_CommentDivisor::create()) + 
						"mov ecx, 3\n" +
						COMMENT(Strings::Menus_Hacking_Abilities_Buffs_StoneSkin_CommentDivide::create()
							->setStringReplacementVariables({ Strings::Menus_Hacking_Lexicon_Assembly_RegisterEax::create(), Strings::Menus_Hacking_Lexicon_Assembly_RegisterEcx::create() })) + 
						COMMENT(Strings::Menus_Hacking_Abilities_Buffs_StoneSkin_CommentDivideByZero::create()) +
						"idiv ecx\n"
						, // x64
						COMMENT(Strings::Menus_Hacking_Abilities_Buffs_StoneSkin_CommentDivisor::create()) + 
						"mov rcx, 3\n" +
						COMMENT(Strings::Menus_Hacking_Abilities_Buffs_StoneSkin_CommentDivide::create()
							->setStringReplacementVariables({ Strings::Menus_Hacking_Lexicon_Assembly_RegisterRax::create(), Strings::Menus_Hacking_Lexicon_Assembly_RegisterRcx::create() })) + 
						COMMENT(Strings::Menus_Hacking_Abilities_Buffs_StoneSkin_CommentDivideByZero::create()) +
						"idiv rcx\n"
					),
					HackableCode::ReadOnlyScript(
						Strings::Menus_Hacking_Abilities_Buffs_StoneSkin_ReduceStoneSkin::create(),
						// x86
						COMMENT(Strings::Menus_Hacking_Abilities_Buffs_StoneSkin_CommentDivisor::create()) + 
						"; mov ecx, 3\n" +
						COMMENT(Strings::Menus_Hacking_Abilities_Buffs_StoneSkin_CommentDivide::create()
							->setStringReplacementVariables({ Strings::Menus_Hacking_Lexicon_Assembly_RegisterEax::create(), Strings::Menus_Hacking_Lexicon_Assembly_RegisterEcx::create() })) + 
						COMMENT(Strings::Menus_Hacking_Abilities_Buffs_StoneSkin_CommentDivideByZero::create()) +
						"; idiv ecx\n\n" +
						COMMENT(Strings::Menus_Hacking_Abilities_Buffs_StoneSkin_CommentMultiplyPt1::create()) +
						COMMENT(Strings::Menus_Hacking_Abilities_Buffs_StoneSkin_CommentMultiplyPt2::create()) +
						"imul eax, 0\n"
						, // x64
						COMMENT(Strings::Menus_Hacking_Abilities_Buffs_StoneSkin_CommentDivisor::create()) + 
						"; mov rcx, 3\n" +
						COMMENT(Strings::Menus_Hacking_Abilities_Buffs_StoneSkin_CommentDivide::create()
							->setStringReplacementVariables({ Strings::Menus_Hacking_Lexicon_Assembly_RegisterRax::create(), Strings::Menus_Hacking_Lexicon_Assembly_RegisterRcx::create() })) + 
						COMMENT(Strings::Menus_Hacking_Abilities_Buffs_StoneSkin_CommentDivideByZero::create()) +
						"; idiv rcx\n\n" +
						COMMENT(Strings::Menus_Hacking_Abilities_Buffs_StoneSkin_CommentMultiplyPt1::create()) +
						COMMENT(Strings::Menus_Hacking_Abilities_Buffs_StoneSkin_CommentMultiplyPt2::create()) +
						"imul rax, 0\n"
					)
				},
				true
			)
		},
	};

	auto stoneSkinFunc = &StoneSkin::applyStoneSkin;
	this->hackables = HackableCode::create((void*&)stoneSkinFunc, codeInfoMap);

	for (HackableCode* next : this->hackables)
	{
		this->owner->registerCode(next);
	}
}

void StoneSkin::onBeforeDamageTaken(CombatEvents::ModifiableDamageOrHealingArgs* damageOrHealing)
{
	super::onBeforeDamageTaken(damageOrHealing);

	this->currentDamageTaken = damageOrHealing->damageOrHealingValue;

	this->applyStoneSkin();

	(*damageOrHealing->damageOrHealing) = this->currentDamageTaken;
	(*damageOrHealing->damageOrHealingMin) = -std::abs(damageOrHealing->damageOrHealingValue * StoneSkin::MaxMultiplier);
	(*damageOrHealing->damageOrHealingMax) = std::abs(damageOrHealing->damageOrHealingValue * StoneSkin::MaxMultiplier);
}

NO_OPTIMIZE void StoneSkin::applyStoneSkin()
{
	static volatile int currentDamageTakenLocal = 0;

	currentDamageTakenLocal = this->currentDamageTaken;

	ASM_PUSH_EFLAGS()
	ASM(push ZAX);
	ASM(push ZCX);
	ASM(push ZDX);

	ASM_MOV_REG_VAR(eax, currentDamageTakenLocal);
	ASM(DIV_CONVERT);

	HACKABLE_CODE_BEGIN(LOCAL_FUNC_ID_STONE_SKIN);
	ASM(mov ZCX, 3)
	ASM(idiv ZCX);
	ASM_NOP16();
	HACKABLE_CODE_END();

	ASM_MOV_VAR_REG(currentDamageTakenLocal, eax);

	ASM(pop ZDX);
	ASM(pop ZCX);
	ASM(pop ZAX);
	ASM_POP_EFLAGS()

	this->currentDamageTaken = currentDamageTakenLocal;

	HACKABLES_STOP_SEARCH();
}
END_NO_OPTIMIZE
