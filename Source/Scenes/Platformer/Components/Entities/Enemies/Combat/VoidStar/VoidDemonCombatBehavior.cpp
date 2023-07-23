#include "VoidDemonCombatBehavior.h"

#include "Engine/Inventory/Inventory.h"
#include "Engine/Inventory/Item.h"
#include "Entities/Platformer/PlatformerEntity.h"
#include "Scenes/Platformer/Components/Entities/Inventory/EntityInventoryBehavior.h"
#include "Scenes/Platformer/Components/Entities/Combat/EntityAttackBehavior.h"
#include "Scenes/Platformer/Level/Combat/Attacks/Abilities/ShadowBolt/CastShadowBolt.h"
#include "Scenes/Platformer/Level/Combat/Attacks/Abilities/BasicSlash/BasicSlash.h"
#include "Scenes/Platformer/Level/Combat/Attacks/Debuffs/CurseOfTongues/CastCurseOfTongues.h"
#include "Scenes/Platformer/Level/Combat/Attacks/Debuffs/ManaDrain/CastManaDrain.h"

#include "Resources/UIResources.h"

using namespace cocos2d;
	
const std::string VoidDemonCombatBehavior::MapKey = "void-demon-combat";

VoidDemonCombatBehavior* VoidDemonCombatBehavior::create(GameObject* owner)
{
	VoidDemonCombatBehavior* instance = new VoidDemonCombatBehavior(owner);

	instance->autorelease();

	return instance;
}

VoidDemonCombatBehavior::VoidDemonCombatBehavior(GameObject* owner) : super(owner)
{
	this->entity = dynamic_cast<PlatformerEntity*>(owner);

	if (this->entity == nullptr)
	{
		this->invalidate();
	}
	
	this->setTimelineSpeedBase(1.25f);
}

VoidDemonCombatBehavior::~VoidDemonCombatBehavior()
{
}

void VoidDemonCombatBehavior::initializePositions()
{
}

void VoidDemonCombatBehavior::onLoad()
{
	super::onLoad();
	
	this->entity->watchForComponent<EntityAttackBehavior>([=](EntityAttackBehavior* attackBehavior)
	{
		attackBehavior->registerAttack(CastCurseOfTongues::create(0.7f, EntityAttackBehavior::DefaultRecoverSpeed, PlatformerAttack::Priority::VeryCommon));
		attackBehavior->registerAttack(CastManaDrain::create(0.7f, EntityAttackBehavior::DefaultRecoverSpeed, PlatformerAttack::Priority::VeryCommon));
		attackBehavior->registerAttack(BasicSlash::create(3, 5, 0.7f, EntityAttackBehavior::DefaultRecoverSpeed, PlatformerAttack::Priority::IfNecessary));
	});
	
	this->entity->watchForComponent<EntityInventoryBehavior>([=](EntityInventoryBehavior* entityInventoryBehavior)
	{
		Inventory* inventory = entityInventoryBehavior->getInventory();

		if (inventory != nullptr)
		{
		}
	});
}
