#include "Copper.h"

#include "Engine/Inventory/CurrencyInventory.h"
#include "Scenes/Platformer/Inventory/Currencies/IOU.h"

#include "Resources/ItemResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

const std::string Copper::SaveKey = "copper";

Copper* Copper::create()
{
	Copper* instance = new Copper();

	instance->autorelease();

	return instance;
}

Copper::Copper() : super(CurrencyInventory::create({{ IOU::getIOUIdentifier(), 14 }}), ItemMeta(20000))
{
}

Copper::~Copper()
{
}

Item* Copper::clone()
{
	return Copper::create();
}

LocalizedString* Copper::getString()
{
	return Strings::Items_Crafting_Ore_Copper::create();
}

const std::string& Copper::getIconResource()
{
	return ItemResources::Crafting_Smithing_T1_Copper;
}

const std::string& Copper::getIdentifier()
{
	return Copper::SaveKey;
}
