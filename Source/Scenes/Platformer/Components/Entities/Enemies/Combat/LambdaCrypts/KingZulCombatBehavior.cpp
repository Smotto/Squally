#include "KingZulCombatBehavior.h"

#include "Engine/Inventory/Inventory.h"
#include "Engine/Inventory/Item.h"
#include "Entities/Platformer/PlatformerEntity.h"
#include "Scenes/Platformer/Components/Entities/Inventory/EntityInventoryBehavior.h"
#include "Scenes/Platformer/Components/Entities/Combat/EntityAttackBehavior.h"
#include "Scenes/Platformer/Level/Combat/Attacks/Debuffs/Daze/CastDaze.h"
#include "Scenes/Platformer/Level/Combat/Attacks/Abilities/BasicSlash/BasicSlash.h"

#include "Resources/UIResources.h"

using namespace cocos2d;
	
const std::string KingZulCombatBehavior::MapKey = "king-zul-combat";

KingZulCombatBehavior* KingZulCombatBehavior::create(GameObject* owner)
{
	KingZulCombatBehavior* instance = new KingZulCombatBehavior(owner);

	instance->autorelease();

	return instance;
}

KingZulCombatBehavior::KingZulCombatBehavior(GameObject* owner) : super(owner)
{
	this->entity = dynamic_cast<PlatformerEntity*>(owner);

	if (this->entity == nullptr)
	{
		this->invalidate();
	}
	
	this->setTimelineSpeedBase(1.25f);
}

KingZulCombatBehavior::~KingZulCombatBehavior()
{
}

void KingZulCombatBehavior::initializePositions()
{
}

void KingZulCombatBehavior::onLoad()
{
	super::onLoad();
	
	this->entity->watchForComponent<EntityAttackBehavior>([=](EntityAttackBehavior* attackBehavior)
	{
		attackBehavior->registerAttack(CastDaze::create(0.7f, EntityAttackBehavior::DefaultRecoverSpeed, PlatformerAttack::Priority::Guaranteed));
		attackBehavior->registerAttack(BasicSlash::create(9, 10, 0.7f, EntityAttackBehavior::DefaultRecoverSpeed, PlatformerAttack::Priority::Common));
	});
	
	this->entity->watchForComponent<EntityInventoryBehavior>([=](EntityInventoryBehavior* entityInventoryBehavior)
	{
		Inventory* inventory = entityInventoryBehavior->getInventory();

		if (inventory != nullptr)
		{
		}
	});
}
