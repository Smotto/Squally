#include "CrystalBowPlans.h"

#include "Engine/Inventory/CurrencyInventory.h"
#include "Scenes/Platformer/Inventory/Currencies/IOU.h"
#include "Scenes/Platformer/Inventory/Items/PlatformerItems.h"

#include "Resources/ItemResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

const std::string CrystalBowPlans::SaveKey = "crystal-bow-plans";

CrystalBowPlans* CrystalBowPlans::create()
{
	CrystalBowPlans* instance = new CrystalBowPlans();

	instance->autorelease();

	return instance;
}

CrystalBowPlans::CrystalBowPlans() : super(CurrencyInventory::create({{ IOU::getIOUIdentifier(), 2 }}))
{
}

CrystalBowPlans::~CrystalBowPlans()
{
}

std::vector<Item*> CrystalBowPlans::craft()
{
	return { CrystalBow::create() };
}

std::vector<std::tuple<Item*, int>> CrystalBowPlans::getReagentsInternal()
{
	return
	{
		{ Crystal::create(), 5 },
		{ Mithril::create(), 3 },
		{ Iron::create(), 2 },
		{ Coal::create(), 4 },
	};
}

Item* CrystalBowPlans::clone()
{
	return CrystalBowPlans::create();
}

LocalizedString* CrystalBowPlans::getString()
{
	return Strings::Items_Equipment_Weapons_Bows_CrystalBow::create();
}

const std::string& CrystalBowPlans::getIconResource()
{
	return ItemResources::Misc_SCROLL_1;
}

std::string CrystalBowPlans::getCraftedItemIconResource()
{
	return ItemResources::Equipment_Weapons_Bows_CrystalBow;
}

const std::string& CrystalBowPlans::getIdentifier()
{
	return CrystalBowPlans::SaveKey;
}
