#include "VoidSwordPlans.h"

#include "Engine/Inventory/CurrencyInventory.h"
#include "Scenes/Platformer/Inventory/Currencies/IOU.h"
#include "Scenes/Platformer/Inventory/Items/PlatformerItems.h"

#include "Resources/ItemResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

const std::string VoidSwordPlans::SaveKey = "void-sword-plans";

VoidSwordPlans* VoidSwordPlans::create()
{
	VoidSwordPlans* instance = new VoidSwordPlans();

	instance->autorelease();

	return instance;
}

VoidSwordPlans::VoidSwordPlans() : super(CurrencyInventory::create({{ IOU::getIOUIdentifier(), 2 }}))
{
}

VoidSwordPlans::~VoidSwordPlans()
{
}

std::vector<Item*> VoidSwordPlans::craft()
{
	return { VoidSword::create() };
}

std::vector<std::tuple<Item*, int>> VoidSwordPlans::getReagentsInternal()
{
	return
	{
		{ Obsidian::create(), 4 },
		{ VoidCrystal::create(), 6 },
		{ Coal::create(), 8 },
	};
}

Item* VoidSwordPlans::clone()
{
	return VoidSwordPlans::create();
}

LocalizedString* VoidSwordPlans::getString()
{
	return Strings::Items_Equipment_Weapons_Swords_VoidSword::create();
}

const std::string& VoidSwordPlans::getIconResource()
{
	return ItemResources::Misc_SCROLL_1;
}

std::string VoidSwordPlans::getCraftedItemIconResource()
{
	return ItemResources::Equipment_Weapons_Swords_VoidSword;
}

const std::string& VoidSwordPlans::getIdentifier()
{
	return VoidSwordPlans::SaveKey;
}
