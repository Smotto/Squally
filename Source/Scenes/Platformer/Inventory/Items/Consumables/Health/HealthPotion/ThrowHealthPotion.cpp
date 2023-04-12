#include "ThrowHealthPotion.h"

#include "cocos/2d/CCActionInterval.h"

#include "Engine/Localization/ConstantString.h"
#include "Engine/Physics/CollisionObject.h"
#include "Engine/Utils/GameUtils.h"
#include "Engine/Sound/WorldSound.h"
#include "Entities/Platformer/PlatformerEntity.h"
#include "Events/CombatEvents.h"
#include "Objects/Platformer/Combat/Projectiles/ThrownObject/ThrownObject.h"
#include "Scenes/Platformer/Components/Entities/Combat/EntityProjectileTargetBehavior.h"
#include "Scenes/Platformer/Inventory/Items/Consumables/Health/HealthPotion/HealthPotion.h"
#include "Scenes/Platformer/Level/Combat/Physics/CombatCollisionType.h"
#include "Scenes/Platformer/State/StateKeys.h"

#include "Resources/ItemResources.h"
#include "Resources/SoundResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

ThrowHealthPotion* ThrowHealthPotion::create(Priority priority)
{
	ThrowHealthPotion* instance = new ThrowHealthPotion(priority);

	instance->autorelease();

	return instance;
}

ThrowHealthPotion::ThrowHealthPotion(Priority priority)
	: super(AttackType::Healing, ItemResources::Consumables_Potions_HealthPotion, priority, AbilityType::Arcane, 10, 15, 0, 0.2f, 1.5f)
{
	this->throwSound = WorldSound::create(SoundResources::Platformer_Physical_Projectiles_ItemThrow1);
	this->healSound = WorldSound::create(SoundResources::Platformer_Spells_Heal2);

	this->addChild(this->throwSound);
	this->addChild(this->healSound);
}

ThrowHealthPotion::~ThrowHealthPotion()
{
}

PlatformerAttack* ThrowHealthPotion::cloneInternal()
{
	return ThrowHealthPotion::create(this->priority);
}

LocalizedString* ThrowHealthPotion::getString()
{
	return Strings::Items_Consumables_Health_HealthPotion::create();
}

LocalizedString* ThrowHealthPotion::getDescription()
{
	return Strings::Items_Consumables_Health_HealthPotionDescription::create()
		->setStringReplacementVariables(ConstantString::create(std::to_string(int(HealthPotion::HealPercentage * 100.0f))));
}

std::string ThrowHealthPotion::getAttackAnimation()
{
	return "ThrowItem";
}

void ThrowHealthPotion::onAttackTelegraphBegin()
{
	super::onAttackTelegraphBegin();
	
	this->throwSound->play(false, this->attackDuration / 2.0f);
}

void ThrowHealthPotion::performAttack(PlatformerEntity* owner, std::vector<PlatformerEntity*> targets)
{
	super::performAttack(owner, targets);

	for (PlatformerEntity* next : targets)
	{
		ThrownObject* potion = ThrownObject::create(owner, next, false, this->getIconResource(), CSize(64.0f, 64.0f));
		
		potion->whenCollidesWith({ (int)CombatCollisionType::EntityEnemy, (int)CombatCollisionType::EntityFriendly }, [=](CollisionData collisionData)
		{
			potion->disable(true);
			
			PlatformerEntity* entity = GameUtils::GetFirstParentOfType<PlatformerEntity>(collisionData.other, true);

			if (entity != nullptr)
			{
				int healing = int(std::round(float(entity->getRuntimeStateOrDefaultInt(StateKeys::MaxHealth, 0))) * HealthPotion::HealPercentage);

				this->healSound->play();
				CombatEvents::TriggerHealing(CombatEvents::DamageOrHealingArgs(owner, entity, healing, this->abilityType));
			}

			return CollisionResult::DoNothing;
		});

		this->replaceOffhandWithProjectile(owner, potion);

		next->getComponent<EntityProjectileTargetBehavior>([=](EntityProjectileTargetBehavior* behavior)
		{
			if (owner == next)
			{
				potion->launchTowardsTarget3D(behavior->getTarget(), Vec2(0.0f, 384.0f), 0.25f, Vec3(0.0f, 0.75f, 0.0f));
			}
			else
			{
				potion->launchTowardsTarget3D(behavior->getTarget(), Vec2::ZERO, 0.25f, Vec3(0.75f, 0.75f, 0.75f));
			}
		});
	}
}

void ThrowHealthPotion::onCleanup()
{
}
