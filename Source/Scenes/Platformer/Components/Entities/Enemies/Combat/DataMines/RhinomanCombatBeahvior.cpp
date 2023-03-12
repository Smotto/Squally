#include "RhinomanCombatBeahvior.h"

#include "Engine/Inventory/Inventory.h"
#include "Engine/Inventory/Item.h"
#include "Entities/Platformer/PlatformerEntity.h"
#include "Scenes/Platformer/Components/Entities/Inventory/EntityInventoryBehavior.h"
#include "Scenes/Platformer/Components/Entities/Combat/EntityAttackBehavior.h"
#include "Scenes/Platformer/Level/Combat/Attacks/Abilities/BasicSlash/BasicSlash.h"
#include "Scenes/Platformer/Level/Combat/Attacks/Debuffs/BrittleBones/CastBrittleBones.h"

#include "Resources/UIResources.h"

using namespace cocos2d;
	
const std::string RhinomanCombatBeahvior::MapKey = "rhinoman-combat";

RhinomanCombatBeahvior* RhinomanCombatBeahvior::create(GameObject* owner)
{
	RhinomanCombatBeahvior* instance = new RhinomanCombatBeahvior(owner);

	instance->autorelease();

	return instance;
}

RhinomanCombatBeahvior::RhinomanCombatBeahvior(GameObject* owner) : super(owner)
{
	this->entity = dynamic_cast<PlatformerEntity*>(owner);

	if (this->entity == nullptr)
	{
		this->invalidate();
	}
	
	this->setTimelineSpeedBase(1.25f);
}

RhinomanCombatBeahvior::~RhinomanCombatBeahvior()
{
}

void RhinomanCombatBeahvior::initializePositions()
{
}

void RhinomanCombatBeahvior::onLoad()
{
	super::onLoad();
	
	this->entity->watchForComponent<EntityAttackBehavior>([=](EntityAttackBehavior* attackBehavior)
	{
		attackBehavior->registerAttack(CastBrittleBones::create(0.7f, EntityAttackBehavior::DefaultRecoverSpeed, PlatformerAttack::Priority::Guaranteed));
		attackBehavior->registerAttack(BasicSlash::create(5, 7, 0.7f, EntityAttackBehavior::DefaultRecoverSpeed, PlatformerAttack::Priority::Common));
	});
	
	this->entity->watchForComponent<EntityInventoryBehavior>([=](EntityInventoryBehavior* entityInventoryBehavior)
	{
		Inventory* inventory = entityInventoryBehavior->getInventory();

		if (inventory != nullptr)
		{
		}
	});
}
