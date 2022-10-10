#include "Decimal5.h"

#include "Engine/Inventory/CurrencyInventory.h"
#include "Scenes/Platformer/Inventory/Currencies/IOU.h"
#include "Scenes/Hexus/CardData/CardKeys.h"

#include "Resources/ItemResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

const std::string Decimal5::SaveKey = "decimal-5";

Decimal5* Decimal5::create()
{
	Decimal5* instance = new Decimal5();

	instance->autorelease();

	return instance;
}

Decimal5::Decimal5() : super(CurrencyInventory::create({{ IOU::getIOUIdentifier(), HexusCard::TuneCost(5) }}), ItemMeta(3, 3))
{
}

Decimal5::~Decimal5()
{
}

Item* Decimal5::clone()
{
	return Decimal5::create();
}

LocalizedString* Decimal5::getString()
{
	return Strings::Hexus_Cards_Decimal5::create();
}

const std::string& Decimal5::getIconResource()
{
	return ItemResources::Collectables_Cards_CardDecimal;
}

const std::string& Decimal5::getIdentifier()
{
	return Decimal5::SaveKey;
}

std::string Decimal5::getCardKey()
{
	return CardKeys::Decimal5;
}
