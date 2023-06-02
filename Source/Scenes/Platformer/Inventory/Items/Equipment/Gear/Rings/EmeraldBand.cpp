#include "EmeraldBand.h"

#include "Engine/Inventory/CurrencyInventory.h"
#include "Scenes/Platformer/Inventory/Currencies/IOU.h"
#include "Scenes/Platformer/Inventory/Items/Recipes/Gear/Rings/EmeraldBandRecipe.h"

#include "Resources/ItemResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

const std::string EmeraldBand::SaveKey = "emerald-band";

EmeraldBand* EmeraldBand::create()
{
	EmeraldBand* instance = new EmeraldBand();

	instance->autorelease();

	return instance;
}

// Tier 4
EmeraldBand::EmeraldBand() : super(CurrencyInventory::create({{ IOU::getIOUIdentifier(), 55 }}), ItemStats(
	// Health
	4,
	// Mana
	6,
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

EmeraldBand::~EmeraldBand()
{
}

Item* EmeraldBand::clone()
{
	return EmeraldBand::create();
}

LocalizedString* EmeraldBand::getString()
{
	return Strings::Items_Equipment_Gear_Rings_EmeraldBand::create();
}

const std::string& EmeraldBand::getIconResource()
{
	return ItemResources::Equipment_Gear_Rings_EmeraldBand;
}

const std::string& EmeraldBand::getIdentifier()
{
	return EmeraldBand::SaveKey;
}

Recipe* EmeraldBand::getRecipe()
{
	return EmeraldBandRecipe::create();
}
