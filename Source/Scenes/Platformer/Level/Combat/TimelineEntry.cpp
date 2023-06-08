#include "TimelineEntry.h"

#include "cocos/2d/CCActionInstant.h"
#include "cocos/2d/CCActionInterval.h"
#include "cocos/2d/CCSprite.h"
#include "cocos/base/CCDirector.h"
#include "cocos/base/CCEventCustom.h"
#include "cocos/base/CCEventListenerCustom.h"

#include "Engine/Animations/SmartAnimationNode.h"
#include "Engine/Camera/GameCamera.h"
#include "Engine/Events/ObjectEvents.h"
#include "Engine/Utils/MathUtils.h"
#include "Entities/Platformer/PlatformerEnemy.h"
#include "Entities/Platformer/PlatformerEntity.h"
#include "Events/CombatEvents.h"
#include "Objects/Camera/CameraFocus.h"
#include "Objects/Platformer/Camera/CameraTarget.h"
#include "Scenes/Platformer/Components/Entities/Stats/EntityHealthBehavior.h"
#include "Scenes/Platformer/Components/Entities/Stats/EntityManaBehavior.h"
#include "Scenes/Platformer/Level/Combat/Attacks/PlatformerAttack.h"
#include "Scenes/Platformer/Level/Combat/Buffs/Buff.h"
#include "Scenes/Platformer/Level/Combat/Buffs/Defend/Defend.h"
#include "Scenes/Platformer/Level/Combat/Buffs/Discipline/Discipline.h"
#include "Scenes/Platformer/State/StateKeys.h"
#include "Scenes/Platformer/Components/Entities/Combat/EntityBuffBehavior.h"
#include "Scenes/Platformer/Components/Entities/Combat/EntityCombatBehaviorBase.h"
#include "Scenes/Platformer/Level/Combat/Attacks/Buffs/Pacifist/Pacifist.h"

#include "Resources/UIResources.h"

using namespace cocos2d;

const float TimelineEntry::CastPercentage = 0.75f;
const float TimelineEntry::BaseSpeedMultiplier = 0.175f;
const int TimelineEntry::DefaultMinDamage = -255;
const int TimelineEntry::DefaultMaxDamage = 255;
const int TimelineEntry::DefaultMinHealing = -255;
const int TimelineEntry::DefaultMaxHealing = 255;

TimelineEntry* TimelineEntry::create(PlatformerEntity* entity, int spawnIndex)
{
	TimelineEntry* instance = new TimelineEntry(entity, spawnIndex);

	instance->autorelease();

	return instance;
}

TimelineEntry::TimelineEntry(PlatformerEntity* entity, int spawnIndex) : super()
{
	this->entity = entity;
	this->line = Sprite::create(UIResources::Combat_Line);
	this->circle = this->isPlayerEntry() ? Sprite::create(UIResources::Combat_PlayerCircle) : Sprite::create(UIResources::Combat_EnemyCircle);
	this->circleSelected = this->isPlayerEntry() ? Sprite::create(UIResources::Combat_PlayerCircleSelected) : Sprite::create(UIResources::Combat_EnemyCircleSelected);
	this->emblem = Sprite::create(entity == nullptr ? UIResources::EmptyImage : entity->getEmblemResource());
	this->overlayCircle = Sprite::create(UIResources::Combat_OverlayCircle);
	this->skull = Sprite::create(UIResources::Combat_Skull);
	this->orphanedAttackCache = Node::create();
	this->spawnIndex = spawnIndex;
	
	for (int index = 0; index < 5; index++)
	{
		Sprite* target = Sprite::create(UIResources::Menus_Icons_CrossHair);

		target->setVisible(false);

		this->targetIcons.push_back(target);
	}
	
	this->circleSelected->setVisible(false);

	this->addChild(this->line);
	this->addChild(this->circle);
	this->addChild(this->circleSelected);
	this->addChild(this->emblem);
	this->addChild(this->overlayCircle);
	this->addChild(this->skull);

	for (auto next : this->targetIcons)
	{
		this->addChild(next);
	}

	this->addChild(this->orphanedAttackCache);
}

