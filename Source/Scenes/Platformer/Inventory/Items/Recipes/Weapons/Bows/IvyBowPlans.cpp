#include "IvyBowPlans.h"

#include "Engine/Inventory/CurrencyInventory.h"
#include "Scenes/Platformer/Inventory/Currencies/IOU.h"
#include "Scenes/Platformer/Inventory/Items/PlatformerItems.h"

#include "Resources/ItemResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

const std::string IvyBowPlans::SaveKey = "ivy-bow-plans";

IvyBowPlans* IvyBowPlans::create()
{
	IvyBowPlans* instance = new IvyBowPlans();

	instance->autorelease();

	return instance;
}

IvyBowPlans::IvyBowPlans() : super(CurrencyInventory::create({{ IOU::getIOUIdentifier(), 2 }}))
{
}

IvyBowPlans::~IvyBowPlans()
{
}

std::vector<Item*> IvyBowPlans::craft()
{
	return { IvyBow::create() };
}

std::vector<std::tuple<Item*, int>> IvyBowPlans::getReagentsInternal()
{
	return
	{
		{ Wood::create(), 12 },
		{ Mithril::create(), 4 },
	};
}

Item* IvyBowPlans::clone()
{
	return IvyBowPlans::create();
}

LocalizedString* IvyBowPlans::getString()
{
	return Strings::Items_Equipment_Weapons_Bows_IvyBow::create();
}

const std::string& IvyBowPlans::getIconResource()
{
	return ItemResources::Misc_SCROLL_1;
}

std::string IvyBowPlans::getCraftedItemIconResource()
{
	return ItemResources::Equipment_Weapons_Bows_IvyBow;
}

const std::string& IvyBowPlans::getIdentifier()
{
	return IvyBowPlans::SaveKey;
}
