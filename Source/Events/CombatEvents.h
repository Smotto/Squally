#pragma once
#include <functional>
#include <string>
#include <vector>

#include "Scenes/Platformer/Level/Combat/Attacks/AbilityType.h"

class Buff;
class PlatformerEntity;
class Projectile;
class Timeline;
class TimelineEntry;
class TimelineEventGroup;

class CombatEvents
{
public:
	static const std::string EventSpawnPrefix;
	static const std::string EventQueryTimeline;
	static const std::string EventGetAssociatedTimelineEntry;
	static const std::string EventMenuBack;
	static const std::string EventChangeMenuState;
	static const std::string EventDisableDefend;
	static const std::string EventDisableItems;
	static const std::string EventSelectionChanged;
	static const std::string EventSelectCastTarget;
	static const std::string EventRequestAIAction;
	static const std::string EventRequestRetargetCorrection;
	static const std::string EventRegisterTimelineEventGroup;
	static const std::string EventBuffApplied;
	static const std::string EventBuffRemoved;
	static const std::string EventBuffTimeElapsed;
	static const std::string EventProjectileSpawned;
	static const std::string EventEntityTimelineReset;
	static const std::string EventPauseTimeline;
	static const std::string EventPauseTimelineCinematic;
	static const std::string EventResumeTimeline;
	static const std::string EventResumeTimelineCinematic;
	static const std::string EventInterruptTimeline;
	static const std::string EventCastBlocked;
	static const std::string EventCastInterrupt;
	static const std::string EventCombatFinished;
	static const std::string EventCombatTutorialFocus;
	static const std::string EventCombatTutorialUnfocus;
	static const std::string EventGiveExp;
	static const std::string EventGiveRewards;
	static const std::string EventBeforeReturnToMap;
	static const std::string EventReturnToMap;
	static const std::string EventReturnToMapRespawn;
	static const std::string EventHackableCombatCue;
	static const std::string EventDamageDealt;
	static const std::string EventHealingDealt;
	static const std::string EventDamage;
	static const std::string EventHealing;
	static const std::string EventManaRestore;
	static const std::string EventManaRestoreDealt;
	static const std::string EventManaDrain;
	static const std::string EventManaDrainDealt;
	static const std::string EventModifyTimelineSpeed;
	static const std::string EventModifyDamageTaken;
	static const std::string EventModifyDamageDealt;
	static const std::string EventModifyHealingTaken;
	static const std::string EventModifyHealingDealt;
	static const std::string EventStatsModifyDamageTaken;
	static const std::string EventStatsModifyDamageDealt;
	static const std::string EventStatsModifyHealingTaken;
	static const std::string EventStatsModifyHealingDealt;
	static const std::string EventModifyDamageDealtComplete;
	static const std::string EventModifyDamageTakenComplete;
	static const std::string EventModifyHealingDealtComplete;
	static const std::string EventModifyHealingTakenComplete;

	struct SpawnArgs
	{
		PlatformerEntity* entity = nullptr;
		bool isEnemySpawn = false;
		int spawnIndex = 0;
		std::function<void()> onSpawnSuccess = nullptr;

		SpawnArgs(PlatformerEntity* entity, bool isEnemySpawn, int spawnIndex, std::function<void()> onSpawnSuccess) : entity(entity), isEnemySpawn(isEnemySpawn), spawnIndex(spawnIndex), onSpawnSuccess(onSpawnSuccess)
		{
		}
	};

	struct QueryTimelineArgs
	{
		std::function<void(Timeline*)> callback = nullptr;

		QueryTimelineArgs(std::function<void(Timeline*)> callback) : callback(callback)
		{
		}
	};

	struct AssociatedEntryArgs
	{
		PlatformerEntity* entity = nullptr;
		std::function<void(TimelineEntry*)> onLocated = nullptr;

		AssociatedEntryArgs(PlatformerEntity* entity, std::function<void(TimelineEntry*)> onLocated) : entity(entity), onLocated(onLocated)
		{
		}
	};