TimelineEntry::~TimelineEntry()
{
}

void TimelineEntry::onEnter()
{
	super::onEnter();

	this->currentCast = nullptr;
	this->targets = std::vector<PlatformerEntity*>();
	this->targetsAsEntries = std::vector<TimelineEntry*>();
	this->isCasting = false;
	this->orphanedAttackCache->removeAllChildren();
	this->skull->setVisible(false);

	if (this->entity != nullptr)
	{
		this->entity->watchForComponent<EntityCombatBehaviorBase>([=](EntityCombatBehaviorBase* combatBehavior)
		{
			this->combatBehavior = combatBehavior;
		});
	}

	this->scheduleUpdate();
}

void TimelineEntry::initializePositions()
{
	super::initializePositions();

	const float EntryOffsetY = 72.0f;
	const float LineOffsetY = 48.0f;

	if (this->isPlayerEntry())
	{
		this->line->setPositionY(-LineOffsetY);
		this->setPositionY(EntryOffsetY);
	}
	else
	{
		this->line->setPositionY(LineOffsetY);
		this->setPositionY(-EntryOffsetY);
	}
}

void TimelineEntry::initializeListeners()
{
	super::initializeListeners();

	this->addEventListenerIgnorePause(EventListenerCustom::create(CombatEvents::EventCastBlocked, [=](EventCustom* eventCustom)
	{
		CombatEvents::CastBlockedArgs* args = static_cast<CombatEvents::CastBlockedArgs*>(eventCustom->getData());

		if (args != nullptr && args->target == this->entity)
		{
			this->isBlocking = true;
		}
	}));

	this->addEventListenerIgnorePause(EventListenerCustom::create(CombatEvents::EventDamage, [=](EventCustom* eventCustom)
	{
		CombatEvents::DamageOrHealingArgs* args = static_cast<CombatEvents::DamageOrHealingArgs*>(eventCustom->getData());

		if (args != nullptr && args->target != nullptr && args->target == this->getEntity())
		{
			if (this->getEntity()->getRuntimeStateOrDefault(StateKeys::IsAlive, Value(true)).asBool())
			{
				this->applyDamage(args);
			}
		}
	}));

	this->addEventListenerIgnorePause(EventListenerCustom::create(CombatEvents::EventHealing, [=](EventCustom* eventCustom)
	{
		CombatEvents::DamageOrHealingArgs* args = static_cast<CombatEvents::DamageOrHealingArgs*>(eventCustom->getData());

		if (args != nullptr && args->target != nullptr && args->target == this->getEntity())
		{
			if (this->getEntity()->getRuntimeStateOrDefault(StateKeys::IsAlive, Value(true)).asBool())
			{
				this->applyHealing(args);
			}
		}
	}));

	this->addEventListenerIgnorePause(EventListenerCustom::create(CombatEvents::EventManaRestore, [=](EventCustom* eventCustom)
	{
		CombatEvents::ManaRestoreOrDrainArgs* args = static_cast<CombatEvents::ManaRestoreOrDrainArgs*>(eventCustom->getData());

		if (args != nullptr && args->target != nullptr && args->target == this->getEntity())
		{
			if (this->getEntity()->getRuntimeStateOrDefault(StateKeys::IsAlive, Value(true)).asBool())
			{
				this->applyManaRestore(args);
			}
		}
	}));

	this->addEventListenerIgnorePause(EventListenerCustom::create(CombatEvents::EventManaDrain, [=](EventCustom* eventCustom)
	{
		CombatEvents::ManaRestoreOrDrainArgs* args = static_cast<CombatEvents::ManaRestoreOrDrainArgs*>(eventCustom->getData());

		if (args != nullptr && args->target != nullptr && args->target == this->getEntity())
		{
			if (this->getEntity()->getRuntimeStateOrDefault(StateKeys::IsAlive, Value(true)).asBool())
			{
				this->applyManaDrain(args);
			}
		}
	}));

	this->addEventListenerIgnorePause(EventListenerCustom::create(CombatEvents::EventGetAssociatedTimelineEntry, [=](EventCustom* eventCustom)
	{
		CombatEvents::AssociatedEntryArgs* args = static_cast<CombatEvents::AssociatedEntryArgs*>(eventCustom->getData());

		if (args != nullptr && args->entity == this->getEntity())
		{
			args->onLocated(this);
		}
	}));
}

