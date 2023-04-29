#include "GoldenWandPlans.h"

#include "Engine/Inventory/CurrencyInventory.h"
#include "Scenes/Platformer/Inventory/Currencies/IOU.h"
#include "Scenes/Platformer/Inventory/Items/PlatformerItems.h"

#include "Resources/ItemResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

const std::string GoldenWandPlans::SaveKey = "golden-wand-plans";

GoldenWandPlans* GoldenWandPlans::create()
{
	GoldenWandPlans* instance = new GoldenWandPlans();

	instance->autorelease();

	return instance;
}

GoldenWandPlans::GoldenWandPlans() : super(CurrencyInventory::create({{ IOU::getIOUIdentifier(), 2 }}))
{
}

GoldenWandPlans::~GoldenWandPlans()
{
}

std::vector<Item*> GoldenWandPlans::craft()
{
	return { GoldenWand::create() };
}

std::vector<std::tuple<Item*, int>> GoldenWandPlans::getReagentsInternal()
{
	return
	{
		{ Emerald::create(), 1 },
		{ Gold::create(), 7 },
		{ Iron::create(), 2 },
		{ Coal::create(), 3 },
	};
}

Item* GoldenWandPlans::clone()
{
	return GoldenWandPlans::create();
}

LocalizedString* GoldenWandPlans::getString()
{
	return Strings::Items_Equipment_Weapons_Wands_GoldenWand::create();
}

const std::string& GoldenWandPlans::getIconResource()
{
	return ItemResources::Misc_SCROLL_1;
}

std::string GoldenWandPlans::getCraftedItemIconResource()
{
	return ItemResources::Equipment_Weapons_Wands_GoldenWand;
}

const std::string& GoldenWandPlans::getIdentifier()
{
	return GoldenWandPlans::SaveKey;
}
