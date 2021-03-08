#include "Binary12.h"

#include "Engine/Inventory/CurrencyInventory.h"
#include "Scenes/Platformer/Inventory/Currencies/IOU.h"
#include "Scenes/Hexus/CardData/CardKeys.h"

#include "Resources/ItemResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

const std::string Binary12::SaveKey = "binary-12";

Binary12* Binary12::create()
{
	Binary12* instance = new Binary12();

	instance->autorelease();

	return instance;
}

Binary12::Binary12() : super(CurrencyInventory::create({{ IOU::getIOUIdentifier(), 14 }}), ItemMeta(3, 3))
{
}

Binary12::~Binary12()
{
}

Item* Binary12::clone()
{
	return Binary12::create();
}

const std::string& Binary12::getItemName()
{
	return Binary12::SaveKey;
}

LocalizedString* Binary12::getString()
{
	return Strings::Hexus_Cards_Binary12::create();
}

const std::string& Binary12::getIconResource()
{
	return ItemResources::Collectables_Cards_CardBinary;
}

const std::string& Binary12::getIdentifier()
{
	return Binary12::SaveKey;
}

std::string Binary12::getCardKey()
{
	return CardKeys::Binary12;
}
