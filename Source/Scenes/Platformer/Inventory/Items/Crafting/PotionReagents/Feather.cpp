#include "Feather.h"

#include "Engine/Inventory/CurrencyInventory.h"
#include "Scenes/Platformer/Inventory/Currencies/IOU.h"

#include "Resources/ItemResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

const std::string Feather::SaveKey = "feather";

Feather* Feather::create()
{
	Feather* instance = new Feather();

	instance->autorelease();

	return instance;
}

Feather::Feather() : super(CurrencyInventory::create({{ IOU::getIOUIdentifier(), 3 }}), ItemMeta(20000))
{
}

Feather::~Feather()
{
}

Item* Feather::clone()
{
	return Feather::create();
}

LocalizedString* Feather::getString()
{
	return Strings::Items_Crafting_PotionReagents_Feather::create();
}

const std::string& Feather::getIconResource()
{
	return ItemResources::Crafting_Alchemy_Potions_T1_Feather;
}

const std::string& Feather::getIdentifier()
{
	return Feather::SaveKey;
}