	struct MenuStateArgs
	{
		enum class CurrentMenu
		{
			Closed,
			ActionSelect,
			ItemSelect,
			AttackSelect,
			DefendSelect,
			ChooseAttackTarget,
			ChooseBuffTarget,
			ChooseResurrectionTarget,
			ChooseAnyTarget,
		};
		
		struct SelectionMeta
		{
			enum class Choice
			{
				Item,
				Attack,
				Defend,
			};

			Choice choice = Choice::Attack;
			std::string iconResource;

			SelectionMeta() : choice(Choice::Attack), iconResource("") { }
			SelectionMeta(Choice choice, std::string iconResource) : choice(choice), iconResource(iconResource) { }
		};

		CurrentMenu currentMenu = CurrentMenu::Closed;
		TimelineEntry* entry = nullptr;
		SelectionMeta selectionMeta;

		MenuStateArgs(CurrentMenu currentMenu, TimelineEntry* entry) : currentMenu(currentMenu), entry(entry), selectionMeta(SelectionMeta()) { }
		MenuStateArgs(CurrentMenu currentMenu, TimelineEntry* entry, SelectionMeta selectionMeta) : currentMenu(currentMenu), entry(entry), selectionMeta(selectionMeta) { }
	};

	struct SelectionArgs
	{
		PlatformerEntity* target = nullptr;

		SelectionArgs(PlatformerEntity* target) : target(target)
		{
		}
	};

	struct CastTargetsArgs
	{
		std::vector<PlatformerEntity*> targets;

		CastTargetsArgs(std::vector<PlatformerEntity*> targets) : targets(targets)
		{
		}
	};

	struct AIRequestArgs
	{
		TimelineEntry* attackingEntry = nullptr;

		AIRequestArgs(TimelineEntry* attackingEntry) : attackingEntry(attackingEntry)
		{
		}
	};

	struct CastBlockedArgs
	{
		PlatformerEntity* target = nullptr;

		CastBlockedArgs(PlatformerEntity* target) : target(target)
		{
		}
	};

	struct CastInterruptArgs
	{
		PlatformerEntity* target = nullptr;

		CastInterruptArgs(PlatformerEntity* target) : target(target)
		{
		}
	};

	struct BuffAppliedArgs
	{
		PlatformerEntity* target = nullptr;
		Buff* buff = nullptr;

		BuffAppliedArgs(PlatformerEntity* target, Buff* buff) : target(target), buff(buff)
		{
		}
	};

	struct BuffRemovedArgs
	{
		PlatformerEntity* target = nullptr;
		Buff* buff = nullptr;

		BuffRemovedArgs(PlatformerEntity* target, Buff* buff) : target(target), buff(buff)
		{
		}
	};

	struct BuffTimeElapsedArgs
	{
		float dt = 0.0f;

		BuffTimeElapsedArgs(float dt) : dt(dt)
		{
		}
	};

	struct ProjectileSpawnedArgs
	{
		PlatformerEntity* owner = nullptr;
		PlatformerEntity* target = nullptr;
		Projectile* projectile = nullptr;

		ProjectileSpawnedArgs(PlatformerEntity* owner, PlatformerEntity* target, Projectile* projectile) : owner(owner), target(target), projectile(projectile)
		{
		}
	};

	struct DamageOrHealingArgs
	{
		PlatformerEntity* caster = nullptr;
		PlatformerEntity* target = nullptr;
		int damageOrHealing = 0;
		AbilityType abilityType = AbilityType::Physical;

		// If true, this flag will prevent buffs from modifying the damage/healing
		bool disableBuffProcessing = false;

		bool overflowedMin = false;
		bool overflowedMax = false;

		DamageOrHealingArgs(
			PlatformerEntity* caster,
			PlatformerEntity* target,
			int damageOrHealing,
			AbilityType abilityType,
			bool disableBuffProcessing = false,
			bool overflowedMin = false,
			bool overflowedMax = false
		)
			:	caster(caster),
				target(target),
				damageOrHealing(damageOrHealing),
				abilityType(abilityType),
				disableBuffProcessing(disableBuffProcessing),
				overflowedMin(overflowedMin),
				overflowedMax(overflowedMax)
		{
		}
	};

