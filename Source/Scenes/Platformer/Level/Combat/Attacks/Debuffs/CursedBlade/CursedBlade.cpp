#include "CursedBlade.h"

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
#include "Engine/Localization/ConstantString.h"
#include "Engine/Sound/WorldSound.h"
#include "Engine/Utils/GameUtils.h"
#include "Engine/Utils/MathUtils.h"
#include "Entities/Platformer/PlatformerEntity.h"
#include "Events/CombatEvents.h"
#include "Events/PlatformerEvents.h"
#include "Scenes/Platformer/Hackables/HackFlags.h"
#include "Scenes/Platformer/Level/Combat/Attacks/Debuffs/CursedBlade/CursedBladeGenericPreview.h"
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

#define LOCAL_FUNC_ID_CURSED_BLADE 1

const std::string CursedBlade::CursedBladeIdentifier = "cursed-blade";
const std::string CursedBlade::HackIdentifierCursedBlade = "cursed-blade";

const int CursedBlade::MaxMultiplier = 4;
const float CursedBlade::DamageIncrease = 10.0f; // Keep in sync with asm
const float CursedBlade::Duration = 16.0f;

CursedBlade* CursedBlade::create(PlatformerEntity* caster, PlatformerEntity* target)
{
	CursedBlade* instance = new CursedBlade(caster, target);

	instance->autorelease();

	return instance;
}

CursedBlade::CursedBlade(PlatformerEntity* caster, PlatformerEntity* target)
	: super(caster, target, UIResources::Menus_Icons_ShieldGlowBlue, AbilityType::Physical, BuffData(CursedBlade::Duration, CursedBlade::CursedBladeIdentifier))
{
	this->spellEffect = SmartParticles::create(ParticleResources::Platformer_Combat_Abilities_Speed);
	this->spellAura = Sprite::create(FXResources::Auras_ChantAura2);

	this->spellAura->setColor(Color3B::YELLOW);
	this->spellAura->setOpacity(0);

	this->addChild(this->spellEffect);
	this->addChild(this->spellAura);
}

CursedBlade::~CursedBlade()
{
}

void CursedBlade::onEnter()
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

void CursedBlade::initializePositions()
{
	super::initializePositions();
}

void CursedBlade::registerHackables()
{
	super::registerHackables();

	if (this->owner == nullptr)
	{
		return;
	}

	HackableCode::CodeInfoMap codeInfoMap =
	{
		{
			LOCAL_FUNC_ID_CURSED_BLADE,
			HackableCode::HackableCodeInfo(
				CursedBlade::HackIdentifierCursedBlade,
				Strings::Menus_Hacking_Abilities_Debuffs_CursedBlade_CursedBlade::create(),
				HackableBase::HackBarColor::Purple,
				UIResources::Menus_Icons_ShieldGlowBlue,
				LazyNode<HackablePreview>::create([=](){ return CursedBladeGenericPreview::create(); }),
				{
					{
						HackableCode::Register::zdi, Strings::Menus_Hacking_Abilities_Debuffs_CursedBlade_RegisterEdi::create()
					},
					{
						HackableCode::Register::zsi, Strings::Menus_Hacking_Abilities_Debuffs_CursedBlade_RegisterEsi::create()
					}
				},
				int(HackFlags::None),
				this->getRemainingDuration(),
				0.0f,
				{
					HackableCode::ReadOnlyScript(
						Strings::Menus_Hacking_CodeEditor_OriginalCode::create(),
						// x86
						COMMENT(Strings::Menus_Hacking_Abilities_Debuffs_CursedBlade_CommentRegister::create()
							->setStringReplacementVariables(Strings::Menus_Hacking_Lexicon_Assembly_RegisterEbx::create())) + 
						COMMENT(Strings::Menus_Hacking_Abilities_Debuffs_CursedBlade_CommentDamageReduce::create()
							->setStringReplacementVariables(ConstantString::create(std::to_string(CursedBlade::DamageIncrease)))) + 
						"fld dword ptr [esi]\n"
						"fadd dword ptr [edi]\n"
						"fistp dword ptr [esi]\n"
						, // x64
						COMMENT(Strings::Menus_Hacking_Abilities_Debuffs_CursedBlade_CommentRegister::create()
							->setStringReplacementVariables(Strings::Menus_Hacking_Lexicon_Assembly_RegisterRbx::create())) + 
						COMMENT(Strings::Menus_Hacking_Abilities_Debuffs_CursedBlade_CommentDamageReduce::create()
							->setStringReplacementVariables(ConstantString::create(std::to_string(CursedBlade::DamageIncrease)))) + 
						"fld dword ptr [rdi]\n"
					),
				},
				true
			)
		},
	};

	auto func = &CursedBlade::applyCursedBlade;
	this->hackables = HackableCode::create((void*&)func, codeInfoMap);

	for (HackableCode* next : this->hackables)
	{
		this->owner->registerCode(next);
	}
}

void CursedBlade::onBeforeDamageDealt(CombatEvents::ModifiableDamageOrHealingArgs* damageOrHealing)
{
	super::onBeforeDamageDealt(damageOrHealing);

	Buff::HackStateStorage[Buff::StateKeyDamageDealt] = Value(damageOrHealing->damageOrHealingValue);

	this->applyCursedBlade();

	(*damageOrHealing->damageOrHealing) = Buff::HackStateStorage[Buff::StateKeyDamageDealt].asInt();
	(*damageOrHealing->damageOrHealingMin) = -std::abs(damageOrHealing->damageOrHealingValue * CursedBlade::MaxMultiplier);
	(*damageOrHealing->damageOrHealingMax) = std::abs(damageOrHealing->damageOrHealingValue * CursedBlade::MaxMultiplier);
}

NO_OPTIMIZE void CursedBlade::applyCursedBlade()
{
	static volatile int currentDamageDealtLocal = 0;
	static volatile int* currentDamageDealtLocalPtr = &currentDamageDealtLocal;
	static volatile float damageIncrease = 10.0f;
	static volatile float* damageIncreasePtr = &damageIncrease;

	currentDamageDealtLocal = Buff::HackStateStorage[Buff::StateKeyDamageDealt].asInt();

	ASM_PUSH_EFLAGS()
	ASM(push ZDI);
	ASM(push ZSI);

	ASM_MOV_REG_VAR(ZDI, damageIncreasePtr);
	ASM_MOV_REG_VAR(ZSI, currentDamageDealtLocalPtr);

	// f([i,u])com(p)(p) + fstsw ax + sahf
	// ftst (Compares St(0) to 0.0)
	HACKABLE_CODE_BEGIN(LOCAL_FUNC_ID_CURSED_BLADE);
	ASM(fld dword ptr [ZSI]);
	ASM(fadd dword ptr [ZDI]);
	ASM(fistp dword ptr [ZSI]);
	ASM_NOP16();
	HACKABLE_CODE_END();

	ASM(pop ZSI);
	ASM(pop ZDI);
	ASM_POP_EFLAGS()

	Buff::HackStateStorage[Buff::StateKeyDamageDealt] = Value(currentDamageDealtLocal);

	HACKABLES_STOP_SEARCH();
}
END_NO_OPTIMIZE
