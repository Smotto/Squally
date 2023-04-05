#include "BoneAxePlans.h"

#include "Engine/Inventory/CurrencyInventory.h"
#include "Scenes/Platformer/Inventory/Currencies/IOU.h"
#include "Scenes/Platformer/Inventory/Items/PlatformerItems.h"

#include "Resources/ItemResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

const std::string BoneAxePlans::SaveKey = "bone-axe-plans";

BoneAxePlans* BoneAxePlans::create()
{
	BoneAxePlans* instance = new BoneAxePlans();

	instance->autorelease();

	return instance;
}

BoneAxePlans::BoneAxePlans() : super(CurrencyInventory::create({{ IOU::getIOUIdentifier(), 2 }}))
{
}

BoneAxePlans::~BoneAxePlans()
{
}

std::vector<Item*> BoneAxePlans::craft()
{
	return { BoneAxe::create() };
}

std::vector<std::tuple<Item*, int>> BoneAxePlans::getReagentsInternal()
{
	return
	{
		{ Bone::create(), 10 },
		{ Iron::create(), 4 },
		{ DarkWood::create(), 2 },
	};
}

Item* BoneAxePlans::clone()
{
	return BoneAxePlans::create();
}

LocalizedString* BoneAxePlans::getString()
{
	return Strings::Items_Equipment_Weapons_Axes_BoneAxe::create();
}

const std::string& BoneAxePlans::getIconResource()
{
	return ItemResources::Misc_SCROLL_1;
}

std::string BoneAxePlans::getCraftedItemIconResource()
{
	return ItemResources::Equipment_Weapons_Axes_BoneAxe;
}

const std::string& BoneAxePlans::getIdentifier()
{
	return BoneAxePlans::SaveKey;
}
