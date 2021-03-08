#include "Hex5.h"

#include "Engine/Inventory/CurrencyInventory.h"
#include "Scenes/Platformer/Inventory/Currencies/IOU.h"
#include "Scenes/Hexus/CardData/CardKeys.h"

#include "Resources/ItemResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

const std::string Hex5::SaveKey = "hex-5";

Hex5* Hex5::create()
{
	Hex5* instance = new Hex5();

	instance->autorelease();

	return instance;
}

Hex5::Hex5() : super(CurrencyInventory::create({{ IOU::getIOUIdentifier(), 6 }}), ItemMeta(3, 3))
{
}

Hex5::~Hex5()
{
}

Item* Hex5::clone()
{
	return Hex5::create();
}

const std::string& Hex5::getItemName()
{
	return Hex5::SaveKey;
}

LocalizedString* Hex5::getString()
{
	return Strings::Hexus_Cards_Hex5::create();
}

const std::string& Hex5::getIconResource()
{
	return ItemResources::Collectables_Cards_CardHex;
}

const std::string& Hex5::getIdentifier()
{
	return Hex5::SaveKey;
}

std::string Hex5::getCardKey()
{
	return CardKeys::Hex5;
}
