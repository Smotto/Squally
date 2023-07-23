#include "BoneWandPlans.h"

#include "Engine/Inventory/CurrencyInventory.h"
#include "Scenes/Platformer/Inventory/Currencies/IOU.h"
#include "Scenes/Platformer/Inventory/Items/PlatformerItems.h"

#include "Resources/ItemResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

const std::string BoneWandPlans::SaveKey = "bone-wand-plans";

BoneWandPlans* BoneWandPlans::create()
{
	BoneWandPlans* instance = new BoneWandPlans();

	instance->autorelease();

	return instance;
}

BoneWandPlans::BoneWandPlans() : super(CurrencyInventory::create({{ IOU::getIOUIdentifier(), 2 }}))
{
}

BoneWandPlans::~BoneWandPlans()
{
}

std::vector<Item*> BoneWandPlans::craft()
{
	return { BoneWand::create() };
}

std::vector<std::tuple<Item*, int>> BoneWandPlans::getReagentsInternal()
{
	return
	{
		{ Diamond::create(), 1 },
		{ Bone::create(), 4 },
		{ DarkWood::create(), 6 },
		{ Coal::create(), 6 },
	};
}

Item* BoneWandPlans::clone()
{
	return BoneWandPlans::create();
}

LocalizedString* BoneWandPlans::getString()
{
	return Strings::Items_Equipment_Weapons_Wands_BoneWand::create();
}

const std::string& BoneWandPlans::getIconResource()
{
	return ItemResources::Misc_SCROLL_1;
}

std::string BoneWandPlans::getCraftedItemIconResource()
{
	return ItemResources::Equipment_Weapons_Wands_BoneWand;
}

const std::string& BoneWandPlans::getIdentifier()
{
	return BoneWandPlans::SaveKey;
}
