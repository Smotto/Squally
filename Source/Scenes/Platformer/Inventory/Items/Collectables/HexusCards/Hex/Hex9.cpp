#include "Hex9.h"

#include "Engine/Inventory/CurrencyInventory.h"
#include "Scenes/Platformer/Inventory/Currencies/IOU.h"
#include "Scenes/Hexus/CardData/CardKeys.h"

#include "Resources/ItemResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

const std::string Hex9::SaveKey = "hex-9";

Hex9* Hex9::create()
{
	Hex9* instance = new Hex9();

	instance->autorelease();

	return instance;
}

Hex9::Hex9() : super(CurrencyInventory::create({{ IOU::getIOUIdentifier(), HexusCard::TuneCost(9) }}), ItemMeta(3, 3))
{
}

Hex9::~Hex9()
{
}

Item* Hex9::clone()
{
	return Hex9::create();
}

LocalizedString* Hex9::getString()
{
	return Strings::Hexus_Cards_Hex9::create();
}

const std::string& Hex9::getIconResource()
{
	return ItemResources::Collectables_Cards_CardHex;
}

const std::string& Hex9::getIdentifier()
{
	return Hex9::SaveKey;
}

std::string Hex9::getCardKey()
{
	return CardKeys::Hex9;
}
