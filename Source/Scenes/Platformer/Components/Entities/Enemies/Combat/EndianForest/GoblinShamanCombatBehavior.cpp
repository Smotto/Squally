#include "GoblinShamanCombatBehavior.h"

#include "Engine/Inventory/Inventory.h"
#include "Engine/Inventory/Item.h"
#include "Entities/Platformer/PlatformerEntity.h"
#include "Scenes/Platformer/Components/Entities/Inventory/EntityInventoryBehavior.h"
#include "Scenes/Platformer/Components/Entities/Combat/EntityAttackBehavior.h"
#include "Scenes/Platformer/Level/Combat/Attacks/Abilities/BasicSlash/BasicSlash.h"
#include "Scenes/Platformer/Level/Combat/Attacks/Abilities/ShadowBolt/CastShadowBolt.h"
#include "Scenes/Platformer/Level/Combat/Attacks/Debuffs/CurseOfTongues/CastCurseOfTongues.h"
#include "Scenes/Platformer/Inventory/Items/Consumables/Health/IncrementHealthFlask/IncrementHealthFlask.h"

#include "Resources/UIResources.h"

using namespace cocos2d;
	
const std::string GoblinShamanCombatBehavior::MapKey = "goblin-shaman-combat";

GoblinShamanCombatBehavior* GoblinShamanCombatBehavior::create(GameObject* owner)
{
	GoblinShamanCombatBehavior* instance = new GoblinShamanCombatBehavior(owner);

	instance->autorelease();

	return instance;
}

GoblinShamanCombatBehavior::GoblinShamanCombatBehavior(GameObject* owner) : super(owner)
{
	this->entity = dynamic_cast<PlatformerEntity*>(owner);

	if (this->entity == nullptr)
	{
		this->invalidate();
	}
	
	this->setTimelineSpeedBase(1.25f);
}

GoblinShamanCombatBehavior::~GoblinShamanCombatBehavior()
{
}

void GoblinShamanCombatBehavior::initializePositions()
{
}

void GoblinShamanCombatBehavior::onLoad()
{
	super::onLoad();
	
	this->entity->watchForComponent<EntityAttackBehavior>([=](EntityAttackBehavior* attackBehavior)
	{
		attackBehavior->registerAttack(CastCurseOfTongues::create(0.7f, EntityAttackBehavior::DefaultRecoverSpeed, PlatformerAttack::Priority::Guaranteed));
		attackBehavior->registerAttack(CastShadowBolt::create(0.7f, EntityAttackBehavior::DefaultRecoverSpeedVerySlow, PlatformerAttack::Priority::Reasonable));
		attackBehavior->registerAttack(BasicSlash::create(3, 5, 0.7f, EntityAttackBehavior::DefaultRecoverSpeed, PlatformerAttack::Priority::IfNecessary));
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
