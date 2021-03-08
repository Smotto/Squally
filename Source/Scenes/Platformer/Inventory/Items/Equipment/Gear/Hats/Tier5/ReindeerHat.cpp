#include "ReindeerHat.h"

#include "Engine/Inventory/CurrencyInventory.h"
#include "Scenes/Platformer/Inventory/Currencies/IOU.h"

#include "Resources/ItemResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

const std::string ReindeerHat::SaveKey = "reindeer-hat";

ReindeerHat* ReindeerHat::create()
{
	ReindeerHat* instance = new ReindeerHat();

	instance->autorelease();

	return instance;
}

ReindeerHat::ReindeerHat() : super(CurrencyInventory::create({{ IOU::getIOUIdentifier(), 2 }}), ItemStats(
	// Health
	0,
	// Mana
	0,
	// Attack
	0,
	// Magic Attack
	0,
	// Armor
	0,
	// Speed
	0.05f
))
{
}

ReindeerHat::~ReindeerHat()
{
}

Item* ReindeerHat::clone()
{
	return ReindeerHat::create();
}

const std::string& ReindeerHat::getItemName()
{
	return ReindeerHat::SaveKey;
}

LocalizedString* ReindeerHat::getString()
{
	return Strings::Items_Equipment_Gear_Hats_ReindeerHat::create();
}

const std::string& ReindeerHat::getIconResource()
{
	return ItemResources::Equipment_Gear_Hats_ReindeerHat;
}

const std::string& ReindeerHat::getIdentifier()
{
	return ReindeerHat::SaveKey;
}

Vec2 ReindeerHat::getDisplayOffset()
{
	return Vec2(-12.0f, 6.0f);
}