	struct ManaRestoreOrDrainArgs
	{
		PlatformerEntity* caster = nullptr;
		PlatformerEntity* target = nullptr;
		int manaRestoreOrDrain = 0;
		AbilityType abilityType = AbilityType::Physical;

		// If true, this flag will prevent buffs from modifying the drain/restore
		bool disableBuffProcessing = false;
		
		bool overflowedMin = false;
		bool overflowedMax = false;

		ManaRestoreOrDrainArgs(
			PlatformerEntity* caster,
			PlatformerEntity* target,
			int manaRestoreOrDrain,
			AbilityType abilityType,
			bool disableBuffProcessing = false,
			bool overflowedMin = false,
			bool overflowedMax = false
		)
			:	caster(caster),
				target(target),
				manaRestoreOrDrain(manaRestoreOrDrain),
				abilityType(abilityType),
				disableBuffProcessing(disableBuffProcessing),
				overflowedMin(overflowedMin),
				overflowedMax(overflowedMax)
		{
		}
	};

	struct ModifiableDamageOrHealingArgs
	{
		PlatformerEntity* caster = nullptr;
		PlatformerEntity* target = nullptr;
		int* damageOrHealing = nullptr;
		int* damageOrHealingMin = nullptr;
		int* damageOrHealingMax = nullptr;
		int damageOrHealingValue = 0;
		int originalDamageOrHealingBeforeBuffs = 0;
		int originalDamageOrHealingBeforeBuffsAndStats = 0;
		AbilityType abilityType = AbilityType::Physical;

		ModifiableDamageOrHealingArgs(
			PlatformerEntity* caster,
			PlatformerEntity* target,
			int* damageOrHealing,
			int* damageOrHealingMin,
			int* damageOrHealingMax,
			int damageOrHealingValue,
			int originalDamageOrHealingBeforeBuffs,
			int originalDamageOrHealingBeforeBuffsAndStats,
			AbilityType abilityType
		)
			:	caster(caster),
				target(target),
				damageOrHealing(damageOrHealing),
				damageOrHealingMin(damageOrHealingMin),
				damageOrHealingMax(damageOrHealingMax),
				damageOrHealingValue(damageOrHealingValue),
				originalDamageOrHealingBeforeBuffs(originalDamageOrHealingBeforeBuffs),
				originalDamageOrHealingBeforeBuffsAndStats(originalDamageOrHealingBeforeBuffsAndStats),
				abilityType(abilityType),
				handled(false)
		{
		}

		void handle()
		{
			this->handled = true;
		}

		bool isHandled()
		{
			return this->handled;
		}

		private:
			bool handled = false;
	};
	
	struct ModifiableTimelineSpeedArgs
	{
		PlatformerEntity* target = nullptr;
		float* speed = nullptr;

		ModifiableTimelineSpeedArgs(PlatformerEntity* target, float* speed)
			: target(target), speed(speed), handled(false)
		{
		}

		void handle()
		{
			this->handled = true;
		}

		bool isHandled()
		{
			return this->handled;
		}

		private:
			bool handled = false;
	};

	struct CombatFinishedArgs
	{
		bool playerVictory = false;

		CombatFinishedArgs(bool playerVictory) : playerVictory(playerVictory)
		{
		}
	};

	struct TimelineResetArgs
	{
		PlatformerEntity* target = nullptr;
		bool wasInterrupt = false;

		TimelineResetArgs(PlatformerEntity* target, bool wasInterrupt) : target(target), wasInterrupt(wasInterrupt), handled(false) { }

		void handle()
		{
			this->handled = true;
		}

		bool isHandled()
		{
			return this->handled;
		}

		private:
			bool handled = false;
	};

	struct RegisterTimelineEventGroupArgs
	{
		TimelineEventGroup* eventGroup;

		RegisterTimelineEventGroupArgs() : eventGroup(nullptr) { }
		RegisterTimelineEventGroupArgs(TimelineEventGroup* eventGroup) : eventGroup(eventGroup) { }
	};

	struct BeforeReturnToMapArgs
	{
		bool defeat = false;

