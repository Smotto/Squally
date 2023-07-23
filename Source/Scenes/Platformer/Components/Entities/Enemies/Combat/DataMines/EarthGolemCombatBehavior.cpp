#include "EarthGolemCombatBehavior.h"

#include "Engine/Inventory/Inventory.h"
#include "Engine/Inventory/Item.h"
#include "Entities/Platformer/PlatformerEntity.h"
#include "Scenes/Platformer/Components/Entities/Inventory/EntityInventoryBehavior.h"
#include "Scenes/Platformer/Components/Entities/Combat/EntityAttackBehavior.h"
#include "Scenes/Platformer/Level/Combat/Attacks/Abilities/BasicSlash/BasicSlash.h"
#include "Scenes/Platformer/Level/Combat/Attacks/Abilities/RockSlide/CastRockSlide.h"

#include "Resources/ObjectResources.h"
#include "Resources/UIResources.h"

using namespace cocos2d;
	
const std::string EarthGolemCombatBehavior::MapKey = "earth-golem-combat";

EarthGolemCombatBehavior* EarthGolemCombatBehavior::create(GameObject* owner)
{
	EarthGolemCombatBehavior* instance = new EarthGolemCombatBehavior(owner);

	instance->autorelease();

	return instance;
}

EarthGolemCombatBehavior::EarthGolemCombatBehavior(GameObject* owner) : super(owner)
{
	this->entity = dynamic_cast<PlatformerEntity*>(owner);

	if (this->entity == nullptr)
	{
		this->invalidate();
	}
	
	this->setTimelineSpeedBase(1.25f);
}

EarthGolemCombatBehavior::~EarthGolemCombatBehavior()
{
}

void EarthGolemCombatBehavior::initializePositions()
{
}

void EarthGolemCombatBehavior::onLoad()
{
	super::onLoad();
	
	this->entity->watchForComponent<EntityAttackBehavior>([=](EntityAttackBehavior* attackBehavior)
	{
		attackBehavior->registerAttack(CastRockSlide::create(0.7f, EntityAttackBehavior::DefaultRecoverSpeed, PlatformerAttack::Priority::Guaranteed, ObjectResources::Traps_Catapult_BALL));
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
