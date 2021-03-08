#include "SteelAxePlans.h"

#include "Engine/Inventory/CurrencyInventory.h"
#include "Scenes/Platformer/Inventory/Currencies/IOU.h"
#include "Scenes/Platformer/Inventory/Items/PlatformerItems.h"

#include "Resources/ItemResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

const std::string SteelAxePlans::SaveKey = "steel-axe-plans";

SteelAxePlans* SteelAxePlans::create()
{
	SteelAxePlans* instance = new SteelAxePlans();

	instance->autorelease();

	return instance;
}

SteelAxePlans::SteelAxePlans() : super(CurrencyInventory::create({{ IOU::getIOUIdentifier(), 2 }}))
{
}

SteelAxePlans::~SteelAxePlans()
{
}

std::vector<Item*> SteelAxePlans::craft()
{
	return { SteelAxe::create() };
}

std::vector<std::tuple<Item*, int>> SteelAxePlans::getReagentsInternal()
{
	return
	{
		{ Wood::create(), 4 },
		{ Coal::create(), 4 },
		{ Iron::create(), 5 },
		{ Copper::create(), 2 },
	};
}

Item* SteelAxePlans::clone()
{
	return SteelAxePlans::create();
}

const std::string& SteelAxePlans::getItemName()
{
	return SteelAxePlans::SaveKey;
}

LocalizedString* SteelAxePlans::getString()
{
	return Strings::Items_Equipment_Weapons_Axes_SteelAxe::create();
}

const std::string& SteelAxePlans::getIconResource()
{
	return ItemResources::Misc_SCROLL_1;
}

std::string SteelAxePlans::getCraftedItemIconResource()
{
	return ItemResources::Equipment_Weapons_Axes_SteelAxe;
}

const std::string& SteelAxePlans::getSerializationKey()
{
	return SteelAxePlans::SaveKey;
}
