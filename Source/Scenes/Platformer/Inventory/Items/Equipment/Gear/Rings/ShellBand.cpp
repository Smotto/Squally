#include "ShellBand.h"

#include "Engine/Inventory/CurrencyInventory.h"
#include "Scenes/Platformer/Inventory/Currencies/IOU.h"
#include "Scenes/Platformer/Inventory/Items/Recipes/Gear/Rings/ShellBandRecipe.h"

#include "Resources/ItemResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

const std::string ShellBand::SaveKey = "shell-band";

ShellBand* ShellBand::create()
{
	ShellBand* instance = new ShellBand();

	instance->autorelease();

	return instance;
}

// Tier 2
ShellBand::ShellBand() : super(CurrencyInventory::create({{ IOU::getIOUIdentifier(), 35 }}), ItemStats(
	// Health
	2,
	// Mana
	4,
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

ShellBand::~ShellBand()
{
}

Item* ShellBand::clone()
{
	return ShellBand::create();
}

LocalizedString* ShellBand::getString()
{
	return Strings::Items_Equipment_Gear_Rings_ShellBand::create();
}

const std::string& ShellBand::getIconResource()
{
	return ItemResources::Equipment_Gear_Rings_ShellBand;
}

const std::string& ShellBand::getIdentifier()
{
	return ShellBand::SaveKey;
}

Recipe* ShellBand::getRecipe()
{
	return ShellBandRecipe::create();
}
