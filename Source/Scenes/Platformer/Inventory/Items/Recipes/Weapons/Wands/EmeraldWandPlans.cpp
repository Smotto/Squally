#include "EmeraldWandPlans.h"

#include "Engine/Inventory/CurrencyInventory.h"
#include "Scenes/Platformer/Inventory/Currencies/IOU.h"
#include "Scenes/Platformer/Inventory/Items/PlatformerItems.h"

#include "Resources/ItemResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

const std::string EmeraldWandPlans::SaveKey = "emerald-wand-plans";

EmeraldWandPlans* EmeraldWandPlans::create()
{
	EmeraldWandPlans* instance = new EmeraldWandPlans();

	instance->autorelease();

	return instance;
}

EmeraldWandPlans::EmeraldWandPlans() : super(CurrencyInventory::create({{ IOU::getIOUIdentifier(), 2 }}))
{
}

EmeraldWandPlans::~EmeraldWandPlans()
{
}

std::vector<Item*> EmeraldWandPlans::craft()
{
	return { EmeraldWand::create() };
}

std::vector<std::tuple<Item*, int>> EmeraldWandPlans::getReagentsInternal()
{
	return
	{
		{ Copper::create(), 2 },
		{ Iron::create(), 3 },
		{ Emerald::create(), 2 },
		{ Coal::create(), 4 },
	};
}

Item* EmeraldWandPlans::clone()
{
	return EmeraldWandPlans::create();
}

LocalizedString* EmeraldWandPlans::getString()
{
	return Strings::Items_Equipment_Weapons_Wands_EmeraldWand::create();
}

const std::string& EmeraldWandPlans::getIconResource()
{
	return ItemResources::Misc_SCROLL_1;
}

std::string EmeraldWandPlans::getCraftedItemIconResource()
{
	return ItemResources::Equipment_Weapons_Wands_EmeraldWand;
}

const std::string& EmeraldWandPlans::getIdentifier()
{
	return EmeraldWandPlans::SaveKey;
}
