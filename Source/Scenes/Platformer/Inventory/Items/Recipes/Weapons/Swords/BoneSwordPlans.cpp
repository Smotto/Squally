#include "BoneSwordPlans.h"

#include "Engine/Inventory/CurrencyInventory.h"
#include "Scenes/Platformer/Inventory/Currencies/IOU.h"
#include "Scenes/Platformer/Inventory/Items/PlatformerItems.h"

#include "Resources/ItemResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

const std::string BoneSwordPlans::SaveKey = "bone-sword-plans";

BoneSwordPlans* BoneSwordPlans::create()
{
	BoneSwordPlans* instance = new BoneSwordPlans();

	instance->autorelease();

	return instance;
}

BoneSwordPlans::BoneSwordPlans() : super(CurrencyInventory::create({{ IOU::getIOUIdentifier(), 2 }}))
{
}

BoneSwordPlans::~BoneSwordPlans()
{
}

std::vector<Item*> BoneSwordPlans::craft()
{
	return { BoneSword::create() };
}

std::vector<std::tuple<Item*, int>> BoneSwordPlans::getReagentsInternal()
{
	return
	{
		{ Bone::create(), 12 },
		{ Iron::create(), 4 },
		{ DarkWood::create(), 2 },
	};
}

Item* BoneSwordPlans::clone()
{
	return BoneSwordPlans::create();
}

LocalizedString* BoneSwordPlans::getString()
{
	return Strings::Items_Equipment_Weapons_Swords_BoneSword::create();
}

const std::string& BoneSwordPlans::getIconResource()
{
	return ItemResources::Misc_SCROLL_1;
}

std::string BoneSwordPlans::getCraftedItemIconResource()
{
	return ItemResources::Equipment_Weapons_Swords_BoneSword;
}

const std::string& BoneSwordPlans::getIdentifier()
{
	return BoneSwordPlans::SaveKey;
}
