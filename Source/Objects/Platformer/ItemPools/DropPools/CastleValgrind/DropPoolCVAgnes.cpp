#include "DropPoolCVAgnes.h"

#include "cocos/base/CCValue.h"

#include "Engine/Input/ClickableNode.h"
#include "Engine/Inventory/ItemChance.h"
#include "Engine/Localization/ConstantString.h"
#include "Engine/Utils/GameUtils.h"
#include "Objects/Platformer/ItemPools/CurrencyPools/CastleValgrind/CurrencyPoolCVGeneric.h"
#include "Objects/Platformer/ItemPools/Tiered/Tier4/AlchemyPoolTier4.h"
#include "Objects/Platformer/ItemPools/Tiered/Tier4/SmithingPoolTier4.h"
#include "Objects/Platformer/ItemPools/Tiered/Tier4/PotionPoolTier4.h"
#include "Scenes/Platformer/Inventory/Items/PlatformerItems.h"

#include "Resources/UIResources.h"

using namespace cocos2d;

const std::string DropPoolCVAgnes::PoolName = "cv-agnes";

DropPoolCVAgnes* DropPoolCVAgnes::create(ValueMap& properties)
{
	DropPoolCVAgnes* instance = new DropPoolCVAgnes(properties);

	instance->autorelease();

	return instance;
}

DropPoolCVAgnes::DropPoolCVAgnes(ValueMap& properties) : super(properties, DropPoolCVAgnes::PoolName, SampleMethod::Random, 3, 4, CurrencyPoolCVGeneric::create(properties),
	{
		AlchemyPoolTier4::create(SampleMethod::Random, 1, 2),
		SmithingPoolTier4::create(SampleMethod::Random, 1, 2),
	})
{
	this->addItemToPool(ItemChance::create(Mithril::create(), ItemChance::Probability::Guaranteed));
	this->addItemToPool(ItemChance::create(Mithril::create(), ItemChance::Probability::Guaranteed));
}

DropPoolCVAgnes::~DropPoolCVAgnes()
{
}
