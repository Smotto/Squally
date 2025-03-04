#include "SmithingPoolTier8.h"

#include "cocos/base/CCValue.h"

#include "Engine/Inventory/ItemChance.h"
#include "Scenes/Platformer/Inventory/Items/PlatformerItems.h"

using namespace cocos2d;

SmithingPoolTier8* SmithingPoolTier8::create(SampleMethod sampleMethod, int min, int max)
{
	SmithingPoolTier8* instance = new SmithingPoolTier8(sampleMethod, min, max);

	instance->autorelease();

	return instance;
}

SmithingPoolTier8::SmithingPoolTier8(SampleMethod sampleMethod, int min, int max) : super(ValueMap(), "smithing-pool-t8", sampleMethod, min, max)
{
	/**************
	 Misc
	**************/
	this->addItemToPool(ItemChance::create(Coal::create(), ItemChance::Probability::Reasonable));

	// Phasing out
	this->addItemToPool(ItemChance::create(DarkWood::create(), ItemChance::Probability::Uncommon));

	/**************
	 Metals
	**************/
	this->addItemToPool(ItemChance::create(Obsidian::create(), ItemChance::Probability::Uncommon));

	/**************
	 Gems
	**************/
	this->addItemToPool(ItemChance::create(VoidCrystal::create(), ItemChance::Probability::Common));
}

SmithingPoolTier8::~SmithingPoolTier8()
{
}
