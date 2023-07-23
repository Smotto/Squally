#include "SquallyCombatBehaviorGroup.h"

#include "Engine/Maps/GameObject.h"
#include "Scenes/Platformer/Components/Entities/Combat/EntityCombatBehaviorGroup.h"
#include "Scenes/Platformer/Components/Entities/Helpers/Scrappy/Combat/ScrappyCombatManagerBehavior.h"
#include "Scenes/Platformer/Components/Entities/Movement/EntityFloatBehavior.h"
#include "Scenes/Platformer/Components/Entities/Squally/Abilities/SquallyHackingBehavior.h"
#include "Scenes/Platformer/Components/Entities/Squally/Combat/SquallyAttackBehavior.h"
#include "Scenes/Platformer/Components/Entities/Squally/Combat/SquallyCombatRespawnBehavior.h"
#include "Scenes/Platformer/Components/Entities/Squally/Inventory/SquallyInventoryBehavior.h"
#include "Scenes/Platformer/Components/Entities/Squally/Inventory/SquallyReceiveItemBehavior.h"
#include "Scenes/Platformer/Components/Entities/Squally/Stats/SquallyStatsBehaviorGroup.h"
#include "Scenes/Platformer/Components/Entities/Squally/Visual/SquallyVisualBehaviorGroup.h"

using namespace cocos2d;

const std::string SquallyCombatBehaviorGroup::MapKey = "squally-combat";

SquallyCombatBehaviorGroup* SquallyCombatBehaviorGroup::create(GameObject* owner)
{
	SquallyCombatBehaviorGroup* instance = new SquallyCombatBehaviorGroup(owner);

	instance->autorelease();

	return instance;
}

SquallyCombatBehaviorGroup::SquallyCombatBehaviorGroup(GameObject* owner) : super(owner, {
	EntityCombatBehaviorGroup::create(owner),
	EntityFloatBehavior::create(owner),
	SquallyInventoryBehavior::create(owner),
	ScrappyCombatManagerBehavior::create(owner),
	SquallyHackingBehavior::create(owner),
	SquallyAttackBehavior::create(owner),
	SquallyStatsBehaviorGroup::create(owner),
	SquallyCombatRespawnBehavior::create(owner),
	SquallyVisualBehaviorGroup::create(owner),
	SquallyReceiveItemBehavior::create(owner),
	})
{
}

SquallyCombatBehaviorGroup::~SquallyCombatBehaviorGroup()
{
}

void SquallyCombatBehaviorGroup::onLoad()
{
	super::onLoad();
}

void SquallyCombatBehaviorGroup::onDisable()
{
	super::onDisable();
}