		BeforeReturnToMapArgs(bool defeat) : defeat(defeat) { }
	};
	
	static void TriggerSpawn(SpawnArgs args);
	static void TriggerQueryTimeline(QueryTimelineArgs args);
	static void TriggerGetAssociatedTimelineEntry(AssociatedEntryArgs args);
	static void TriggerMenuGoBack();
	static void TriggerMenuStateChange(MenuStateArgs args);
	static void TriggerDisableDefend();
	static void TriggerDisableItems();
	static void TriggerSelectionChanged(SelectionArgs args);
	static void TriggerSelectCastTarget(CastTargetsArgs args);
	static void TriggerRequestRetargetCorrection(AIRequestArgs args);
	static void TriggerRequestAIAction(AIRequestArgs args);
	static void TriggerPauseTimeline();
	static void TriggerPauseTimelineCinematic();
	static void TriggerResumeTimeline();
	static void TriggerResumeTimelineCinematic();
	static void TriggerInterruptTimeline();
	static void TriggerRegisterTimelineEventGroup(RegisterTimelineEventGroupArgs args);
	static void TriggerBuffApplied(BuffAppliedArgs args);
	static void TriggerBuffRemoved(BuffRemovedArgs args);
	static void TriggerBuffTimeElapsed(BuffTimeElapsedArgs args);
	static void TriggerProjectileSpawned(ProjectileSpawnedArgs args);
	static void TriggerEntityTimelineReset(TimelineResetArgs args);
	static void TriggerCastBlocked(CastBlockedArgs args);
	static void TriggerCastInterrupt(CastInterruptArgs args);
	static void TriggerCombatFinished(CombatFinishedArgs args);
	static void TriggerGiveExp();
	static void TriggerGiveRewards();
	static void TriggerBeforeReturnToMap(BeforeReturnToMapArgs args);
	static void TriggerReturnToMap();
	static void TriggerReturnToMapRespawn();
	static void TriggerHackableCombatCue();
	// Functionally, damage/healing are the same, but treat 0 differently. Damage will dislay -0, gain will display +0
	static void TriggerDamageDealt(CombatEvents::DamageOrHealingArgs args);
	static void TriggerHealingDealt(CombatEvents::DamageOrHealingArgs args);
	static void TriggerDamage(CombatEvents::DamageOrHealingArgs args);
	static void TriggerHealing(CombatEvents::DamageOrHealingArgs args);
	// Functionally, restore/drain are the same, but treat 0 differently. Drain will dislay -0, restore will display +0
	static void TriggerManaRestoreDealt(ManaRestoreOrDrainArgs args);
	static void TriggerManaRestore(ManaRestoreOrDrainArgs args);
	static void TriggerManaDrainDealt(ManaRestoreOrDrainArgs args);
	static void TriggerManaDrain(ManaRestoreOrDrainArgs args);
	static void TriggerModifyTimelineSpeed(ModifiableTimelineSpeedArgs args);
	static void TriggerModifyDamageTaken(ModifiableDamageOrHealingArgs args);
	static void TriggerModifyDamageDealt(ModifiableDamageOrHealingArgs args);
	static void TriggerModifyHealingTaken(ModifiableDamageOrHealingArgs args);
	static void TriggerModifyHealingDealt(ModifiableDamageOrHealingArgs args);
	static void TriggerStatsModifyDamageTaken(ModifiableDamageOrHealingArgs args);
	static void TriggerStatsModifyDamageDealt(ModifiableDamageOrHealingArgs args);
	static void TriggerStatsModifyHealingTaken(ModifiableDamageOrHealingArgs args);
	static void TriggerStatsModifyHealingDealt(ModifiableDamageOrHealingArgs args);
	static void TriggerModifyDamageDealtComplete(CombatEvents::DamageOrHealingArgs args);
	static void TriggerModifyDamageTakenComplete(CombatEvents::DamageOrHealingArgs args);
	static void TriggerModifyHealingDealtComplete(CombatEvents::DamageOrHealingArgs args);
	static void TriggerModifyHealingTakenComplete(CombatEvents::DamageOrHealingArgs args);
};
