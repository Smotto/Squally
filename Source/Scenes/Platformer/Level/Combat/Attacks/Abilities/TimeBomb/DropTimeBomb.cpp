#include "DropTimeBomb.h"

#include "cocos/2d/CCActionInterval.h"

#include "Engine/Animations/SmartAnimationSequenceNode.h"
#include "Engine/Events/ObjectEvents.h"
#include "Engine/Physics/CollisionObject.h"
#include "Engine/Sound/WorldSound.h"
#include "Engine/Utils/CombatUtils.h"
#include "Engine/Utils/GameUtils.h"
#include "Entities/Platformer/PlatformerEntity.h"
#include "Events/CombatEvents.h"
#include "Objects/Platformer/Combat/Projectiles/TimeBomb/TimeBomb.h"
#include "Objects/Platformer/Combat/Projectiles/ThrownObject/ThrownObject.h"
#include "Scenes/Platformer/Components/Entities/Combat/EntityBuffBehavior.h"
#include "Scenes/Platformer/Components/Entities/Combat/EntityProjectileTargetBehavior.h"
#include "Scenes/Platformer/Level/Combat/Attacks/Buffs/TimeBomb/Bombed.h"
#include "Scenes/Platformer/Level/Combat/Physics/CombatCollisionType.h"

#include "Resources/FXResources.h"
#include "Resources/SoundResources.h"
#include "Resources/UIResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

DropTimeBomb* DropTimeBomb::create(float attackDuration, float recoverDuration, Priority priority)
{
	DropTimeBomb* instance = new DropTimeBomb(attackDuration, recoverDuration, priority);

	instance->autorelease();

	return instance;
}

DropTimeBomb::DropTimeBomb(float attackDuration, float recoverDuration, Priority priority)
	: super(AttackType::Damage, UIResources::Menus_Icons_Bomb, priority, AbilityType::Fire, 450, 500, 12, attackDuration, recoverDuration)
{
}

DropTimeBomb::~DropTimeBomb()
{
}

PlatformerAttack* DropTimeBomb::cloneInternal()
{
	return DropTimeBomb::create(this->getAttackDuration(), this->getRecoverDuration(), this->priority);
}

LocalizedString* DropTimeBomb::getString()
{
	return Strings::Platformer_Combat_Attacks_DropTimeBomb::create();
}

std::string DropTimeBomb::getAttackAnimation()
{
	return "AttackDropTimeBomb";
}

void DropTimeBomb::performAttack(PlatformerEntity* owner, std::vector<PlatformerEntity*> targets)
{
	super::performAttack(owner, targets);

	for (PlatformerEntity* next : targets)
	{
		TimeBomb* timeBomb = TimeBomb::create(owner, next, [=]()
		{
			CombatEvents::TriggerDamage(CombatEvents::DamageOrHealingArgs(owner, next, this->getRandomDamage(), this->abilityType));
		});

		ObjectEvents::TriggerObjectSpawn(RequestObjectSpawnArgs(
			owner,
			timeBomb,
			SpawnMethod::Above,
			PositionMode::Discard,
			[&]()
			{
			},
			[&]()
			{
			}
		));

		const float BombHeightHalf = 133.0f / 2.0f;
		const float ZDrawDepthOffset = 1.0f;
		const float sign = RandomHelper::random_real(0.0f, 1.0f) <= 0.5f ? -1.0f : 1.0f;

		Vec2 offset = next->getEntityBottomPoint() + Vec2(sign * RandomHelper::random_real(64.0f, 96.0f), BombHeightHalf);

		timeBomb->runSpawnFX();
		timeBomb->setPosition3D(GameUtils::getWorldCoords3D(next) + Vec3(offset.x, offset.y, ZDrawDepthOffset));
		
		next->getComponent<EntityBuffBehavior>([&](EntityBuffBehavior* entityBuffBehavior)
		{
			entityBuffBehavior->applyBuff(Bombed::create(owner, next));
		});
	}
}

bool DropTimeBomb::isWorthUsing(PlatformerEntity* caster, const std::vector<PlatformerEntity*>& sameTeam, const std::vector<PlatformerEntity*>& otherTeam)
{
	int buffCount = 0;

	for (auto next : otherTeam)
	{
		if (CombatUtils::HasDuplicateCastOnLivingTarget(caster, next, [](PlatformerAttack* next) { return dynamic_cast<DropTimeBomb*>(next) != nullptr;  }))
		{
			// Consider a cast-in-progress as a buff
			buffCount++;
			continue;
		}

		next->getComponent<EntityBuffBehavior>([&](EntityBuffBehavior* entityBuffBehavior)
		{
			entityBuffBehavior->getBuff<TimeBomb>([&](TimeBomb* bombed)
			{
				buffCount++;
			});
		});
	}

	return buffCount != otherTeam.size();
}

float DropTimeBomb::getUseUtility(PlatformerEntity* caster, PlatformerEntity* target, const std::vector<PlatformerEntity*>& sameTeam, const std::vector<PlatformerEntity*>& otherTeam)
{
	float utility = CombatUtils::HasDuplicateCastOnLivingTarget(caster, target, [](PlatformerAttack* next) { return dynamic_cast<DropTimeBomb*>(next) != nullptr;  }) ? 0.0f : 1.0f;

	target->getComponent<EntityBuffBehavior>([&](EntityBuffBehavior* entityBuffBehavior)
	{
		entityBuffBehavior->getBuff<Bombed>([&](Bombed* bombed)
		{
			utility = 0.0f;
		});
	});

	return utility;
}

void DropTimeBomb::onCleanup()
{
}
