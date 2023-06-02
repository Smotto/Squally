#include "MarineBand.h"

#include "Engine/Inventory/CurrencyInventory.h"
#include "Scenes/Platformer/Inventory/Currencies/IOU.h"
#include "Scenes/Platformer/Inventory/Items/Recipes/Gear/Rings/MarineBandRecipe.h"

#include "Resources/ItemResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

const std::string MarineBand::SaveKey = "marine-band";

MarineBand* MarineBand::create()
{
	MarineBand* instance = new MarineBand();

	instance->autorelease();

	return instance;
}

// Tier 3
MarineBand::MarineBand() : super(CurrencyInventory::create({{ IOU::getIOUIdentifier(), 45 }}), ItemStats(
	// Health
	3,
	// Mana
	5,
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

MarineBand::~MarineBand()
{
}

Item* MarineBand::clone()
{
	return MarineBand::create();
}

LocalizedString* MarineBand::getString()
{
	return Strings::Items_Equipment_Gear_Rings_MarineBand::create();
}

const std::string& MarineBand::getIconResource()
{
	return ItemResources::Equipment_Gear_Rings_MarineBand;
}

const std::string& MarineBand::getIdentifier()
{
	return MarineBand::SaveKey;
}

Recipe* MarineBand::getRecipe()
{
	return MarineBandRecipe::create();
}
