#include "HealthLink.h"

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
#include "Scenes/Platformer/Components/Entities/Combat/EntityBuffBehavior.h"
#include "Scenes/Platformer/Hackables/HackFlags.h"
#include "Scenes/Platformer/Level/Combat/Attacks/Buffs/HealthLink/HealthLinkGenericPreview.h"
#include "Scenes/Platformer/Level/Combat/CombatMap.h"
#include "Scenes/Platformer/Level/Combat/Timeline.h"
#include "Scenes/Platformer/Level/Combat/TimelineEvent.h"
#include "Scenes/Platformer/Level/Combat/TimelineEventGroup.h"
#include "Scenes/Platformer/State/StateKeys.h"

#include "Resources/FXResources.h"
#include "Resources/ObjectResources.h"
#include "Resources/ParticleResources.h"
#include "Resources/SoundResources.h"
#include "Resources/UIResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

#define LOCAL_FUNC_ID_HEALTH_LINK 1

const std::string HealthLink::HealthLinkIdentifier = "health-link";
const float HealthLink::Duration = -1.0f;

// Static to prevent GCC optimization issues
volatile int HealthLink::healthLinkDamage = 0;

HealthLink* HealthLink::create(PlatformerEntity* caster, PlatformerEntity* target)
{
	HealthLink* instance = new HealthLink(caster, target);

	instance->autorelease();

	return instance;
}

HealthLink::HealthLink(PlatformerEntity* caster, PlatformerEntity* target)
	: super(caster, target, UIResources::Menus_Icons_Clones, AbilityType::Physical, BuffData(HealthLink::Duration, HealthLink::HealthLinkIdentifier))
{
	this->spellEffect = SmartParticles::create(ParticleResources::Platformer_Combat_Abilities_Speed);
	this->healthLinkDamage = 0;

	this->addChild(this->spellEffect);
}

HealthLink::~HealthLink()
{
}

void HealthLink::onEnter()
{
	super::onEnter();

	this->spellEffect->start();
}

void HealthLink::initializePositions()
{
	super::initializePositions();
}

void HealthLink::registerHackables()
{
	super::registerHackables();

	if (this->owner == nullptr)
	{
		return;
	}

	HackableCode::CodeInfoMap codeInfoMap =
	{
		{
			LOCAL_FUNC_ID_HEALTH_LINK,
			HackableCode::HackableCodeInfo(
				HealthLink::HealthLinkIdentifier,
				Strings::Menus_Hacking_Abilities_Buffs_HealthLink_HealthLink::create(),
				HackableBase::HackBarColor::Blue,
				UIResources::Menus_Icons_Clones,
				LazyNode<HackablePreview>::create([=](){ return HealthLinkGenericPreview::create(); }),
				{
					{
						HackableCode::Register::zdi, Strings::Menus_Hacking_Abilities_Buffs_HealthLink_RegisterEdi::create(),
					},
				},
				int(HackFlags::None),
				this->getRemainingDuration(),
				0.0f,
				{
					HackableCode::ReadOnlyScript(
						Strings::Menus_Hacking_CodeEditor_OriginalCode::create(),
						// x86
						COMMENT(Strings::Menus_Hacking_Abilities_Buffs_HealthLink_CommentShr::create()) +
						COMMENT(Strings::Menus_Hacking_Abilities_Buffs_HealthLink_CommentShrBy1::create()) +
						COMMENT(Strings::Menus_Hacking_Abilities_Buffs_HealthLink_CommentElaborate::create()) +
						"shr edi, 1\n\n" +
						COMMENT(Strings::Menus_Hacking_Abilities_Buffs_HealthLink_CommentHint::create())
						
						, // x64
						COMMENT(Strings::Menus_Hacking_Abilities_Buffs_HealthLink_CommentShr::create()) +
						COMMENT(Strings::Menus_Hacking_Abilities_Buffs_HealthLink_CommentShrBy1::create()) +
						COMMENT(Strings::Menus_Hacking_Abilities_Buffs_HealthLink_CommentElaborate::create()) +
						"shr rdi, 1\n\n" +
						COMMENT(Strings::Menus_Hacking_Abilities_Buffs_HealthLink_CommentHint::create())
					),
				},
				true
			)
		},
	};

	this->hackables = CREATE_HACKABLES(HealthLink::applyHealthLink, codeInfoMap);

	for (HackableCode* next : this->hackables)
	{
		this->owner->registerCode(next);
	}
}

NO_OPTIMIZE void HealthLink::onBeforeDamageTaken(CombatEvents::ModifiableDamageOrHealingArgs* damageOrHealing)
{
	super::onBeforeDamageTaken(damageOrHealing);

	this->healthLinkDamage = damageOrHealing->damageOrHealingValue;

	this->applyHealthLink();

	// Damage all team mates
	CombatEvents::TriggerQueryTimeline(CombatEvents::QueryTimelineArgs([&](Timeline* timeline)
	{
		std::vector<PlatformerEntity*> targets = timeline->getSameTeamEntities(damageOrHealing->target);
		std::vector<PlatformerEntity*> healthLinkTargets; 
		for (PlatformerEntity* target : targets)
		{
			if (target->getRuntimeStateOrDefaultBool(StateKeys::IsAlive, true))
			{
				caster->getComponent<EntityBuffBehavior>([&](EntityBuffBehavior* entityBuffBehavior)
				{
					entityBuffBehavior->getBuff<HealthLink>([&](HealthLink* healthLink)
					{
						healthLinkTargets.push_back(target);
					});
				});
			}
		}

		// For some incomprehensible reason, this needs to be a separate loop otherwise some enemies do not get it at all.
		// TODO: Figure out why, if you care enough.
		for (PlatformerEntity* target : healthLinkTargets)
		{
			CombatEvents::TriggerDamage(CombatEvents::DamageOrHealingArgs(damageOrHealing->caster, target, std::abs(this->healthLinkDamage), AbilityType::Passive, true));
		}
	}));
}
END_NO_OPTIMIZE

NO_OPTIMIZE void HealthLink::applyHealthLink()
{
	static volatile int healthLinkDamageLocal = 0;

	healthLinkDamageLocal = this->healthLinkDamage;

	ASM_PUSH_EFLAGS()
	ASM(push ZDI);
	ASM_MOV_REG_VAR(edi, healthLinkDamageLocal);

	HACKABLE_CODE_BEGIN(LOCAL_FUNC_ID_HEALTH_LINK);
	ASM(shr ZDI, 1);
	ASM_NOP16();
	HACKABLE_CODE_END();

	ASM_MOV_VAR_REG(healthLinkDamageLocal, edi);

	ASM(pop ZDI);
	ASM_POP_EFLAGS()

	this->healthLinkDamage = healthLinkDamageLocal;

	HACKABLES_STOP_SEARCH();
}
END_NO_OPTIMIZE
