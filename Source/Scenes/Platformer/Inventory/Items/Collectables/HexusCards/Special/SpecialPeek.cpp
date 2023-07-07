#include "SpecialPeek.h"

#include "Engine/Inventory/CurrencyInventory.h"
#include "Scenes/Platformer/Inventory/Currencies/IOU.h"
#include "Scenes/Hexus/CardData/CardKeys.h"

#include "Resources/ItemResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

const std::string SpecialPeek::SaveKey = "special-peek";

SpecialPeek* SpecialPeek::create()
{
	SpecialPeek* instance = new SpecialPeek();

	instance->autorelease();

	return instance;
}

SpecialPeek::SpecialPeek() : super(CurrencyInventory::create({{ IOU::getIOUIdentifier(), 3 }}), ItemMeta(3, 3))
{
}

SpecialPeek::~SpecialPeek()
{
}

Item* SpecialPeek::clone()
{
	return SpecialPeek::create();
}

LocalizedString* SpecialPeek::getString()
{
	return Strings::Hexus_Cards_Peek::create();
}

const std::string& SpecialPeek::getIconResource()
{
	return ItemResources::Collectables_Cards_CardSpecial;
}

const std::string& SpecialPeek::getIdentifier()
{
	return SpecialPeek::SaveKey;
}

std::string SpecialPeek::getCardKey()
{
	return CardKeys::Peek;
}
