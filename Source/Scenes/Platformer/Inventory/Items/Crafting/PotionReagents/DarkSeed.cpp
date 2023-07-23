#include "DarkSeed.h"

#include "Engine/Inventory/CurrencyInventory.h"
#include "Scenes/Platformer/Inventory/Currencies/IOU.h"

#include "Resources/ItemResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

const std::string DarkSeed::SaveKey = "dark-seed";

DarkSeed* DarkSeed::create()
{
	DarkSeed* instance = new DarkSeed();

	instance->autorelease();

	return instance;
}

DarkSeed::DarkSeed() : super(CurrencyInventory::create({{ IOU::getIOUIdentifier(), 3 }}), ItemMeta(20000))
{
}

DarkSeed::~DarkSeed()
{
}

Item* DarkSeed::clone()
{
	return DarkSeed::create();
}

LocalizedString* DarkSeed::getString()
{
	return Strings::Items_Crafting_PotionReagents_DarkSeed::create();
}

const std::string& DarkSeed::getIconResource()
{
	return ItemResources::Crafting_Alchemy_Potions_T3_DarkSeed;
}

const std::string& DarkSeed::getIdentifier()
{
	return DarkSeed::SaveKey;
}