void TimelineEntry::update(float dt)
{
	super::update(dt);

	if (this->getEntity() == nullptr)
	{
		return;
	}

	if (this->getEntity()->getRuntimeStateOrDefault(StateKeys::IsAlive, Value(true)).asBool())
	{
		this->emblem->setVisible(true);
		this->skull->setVisible(false);
	}
	else
	{
		this->emblem->setVisible(false);
		this->skull->setVisible(true);
	}
}

PlatformerEntity* TimelineEntry::getEntity()
{
	return this->entity;
}

void TimelineEntry::applyDamage(CombatEvents::DamageOrHealingArgs* args)
{
	PlatformerEntity* target = this->getEntity();

	if (args == nullptr || target == nullptr)
	{
		return;
	}

	PlatformerEntity* caster = args->caster;
	int damage = args->damageOrHealing;
	AbilityType abilityType = args->abilityType;
	int minDamage = TimelineEntry::DefaultMinDamage;
	int maxDamage = TimelineEntry::DefaultMaxDamage;

	if (!args->disableBuffProcessing)
	{
		int originalDamageBeforeBuffsAndStats = args->damageOrHealing;
		int originalDamageBeforeBuffs = originalDamageBeforeBuffsAndStats;

		// Apply stats
		CombatEvents::TriggerStatsModifyDamageDealt(CombatEvents::ModifiableDamageOrHealingArgs(caster, target, &damage, &minDamage, &maxDamage, damage, originalDamageBeforeBuffs, originalDamageBeforeBuffsAndStats, abilityType));
		CombatEvents::TriggerStatsModifyDamageTaken(CombatEvents::ModifiableDamageOrHealingArgs(caster, target, &damage, &minDamage, &maxDamage, damage, originalDamageBeforeBuffs, originalDamageBeforeBuffsAndStats, abilityType));

		originalDamageBeforeBuffs = damage;

		CombatEvents::ModifiableDamageOrHealingArgs args = CombatEvents::ModifiableDamageOrHealingArgs(caster, target, &damage, &minDamage, &maxDamage, damage, originalDamageBeforeBuffs, originalDamageBeforeBuffsAndStats, abilityType);

		/******************
		Modify outgoing damage
		*******************/

		// Fire event to let any arena objects process damage (ie FogOfWar)
		CombatEvents::TriggerModifyDamageDealt(args);

		// Iterate buffs sequentially, as they are ordered by priority
		caster->getComponent<EntityBuffBehavior>([&](EntityBuffBehavior* entityBuffBehavior)
		{
			for (auto next : entityBuffBehavior->getBuffs())
			{
				if (args.caster == next->owner)
				{
					args.damageOrHealingValue = damage;
					next->onBeforeDamageDealt(&args);
				}
			}

			CombatEvents::DamageOrHealingArgs postArgs = CombatEvents::DamageOrHealingArgs(caster, target, damage, abilityType);

			for (auto next : entityBuffBehavior->getBuffs())
			{
				if (args.caster == next->owner)
				{
					args.damageOrHealingValue = damage;
					next->onAfterDamageDealt(&postArgs);
				}
			}
		});

		// Fire finish event for arena objects
		CombatEvents::TriggerModifyDamageDealtComplete(CombatEvents::DamageOrHealingArgs(caster, target, damage, abilityType));
		
		/******************
		Modify incoming damage
		*******************/

		// Fire event to let any arena objects process damage
		CombatEvents::TriggerModifyDamageTaken(args);

		// Iterate buffs sequentially, as they are ordered by priority
		target->getComponent<EntityBuffBehavior>([&](EntityBuffBehavior* entityBuffBehavior)
		{
			for (auto next : entityBuffBehavior->getBuffs())
			{
				if (args.target == next->owner)
				{
					args.damageOrHealingValue = damage;
					next->onBeforeDamageTaken(&args);
				}
			}

			CombatEvents::DamageOrHealingArgs postArgs = CombatEvents::DamageOrHealingArgs(caster, target, damage, abilityType);

			for (auto next : entityBuffBehavior->getBuffs())
			{
				if (args.target == next->owner)
				{
					args.damageOrHealingValue = damage;
					next->onAfterDamageTaken(&postArgs);
				}
			}
		});
		
		// Fire finish event for arena objects
		CombatEvents::TriggerModifyDamageTakenComplete(CombatEvents::DamageOrHealingArgs(caster, target, damage, abilityType));
	}

	if (abilityType != AbilityType::Passive && damage > 0)
	{
		this->tryInterrupt();
	}

	int health = target->getRuntimeStateOrDefaultInt(StateKeys::Health, 0);

	// Clamp to the global min/max damage/healing
	bool isOverflowedMin = args->overflowedMin;
	bool isOverflowedMax = args->overflowedMax;

	if (!isOverflowedMin && !isOverflowedMax)
	{
		isOverflowedMin = damage <= minDamage;
		isOverflowedMin = damage >= maxDamage;
	}

	int clampedDamage = MathUtils::clamp(damage, minDamage, maxDamage);

	target->getComponent<EntityHealthBehavior>([=](EntityHealthBehavior* healthBehavior)
	{
		healthBehavior->setHealth(health - clampedDamage);
	});

	CombatEvents::TriggerDamageDealt(CombatEvents::DamageOrHealingArgs(caster, target, clampedDamage, abilityType, false, isOverflowedMin, isOverflowedMax));
}

