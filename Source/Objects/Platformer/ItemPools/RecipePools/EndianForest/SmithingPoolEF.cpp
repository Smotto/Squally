#include "SmithingPoolEF.h"

#include "cocos/base/CCValue.h"

#include "Engine/Inventory/ItemChance.h"
#include "Scenes/Platformer/Inventory/Items/PlatformerItems.h"

using namespace cocos2d;

const std::string SmithingPoolEF::PoolName = "smithing-pool-ef";

SmithingPoolEF* SmithingPoolEF::create(ValueMap& properties)
{
	SmithingPoolEF* instance = new SmithingPoolEF(properties);

	instance->autorelease();

	return instance;
}

SmithingPoolEF::SmithingPoolEF(ValueMap& properties) : super(properties, SmithingPoolEF::PoolName, SampleMethod::Unbounded, 1, 1)
{
	// Allows for stability in item ordering
	this->toggleDisableShuffle(true);
	
	// this->addItemToPool(ItemChance::create(BucketHelmPlans::create(), ItemChance::Probability::Guaranteed));
	// this->addItemToPool(ItemChance::create(OldShoePlans::create(), ItemChance::Probability::Guaranteed));
	
	this->addItemToPool(ItemChance::create(CopperSwordPlans::create(), ItemChance::Probability::Guaranteed));
	this->addItemToPool(ItemChance::create(CopperAxePlans::create(), ItemChance::Probability::Guaranteed));
	this->addItemToPool(ItemChance::create(CopperMalletPlans::create(), ItemChance::Probability::Guaranteed));
	this->addItemToPool(ItemChance::create(HuntersBowPlans::create(), ItemChance::Probability::Guaranteed));
	this->addItemToPool(ItemChance::create(QuartzWandPlans::create(), ItemChance::Probability::Guaranteed));
	
	this->addItemToPool(ItemChance::create(WoodenSwordPlans::create(), ItemChance::Probability::Guaranteed));
	this->addItemToPool(ItemChance::create(WoodenAxePlans::create(), ItemChance::Probability::Guaranteed));
	this->addItemToPool(ItemChance::create(WoodenClubPlans::create(), ItemChance::Probability::Guaranteed));
	this->addItemToPool(ItemChance::create(WoodenBowPlans::create(), ItemChance::Probability::Guaranteed));
	this->addItemToPool(ItemChance::create(WoodenWandPlans::create(), ItemChance::Probability::Guaranteed));
}

SmithingPoolEF::~SmithingPoolEF()
{
}
