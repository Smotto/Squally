#include "PirateHat.h"

#include "Engine/Inventory/CurrencyInventory.h"
#include "Scenes/Platformer/Inventory/Currencies/IOU.h"
#include "Scenes/Platformer/Inventory/Items/Recipes/Gear/Hats/Special/PirateHatPlans.h"

#include "Resources/ItemResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

const std::string PirateHat::SaveKey = "pirate-hat";

PirateHat* PirateHat::create()
{
	PirateHat* instance = new PirateHat();

	instance->autorelease();

	return instance;
}

PirateHat::PirateHat() : super(CurrencyInventory::create({{ IOU::getIOUIdentifier(), 2 }}), ItemStats(
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

PirateHat::~PirateHat()
{
}

Item* PirateHat::clone()
{
	return PirateHat::create();
}

LocalizedString* PirateHat::getString()
{
	return Strings::Items_Equipment_Gear_Hats_PirateHat::create();
}

const std::string& PirateHat::getIconResource()
{
	return ItemResources::Equipment_Gear_Hats_Special_PirateHat;
}

const std::string& PirateHat::getIdentifier()
{
	return PirateHat::SaveKey;
}

Vec2 PirateHat::getDisplayOffset()
{
	return Vec2(-12.0f, 2.0f);
}

Recipe* PirateHat::getRecipe()
{
	return PirateHatPlans::create();
}