void TimelineEntry::applyHealing(CombatEvents::DamageOrHealingArgs* args)
{
	PlatformerEntity* target = this->getEntity();

	if (target == nullptr)
	{
		return;
	}

	PlatformerEntity* caster = args->caster;
	int healing = args->damageOrHealing;
	AbilityType abilityType = args->abilityType;
	int minHealing = TimelineEntry::DefaultMinHealing;
	int maxHealing = TimelineEntry::DefaultMaxHealing;

	if (!args->disableBuffProcessing)
	{
		int originalHealingBeforeBuffsAndStats = healing;
		int originalHealingBeforeBuffs = originalHealingBeforeBuffsAndStats;

		// Apply stats
		CombatEvents::TriggerStatsModifyHealingDealt(CombatEvents::ModifiableDamageOrHealingArgs(caster, target, &healing, &minHealing, &maxHealing, healing, originalHealingBeforeBuffs, originalHealingBeforeBuffsAndStats, abilityType));
		CombatEvents::TriggerStatsModifyHealingTaken(CombatEvents::ModifiableDamageOrHealingArgs(caster, target, &healing, &minHealing, &maxHealing, healing, originalHealingBeforeBuffs, originalHealingBeforeBuffsAndStats, abilityType));
		
		CombatEvents::ModifiableDamageOrHealingArgs args = CombatEvents::ModifiableDamageOrHealingArgs(caster, target, &healing, &minHealing, &maxHealing, healing, originalHealingBeforeBuffs, originalHealingBeforeBuffsAndStats, abilityType);

		/******************
		Modify outgoing healing
		*******************/

		// Fire event to let any arena objects process damage
		CombatEvents::TriggerModifyHealingDealt(args);

		// Iterate buffs sequentially, as they are ordered by priority
		caster->getComponent<EntityBuffBehavior>([&](EntityBuffBehavior* entityBuffBehavior)
		{
			for (auto next : entityBuffBehavior->getBuffs())
			{
				if (args.caster == next->owner)
				{
					next->onBeforeHealingDealt(&args);
				}
			}

			CombatEvents::DamageOrHealingArgs postArgs = CombatEvents::DamageOrHealingArgs(caster, target, healing, abilityType);

			for (auto next : entityBuffBehavior->getBuffs())
			{
				if (args.caster == next->owner)
				{
					next->onAfterHealingDealt(&postArgs);
				}
			}
		});

		// Fire finish event for arena objects
		CombatEvents::TriggerModifyHealingDealtComplete(CombatEvents::DamageOrHealingArgs(caster, target, healing, abilityType));
		
		/******************
		Modify incoming healing
		*******************/

		// Fire event to let any arena objects process healing
		CombatEvents::TriggerModifyHealingTaken(args);

		// Iterate buffs sequentially, as they are ordered by priority
		target->getComponent<EntityBuffBehavior>([&](EntityBuffBehavior* entityBuffBehavior)
		{
			for (auto next : entityBuffBehavior->getBuffs())
			{
				if (args.target == next->owner)
				{
					next->onBeforeHealingTaken(&args);
				}
			}

			CombatEvents::DamageOrHealingArgs postArgs = CombatEvents::DamageOrHealingArgs(caster, target, healing, abilityType);

			for (auto next : entityBuffBehavior->getBuffs())
			{
				if (args.target == next->owner)
				{
					next->onAfterHealingTaken(&postArgs);
				}
			}
		});

		// Fire finish event for arena objects
		CombatEvents::TriggerModifyHealingTakenComplete(CombatEvents::DamageOrHealingArgs(caster, target, healing, abilityType));
	}

	int health = target->getRuntimeStateOrDefaultInt(StateKeys::Health, 0);

	// Clamp to the global min/max damage/healing
	bool isOverflowedMin = args->overflowedMin;
	bool isOverflowedMax = args->overflowedMax;

	if (!isOverflowedMin && !isOverflowedMax)
	{
		isOverflowedMin = healing <= minHealing;
		isOverflowedMin = healing >= maxHealing;
	}

	int clampedHealing = MathUtils::clamp(healing, minHealing, maxHealing);

	target->getComponent<EntityHealthBehavior>([=](EntityHealthBehavior* healthBehavior)
	{
		healthBehavior->setHealth(health + healing);
	});

	CombatEvents::TriggerHealingDealt(CombatEvents::DamageOrHealingArgs(caster, target, healing, abilityType, false, isOverflowedMin, isOverflowedMax));
}

