#include "MinotaurCombatBehavior.h"

#include "Engine/Inventory/Inventory.h"
#include "Engine/Inventory/Item.h"
#include "Entities/Platformer/PlatformerEntity.h"
#include "Scenes/Platformer/Components/Entities/Inventory/EntityInventoryBehavior.h"
#include "Scenes/Platformer/Components/Entities/Combat/EntityAttackBehavior.h"
#include "Scenes/Platformer/Components/Entities/Combat/EntityBuffBehavior.h"
#include "Scenes/Platformer/Level/Combat/Attacks/Buffs/HealthLink/HealthLink.h"
#include "Scenes/Platformer/Level/Combat/Attacks/Abilities/BasicSlash/BasicSlash.h"
#include "Scenes/Platformer/Inventory/Items/Consumables/Health/IncrementHealthFlask/IncrementHealthFlask.h"

#include "Resources/UIResources.h"

using namespace cocos2d;
	
const std::string MinotaurCombatBehavior::MapKey = "minotaur-combat";

MinotaurCombatBehavior* MinotaurCombatBehavior::create(GameObject* owner)
{
	MinotaurCombatBehavior* instance = new MinotaurCombatBehavior(owner);

	instance->autorelease();

	return instance;
}

MinotaurCombatBehavior::MinotaurCombatBehavior(GameObject* owner) : super(owner)
{
	this->entity = dynamic_cast<PlatformerEntity*>(owner);

	if (this->entity == nullptr)
	{
		this->invalidate();
	}
	
	this->setTimelineSpeedBase(1.25f);
}

MinotaurCombatBehavior::~MinotaurCombatBehavior()
{
}

void MinotaurCombatBehavior::initializePositions()
{
}

void MinotaurCombatBehavior::onLoad()
{
	super::onLoad();
	
	this->entity->watchForComponent<EntityAttackBehavior>([=](EntityAttackBehavior* attackBehavior)
	{
		attackBehavior->registerAttack(BasicSlash::create(4, 6, 0.7f, EntityAttackBehavior::DefaultRecoverSpeed, PlatformerAttack::Priority::IfNecessary));
	});

	this->entity->watchForComponent<EntityBuffBehavior>([&](EntityBuffBehavior* entityBuffBehavior)
	{
		entityBuffBehavior->applyBuff(HealthLink::create(this->entity, this->entity));
	});
	
	this->entity->watchForComponent<EntityInventoryBehavior>([=](EntityInventoryBehavior* entityInventoryBehavior)
	{
		Inventory* inventory = entityInventoryBehavior->getInventory();

		if (inventory != nullptr)
		{
			inventory->forceInsert(IncrementHealthFlask::create());
		}
	});
}
