#include "SapphireWandPlans.h"

#include "Engine/Inventory/CurrencyInventory.h"
#include "Scenes/Platformer/Inventory/Currencies/IOU.h"
#include "Scenes/Platformer/Inventory/Items/PlatformerItems.h"

#include "Resources/ItemResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

const std::string SapphireWandPlans::SaveKey = "serpent-wand-plans";

SapphireWandPlans* SapphireWandPlans::create()
{
	SapphireWandPlans* instance = new SapphireWandPlans();

	instance->autorelease();

	return instance;
}

SapphireWandPlans::SapphireWandPlans() : super(CurrencyInventory::create({{ IOU::getIOUIdentifier(), 2 }}))
{
}

SapphireWandPlans::~SapphireWandPlans()
{
}

std::vector<Item*> SapphireWandPlans::craft()
{
	return { SapphireWand::create() };
}

std::vector<std::tuple<Item*, int>> SapphireWandPlans::getReagentsInternal()
{
	return
	{
		{ Sapphire::create(), 1 },
		{ Gold::create(), 4 },
		{ Iron::create(), 6 },
		{ Coal::create(), 4 },
	};
}

Item* SapphireWandPlans::clone()
{
	return SapphireWandPlans::create();
}

LocalizedString* SapphireWandPlans::getString()
{
	return Strings::Items_Equipment_Weapons_Wands_SapphireWand::create();
}

const std::string& SapphireWandPlans::getIconResource()
{
	return ItemResources::Misc_SCROLL_1;
}

std::string SapphireWandPlans::getCraftedItemIconResource()
{
	return ItemResources::Equipment_Weapons_Wands_SapphireWand;
}

const std::string& SapphireWandPlans::getIdentifier()
{
	return SapphireWandPlans::SaveKey;
}
