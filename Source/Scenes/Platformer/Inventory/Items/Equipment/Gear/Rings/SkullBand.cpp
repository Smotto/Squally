#include "SkullBand.h"

#include "Engine/Inventory/CurrencyInventory.h"
#include "Scenes/Platformer/Inventory/Currencies/IOU.h"
#include "Scenes/Platformer/Inventory/Items/Recipes/Gear/Rings/SkullBandRecipe.h"

#include "Resources/ItemResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

const std::string SkullBand::SaveKey = "skull-band";

SkullBand* SkullBand::create()
{
	SkullBand* instance = new SkullBand();

	instance->autorelease();

	return instance;
}

// Tier 5
SkullBand::SkullBand() : super(CurrencyInventory::create({{ IOU::getIOUIdentifier(), 65 }}), ItemStats(
	// Health
	5,
	// Mana
	7,
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

SkullBand::~SkullBand()
{
}

Item* SkullBand::clone()
{
	return SkullBand::create();
}

LocalizedString* SkullBand::getString()
{
	return Strings::Items_Equipment_Gear_Rings_SkullBand::create();
}

const std::string& SkullBand::getIconResource()
{
	return ItemResources::Equipment_Gear_Rings_T5_SkullBand;
}

const std::string& SkullBand::getIdentifier()
{
	return SkullBand::SaveKey;
}

Recipe* SkullBand::getRecipe()
{
	return SkullBandRecipe::create();
}
