#include "CastShadowBolt.h"

#include "cocos/2d/CCActionInterval.h"

#include "Engine/Animations/SmartAnimationSequenceNode.h"
#include "Engine/Events/ObjectEvents.h"
#include "Engine/Physics/CollisionObject.h"
#include "Engine/Sound/WorldSound.h"
#include "Engine/Utils/GameUtils.h"
#include "Entities/Platformer/PlatformerEntity.h"
#include "Events/CombatEvents.h"
#include "Objects/Platformer/Combat/Projectiles/ShadowBolt/ShadowBolt.h"
#include "Objects/Platformer/Combat/Projectiles/ThrownObject/ThrownObject.h"
#include "Scenes/Platformer/AttachedBehavior/Entities/Combat/EntityProjectileTargetBehavior.h"
#include "Scenes/Platformer/Level/Combat/Physics/CombatCollisionType.h"
#include "Scenes/Platformer/State/StateKeys.h"

#include "Resources/FXResources.h"
#include "Resources/SoundResources.h"
#include "Resources/UIResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

CastShadowBolt* CastShadowBolt::create(float attackDuration, float recoverDuration, Priority priority)
{
	CastShadowBolt* instance = new CastShadowBolt(attackDuration, recoverDuration, priority);

	instance->autorelease();

	return instance;
}

CastShadowBolt::CastShadowBolt(float attackDuration, float recoverDuration, Priority priority)
	: super(AttackType::Damage, UIResources::Menus_Icons_SpellImpactPurple, priority, AbilityType::Shadow, 4, 6, 9, attackDuration, recoverDuration)
{
}

CastShadowBolt::~CastShadowBolt()
{
}

PlatformerAttack* CastShadowBolt::cloneInternal()
{
	return CastShadowBolt::create(this->getAttackDuration(), this->getRecoverDuration(), this->priority);
}

LocalizedString* CastShadowBolt::getString()
{
	return Strings::Platformer_Combat_Attacks_Slash::create();
}

std::string CastShadowBolt::getAttackAnimation()
{
	return "AttackCast";
}

void CastShadowBolt::performAttack(PlatformerEntity* owner, std::vector<PlatformerEntity*> targets)
{
	super::performAttack(owner, targets);
	
	for (auto next : targets)
	{
		SmartAnimationSequenceNode* launchFx = SmartAnimationSequenceNode::create();
		ShadowBolt* shadowBolt = ShadowBolt::create(owner, next);

		shadowBolt->runSpawnFX();

		shadowBolt->whenCollidesWith({ (int)CombatCollisionType::EntityEnemy, (int)CombatCollisionType::EntityFriendly }, [=](CollisionData collisionData)
		{
			PlatformerEntity* entity = GameUtils::getFirstParentOfType<PlatformerEntity>(collisionData.other, true);
			
			if (!entity->getRuntimeStateOrDefault(StateKeys::IsAlive, Value(true)).asBool())
			{
				return CollisionResult::DoNothing;
			}

			shadowBolt->disable(true);
			shadowBolt->runImpactFX();

			if (entity != nullptr)
			{
				CombatEvents::TriggerDamage(CombatEvents::DamageOrHealingArgs(owner, entity, this->getRandomDamage(), this->abilityType));
			}

			return CollisionResult::DoNothing;
		});

		launchFx->playAnimation(FXResources::ShadowRing_ShadowRing_0000, 0.05f, true);
		launchFx->setFlippedX(owner->isFlippedX());

		ObjectEvents::TriggerObjectSpawn(RequestObjectSpawnArgs(
			owner,
			shadowBolt,
			SpawnMethod::Above,
			PositionMode::Discard,
			[&]()
			{
			},
			[&]()
			{
			}
		));
		ObjectEvents::TriggerObjectSpawn(RequestObjectSpawnArgs(
			owner,
			launchFx,
			SpawnMethod::Above,
			PositionMode::Discard,
			[&]()
			{
			},
			[&]()
			{
			}
		));
		
		shadowBolt->setPosition3D(GameUtils::getWorldCoords3D(owner) + Vec3((owner->isFlippedX() ? -96.0f : 96.0f), 96.0f, 0.0f));
		launchFx->setPosition3D(GameUtils::getWorldCoords3D(shadowBolt) + Vec3((owner->isFlippedX() ? -180.0f : 180.0f), 0.0f, 0.0f));

		next->getAttachedBehavior<EntityProjectileTargetBehavior>([=](EntityProjectileTargetBehavior* behavior)
		{
			shadowBolt->launchTowardsTarget3D(behavior->getTarget(), Vec2::ZERO, 0.0f, Vec3(0.3f, 0.3f, 0.3f), Vec3(0.0f, -64.0f, 0.0f));
		});
	}
}

void CastShadowBolt::onCleanup()
{
}
