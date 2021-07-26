#include "TrainingDummyCombatBehavior.h"

#include "Engine/Inventory/Inventory.h"
#include "Engine/Inventory/Item.h"
#include "Entities/Platformer/PlatformerEntity.h"
#include "Scenes/Platformer/Components/Entities/Inventory/EntityInventoryBehavior.h"
#include "Scenes/Platformer/Components/Entities/Combat/EntityAttackBehavior.h"
#include "Scenes/Platformer/Level/Combat/Attacks/Abilities/TrainingHeal/CastTrainingHeal.h"

#include "Resources/UIResources.h"

using namespace cocos2d;
	
const std::string TrainingDummyCombatBehavior::MapKey = "training-dummy-combat";

TrainingDummyCombatBehavior* TrainingDummyCombatBehavior::create(GameObject* owner)
{
	TrainingDummyCombatBehavior* instance = new TrainingDummyCombatBehavior(owner);

	instance->autorelease();

	return instance;
}

TrainingDummyCombatBehavior::TrainingDummyCombatBehavior(GameObject* owner) : super(owner)
{
	this->entity = dynamic_cast<PlatformerEntity*>(owner);

	if (this->entity == nullptr)
	{
		this->invalidate();
	}

	this->setTimelineSpeedBase(1.25f);
}

TrainingDummyCombatBehavior::~TrainingDummyCombatBehavior()
{
}

void TrainingDummyCombatBehavior::initializePositions()
{
}

void TrainingDummyCombatBehavior::onLoad()
{
	super::onLoad();
	
	this->entity->watchForComponent<EntityAttackBehavior>([=](EntityAttackBehavior* attackBehavior)
	{
		attackBehavior->registerAttack(CastTrainingHeal::create(0.7f, EntityAttackBehavior::DefaultRecoverSpeed, PlatformerAttack::Priority::Common));
	});
}