void TimelineEntry::applyManaRestore(CombatEvents::ManaRestoreOrDrainArgs* args)
{
	int mana = this->getEntity()->getRuntimeStateOrDefaultInt(StateKeys::Mana, 0);

	this->getEntity()->getComponent<EntityManaBehavior>([=](EntityManaBehavior* manaBehavior)
	{
		manaBehavior->setMana(mana + args->manaRestoreOrDrain);
	});

	CombatEvents::TriggerManaRestoreDealt(*args);
}

void TimelineEntry::applyManaDrain(CombatEvents::ManaRestoreOrDrainArgs* args)
{
	int mana = this->getEntity()->getRuntimeStateOrDefaultInt(StateKeys::Mana, 0);

	this->getEntity()->getComponent<EntityManaBehavior>([=](EntityManaBehavior* manaBehavior)
	{
		manaBehavior->setMana(mana - args->manaRestoreOrDrain);
	});

	CombatEvents::TriggerManaDrainDealt(CombatEvents::ManaRestoreOrDrainArgs(*args));
}

void TimelineEntry::stageTargets(std::vector<PlatformerEntity*> targets)
{
	this->targets.clear();

	for (PlatformerEntity* next : targets)
	{
		if (next == nullptr)
		{
			continue;
		}

		CombatEvents::TriggerGetAssociatedTimelineEntry(CombatEvents::AssociatedEntryArgs(next, [=](TimelineEntry* timelineEntry)
		{
			this->targets.push_back(next);
			this->targetsAsEntries.push_back(timelineEntry);
		}));
	}
}

void TimelineEntry::stageCast(PlatformerAttack* attack)
{
	// If this attach was created by the result of an item, it has no parent, and thus we need to keep a reference to it
	if (attack != nullptr && attack->getParent() == nullptr)
	{
		this->orphanedAttackCache->addChild(attack);
	}

	if (attack != nullptr)
	{
		this->isCasting = true;
	}

	this->currentCast = attack;

	if (this->currentCast != nullptr)
	{
		attack->onAttackStaged();
	}
}

