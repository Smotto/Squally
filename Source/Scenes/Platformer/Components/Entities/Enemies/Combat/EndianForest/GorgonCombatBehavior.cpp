#include "GorgonCombatBehavior.h"

#include "Engine/Inventory/Inventory.h"
#include "Engine/Inventory/Item.h"
#include "Entities/Platformer/PlatformerEntity.h"
#include "Scenes/Platformer/Components/Entities/Inventory/EntityInventoryBehavior.h"
#include "Scenes/Platformer/Components/Entities/Combat/EntityAttackBehavior.h"
#include "Scenes/Platformer/Level/Combat/Attacks/Abilities/DoubleSlash/DoubleSlash.h"
#include "Scenes/Platformer/Level/Combat/Attacks/Buffs/MultiCast/CastStoneSkinHaste.h"
#include "Scenes/Platformer/Inventory/Items/Consumables/Health/IncrementHealthFlask/IncrementHealthFlask.h"

#include "Resources/UIResources.h"

using namespace cocos2d;
	
const std::string GorgonCombatBehavior::MapKey = "gorgon-combat";

GorgonCombatBehavior* GorgonCombatBehavior::create(GameObject* owner)
{
	GorgonCombatBehavior* instance = new GorgonCombatBehavior(owner);

	instance->autorelease();

	return instance;
}

GorgonCombatBehavior::GorgonCombatBehavior(GameObject* owner) : super(owner)
{
	this->entity = dynamic_cast<PlatformerEntity*>(owner);

	if (this->entity == nullptr)
	{
		this->invalidate();
	}
	
	this->setTimelineSpeedBase(1.20f);
}

GorgonCombatBehavior::~GorgonCombatBehavior()
{
}

void GorgonCombatBehavior::initializePositions()
{
}

void GorgonCombatBehavior::onLoad()
{
	super::onLoad();
	
	this->entity->watchForComponent<EntityAttackBehavior>([=](EntityAttackBehavior* attackBehavior)
	{
		attackBehavior->registerAttack(CastStoneSkinHaste::create(0.7f, EntityAttackBehavior::DefaultRecoverSpeed, PlatformerAttack::Priority::Guaranteed));
		attackBehavior->registerAttack(DoubleSlash::create(4, 6, 0.7f, EntityAttackBehavior::DefaultRecoverSpeed, PlatformerAttack::Priority::VeryCommon));
	});
	
	this->entity->watchForComponent<EntityInventoryBehavior>([=](EntityInventoryBehavior* entityInventoryBehavior)
	{
		Inventory* inventory = entityInventoryBehavior->getInventory();

		if (inventory != nullptr)
		{
			// inventory->forceInsert(IncrementHealthFlask::create());
		}
	});
}
