#include "GuanoAttackBehavior.h"

#include "Engine/Inventory/Inventory.h"
#include "Engine/Inventory/Item.h"
#include "Entities/Platformer/PlatformerEntity.h"
#include "Scenes/Platformer/Components/Entities/Inventory/EntityInventoryBehavior.h"
#include "Scenes/Platformer/Components/Entities/Combat/EntityAttackBehavior.h"
#include "Scenes/Platformer/Inventory/Items/Consumables/Health/IncrementHealthFlask/IncrementHealthFlask.h"
#include "Scenes/Platformer/Level/Combat/Attacks/Abilities/BasicSlashAxe/BasicSlashAxe.h"
#include "Scenes/Platformer/Level/Combat/Buffs/Defend/CastDefend.h"
#include "Scenes/Platformer/Level/Combat/Buffs/Distract/CastDistract.h"

#include "Resources/UIResources.h"

using namespace cocos2d;
	
const std::string GuanoAttackBehavior::MapKey = "guano-attack";

GuanoAttackBehavior* GuanoAttackBehavior::create(GameObject* owner)
{
	GuanoAttackBehavior* instance = new GuanoAttackBehavior(owner);

	instance->autorelease();

	return instance;
}

GuanoAttackBehavior::GuanoAttackBehavior(GameObject* owner) : super(owner)
{
	this->entity = dynamic_cast<PlatformerEntity*>(owner);

	if (this->entity == nullptr)
	{
		this->invalidate();
	}
}

GuanoAttackBehavior::~GuanoAttackBehavior()
{
}

void GuanoAttackBehavior::initializePositions()
{
}

void GuanoAttackBehavior::onLoad()
{
	this->entity->watchForComponent<EntityAttackBehavior>([=](EntityAttackBehavior* attackBehavior)
	{
		std::tuple<int, int> attackRange = attackBehavior->computeAttackRange();
		int minAttack = std::get<0>(attackRange);
		int maxAttack = std::get<1>(attackRange);

		attackBehavior->registerAttack(BasicSlashAxe::create(minAttack, maxAttack, 0.7f, EntityAttackBehavior::DefaultRecoverSpeed, PlatformerAttack::Priority::Common));
		attackBehavior->registerDefensive(CastDefend::create(0.7f, EntityAttackBehavior::DefaultRecoverSpeed, PlatformerAttack::Priority::Common));
		attackBehavior->registerDefensive(CastDistract::create(0.7f, EntityAttackBehavior::DefaultRecoverSpeed, PlatformerAttack::Priority::Common));
	});
}

void GuanoAttackBehavior::onDisable()
{
	super::onDisable();
}