std::vector<PlatformerEntity*> TimelineEntry::getStagedTargets()
{
	return this->targets;
}

PlatformerAttack* TimelineEntry::getStagedCast()
{
	return this->currentCast;
}

void TimelineEntry::defend()
{
	if (this->getEntity() == nullptr)
	{
		return;
	}

	this->isCasting = true;
	
	this->getEntity()->getComponent<EntityBuffBehavior>([=](EntityBuffBehavior* entityBuffBehavior)
	{
		entityBuffBehavior->applyBuff(Defend::create(this->getEntity()));
	});
}

float TimelineEntry::getProgress()
{
	return this->progress;
}

void TimelineEntry::setProgress(float progress)
{
	this->progress = MathUtils::clamp(progress, 0.0f, 1.0f);
}

void TimelineEntry::addInitSpeed(float bonus)
{
	if (this->combatBehavior == nullptr)
	{
		return;
	}

	this->combatBehavior->setTimelineSpeedBase(this->combatBehavior->getTimelineSpeedBase() + bonus);
}

void TimelineEntry::addTimeWithoutActions(float dt)
{
	float speed = this->combatBehavior == nullptr ? 1.0f : this->combatBehavior->getTimelineSpeed();
	
	CombatEvents::TriggerModifyTimelineSpeed(CombatEvents::ModifiableTimelineSpeedArgs(this->getEntity(), &speed));

	this->setProgress(this->progress + (dt * (speed + this->interruptBonus) * TimelineEntry::BaseSpeedMultiplier));

	if (this->isCasting && speed < 0.0f && this->progress < TimelineEntry::CastPercentage)
	{
		this->stageCast(nullptr);
		this->isCasting = false;
	}
}

void TimelineEntry::addTime(float dt)
{
	this->addTimeWithoutActions(dt);
	this->tryPerformActions();
}

void TimelineEntry::tryPerformActions()
{
	// Cast started
	if (this->progress >= TimelineEntry::CastPercentage && !this->isCasting && !this->isPacifist())
	{
		CombatEvents::TriggerInterruptTimeline();

		if (this->isPlayerEntry())
		{
			CombatEvents::TriggerPauseTimeline();
			CombatEvents::TriggerMenuStateChange(CombatEvents::MenuStateArgs(CombatEvents::MenuStateArgs::CurrentMenu::ActionSelect, this));
		}
		else
		{
			CombatEvents::TriggerPauseTimeline();
			CombatEvents::TriggerRequestAIAction(CombatEvents::AIRequestArgs(this));
		}

		ObjectEvents::QueryObjects<CameraFocus>([&](CameraFocus* cameraTarget)
		{
			GameCamera::getInstance()->setTarget(cameraTarget->getTrackingData());
		}, CameraFocus::MapKey);
	}
	// Progress complete, do the cast
	else if (this->progress >= 1.0f)
	{
		if (this->isPacifist())
		{
			this->resetTimeline();
			return;
		}
		
		CombatEvents::TriggerInterruptTimeline();

		if (this->entity != nullptr && this->currentCast != nullptr && !this->targets.empty())
		{
			CombatEvents::TriggerPauseTimeline();

			this->performCast();
		}
		else
		{
			this->resetTimeline();
		}
	}
	else if (this->progress < TimelineEntry::CastPercentage)
	{
		// This is necessary if the entity drifts backwards due to a debuff (thus casting is canceled)
		this->isCasting = false;
	}
}

