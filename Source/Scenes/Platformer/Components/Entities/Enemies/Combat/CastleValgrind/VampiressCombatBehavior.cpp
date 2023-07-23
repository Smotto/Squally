#include "VampiressCombatBehavior.h"

#include "Engine/Inventory/Inventory.h"
#include "Engine/Inventory/Item.h"
#include "Entities/Platformer/PlatformerEntity.h"
#include "Scenes/Platformer/Components/Entities/Inventory/EntityInventoryBehavior.h"
#include "Scenes/Platformer/Components/Entities/Combat/EntityAttackBehavior.h"
#include "Scenes/Platformer/Components/Entities/Combat/EntityBuffBehavior.h"
#include "Scenes/Platformer/Level/Combat/Attacks/Buffs/ManaLife/ManaLife.h"
#include "Scenes/Platformer/Level/Combat/Attacks/Debuffs/Leech/CastLeech.h"
#include "Scenes/Platformer/Level/Combat/Attacks/Abilities/BasicSlash/BasicSlash.h"

#include "Resources/UIResources.h"

using namespace cocos2d;
	
const std::string VampiressCombatBehavior::MapKey = "vampiress-combat";

VampiressCombatBehavior* VampiressCombatBehavior::create(GameObject* owner)
{
	VampiressCombatBehavior* instance = new VampiressCombatBehavior(owner);

	instance->autorelease();

	return instance;
}

VampiressCombatBehavior::VampiressCombatBehavior(GameObject* owner) : super(owner)
{
	this->entity = dynamic_cast<PlatformerEntity*>(owner);

	if (this->entity == nullptr)
	{
		this->invalidate();
	}
	
	this->setTimelineSpeedBase(1.25f);
}

VampiressCombatBehavior::~VampiressCombatBehavior()
{
}

void VampiressCombatBehavior::initializePositions()
{
}

void VampiressCombatBehavior::onLoad()
{
	super::onLoad();
	
	this->entity->watchForComponent<EntityAttackBehavior>([=](EntityAttackBehavior* attackBehavior)
	{
		attackBehavior->registerAttack(CastLeech::create(0.7f, EntityAttackBehavior::DefaultRecoverSpeed, PlatformerAttack::Priority::Guaranteed));
		attackBehavior->registerAttack(BasicSlash::create(6, 8, 0.7f, EntityAttackBehavior::DefaultRecoverSpeed, PlatformerAttack::Priority::Common));
	});

	this->entity->watchForComponent<EntityBuffBehavior>([&](EntityBuffBehavior* entityBuffBehavior)
	{
		entityBuffBehavior->applyBuff(ManaLife::create(this->entity, this->entity));
	});
	
	this->entity->watchForComponent<EntityInventoryBehavior>([=](EntityInventoryBehavior* entityInventoryBehavior)
	{
		Inventory* inventory = entityInventoryBehavior->getInventory();

		if (inventory != nullptr)
		{
		}
	});
}
