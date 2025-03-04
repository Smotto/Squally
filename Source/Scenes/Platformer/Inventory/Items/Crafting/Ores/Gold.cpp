#include "Gold.h"

#include "Engine/Inventory/CurrencyInventory.h"
#include "Scenes/Platformer/Inventory/Currencies/IOU.h"

#include "Resources/ItemResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

const std::string Gold::SaveKey = "gold";

Gold* Gold::create()
{
	Gold* instance = new Gold();

	instance->autorelease();

	return instance;
}

Gold::Gold() : super(CurrencyInventory::create({{ IOU::getIOUIdentifier(), 8 * 3 }}), ItemMeta(20000))
{
}

Gold::~Gold()
{
}

Item* Gold::clone()
{
	return Gold::create();
}

LocalizedString* Gold::getString()
{
	return Strings::Items_Crafting_Ore_Gold::create();
}

const std::string& Gold::getIconResource()
{
	return ItemResources::Crafting_Smithing_T3_Gold;
}

const std::string& Gold::getIdentifier()
{
	return Gold::SaveKey;
}