void TimelineEntry::performCast()
{
	CombatEvents::TriggerRequestRetargetCorrection(CombatEvents::AIRequestArgs(this));

	this->cameraFocusEntry();

	this->runAction(Sequence::create(
		DelayTime::create(1.0f),
		CallFunc::create([=]()
		{
			if (this->targets.empty() || this->entity == nullptr || this->currentCast == nullptr)
			{
				this->resetTimeline();
				CombatEvents::TriggerResumeTimeline();
				return;
			}

			this->entity->getAnimations()->clearAnimationPriority();
			this->entity->getAnimations()->playAnimation(this->currentCast->getAttackAnimation(), SmartAnimationNode::AnimationPlayMode::ReturnToIdle, SmartAnimationNode::AnimParams(1.0f, 0.5f, true));
			this->isCasting = false;

			this->currentCast->execute(
				this->entity,
				this->targets,
				[=]()
				{
					// Attack complete -- camera focus target
					if (!this->targetsAsEntries.empty())
					{
						this->targetsAsEntries[0]->cameraFocusEntry();
					}
				},
				[=]()
				{
					this->stageCast(nullptr);
					this->resetTimeline();
					CombatEvents::TriggerResumeTimeline();
				}
			);
		}),
		nullptr
	));
}

void TimelineEntry::cameraFocusEntry()
{
	std::string cameraTag = (this->isPlayerEntry() ? "player" : "enemy") + ("-camera-" + std::to_string(this->spawnIndex));

	ObjectEvents::QueryObjects<CameraTarget>([&](CameraTarget* cameraTarget)
	{
		GameCamera::getInstance()->setTarget(cameraTarget->getTrackingData());
	}, cameraTag);
}

void TimelineEntry::tryInterrupt()
{
	if (!this->isBlocking && !this->isCasting)
	{
		return;
	}
	
	if (this->isBlocking && this->isCasting)
	{
		this->setProgress(this->progress / 2.0f);
		this->interruptBonus = 0.1f;
	}
	else if (this->isBlocking && !this->isCasting)
	{
		// No interrupt when not casting. This is just a carry-over block.
		return;
	}
	else if (this->isCasting)
	{
		bool hasDiscipline = false;

		// Discipline is the only buff that allows ignoring interrupts, so we're just hacking in this one edge case
		this->entity->getComponent<EntityBuffBehavior>([&](EntityBuffBehavior* buffBehavior)
		{
			buffBehavior->getBuff<Discipline>([&](Buff*)
			{
				hasDiscipline = true;
			});
		});

		if (!hasDiscipline)
		{
			CombatEvents::TriggerCastInterrupt(CombatEvents::CastInterruptArgs(this->entity));

			this->setProgress(this->progress / 4.0f);
			this->interruptBonus = 0.1f;
		}
	}
	
	this->stageCast(nullptr);
	this->isCasting = false;
	this->isBlocking = false;

	CombatEvents::TriggerEntityTimelineReset(CombatEvents::TimelineResetArgs(this->getEntity(), true));
}

void TimelineEntry::resetTimeline()
{
	this->progress = std::fmod(this->progress, 1.0f);
	this->interruptBonus = 0.0f;
	this->isCasting = false;
	this->isBlocking = false;

	CombatEvents::TriggerEntityTimelineReset(CombatEvents::TimelineResetArgs(this->getEntity(), false));
}

bool TimelineEntry::isPlayerEntry()
{
	return (dynamic_cast<PlatformerEnemy*>(this->entity) == nullptr);
}

void TimelineEntry::setSelected(bool isSelected)
{
	this->circle->setVisible(!isSelected);
	this->circleSelected->setVisible(isSelected);
}

void TimelineEntry::clearBuffTargets()
{
	this->overlayCircle->setOpacity(0);

	for (Sprite* next : this->targetIcons)
	{
		next->setVisible(false);
	}
}

void TimelineEntry::addBuffTarget(std::string iconResource)
{
	this->overlayCircle->setOpacity(128);

	for (Sprite* next : this->targetIcons)
	{
		if (!next->isVisible())
		{
			next->initWithFile(iconResource);
			next->setVisible(true);
			return;
		}
	}
}

const std::string TimelineEntry::getEmblemResource()
{
	return entity == nullptr ? UIResources::EmptyImage : entity->getEmblemResource();
}

bool TimelineEntry::isPacifist()
{
	if (this->getEntity() != nullptr)
	{
		return this->getEntity()->getRuntimeStateOrDefaultBool(StateKeys::IsPacifist, false);
	}

	return false;
}
