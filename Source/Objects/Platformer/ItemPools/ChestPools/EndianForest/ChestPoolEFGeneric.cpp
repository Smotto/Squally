#include "ChestPoolEFGeneric.h"

#include "cocos/base/CCValue.h"

#include "Engine/Inventory/MergePool.h"
#include "Objects/Platformer/ItemPools/Tiered/Tier1/AlchemyPoolTier1.h"
#include "Objects/Platformer/ItemPools/Tiered/Tier1/SmithingPoolTier1.h"
#include "Objects/Platformer/ItemPools/Tiered/Tier1/PotionPoolTier1.h"
#include "Scenes/Platformer/Inventory/Items/PlatformerItems.h"

using namespace cocos2d;

const std::string ChestPoolEFGeneric::PoolName = "ef-generic";

ChestPoolEFGeneric* ChestPoolEFGeneric::create(ValueMap& properties)
{
	ChestPoolEFGeneric* instance = new ChestPoolEFGeneric(properties);

	instance->autorelease();

	return instance;
}

ChestPoolEFGeneric::ChestPoolEFGeneric(ValueMap& properties) : super(
	properties, ChestPoolEFGeneric::PoolName, SampleMethod::Guarantee, 2, 3,
	{
		MergePool::create(SampleMethod::Random, 3, 5,
		{
			AlchemyPoolTier1::create(),
			SmithingPoolTier1::create(),
			PotionPoolTier1::create()
		})
	})
{
}

ChestPoolEFGeneric::~ChestPoolEFGeneric()
{
}
