#include "WitchesWand.h"

#include "Engine/Inventory/CurrencyInventory.h"
#include "Scenes/Platformer/Inventory/Currencies/IOU.h"
#include "Scenes/Platformer/Inventory/Items/Recipes/Weapons/Wands/WitchesWandPlans.h"

#include "Resources/ItemResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

const std::string WitchesWand::SaveKey = "witches-wand";

WitchesWand* WitchesWand::create()
{
	WitchesWand* instance = new WitchesWand();

	instance->autorelease();

	return instance;
}

WitchesWand::WitchesWand() : super(CurrencyInventory::create({{ IOU::getIOUIdentifier(), 2 }}), 24, 28, ItemStats(
	// Health
	0,
	// Mana
	16,
	// Attack
	0,
	// Magic Attack
	0,
	// Armor
	0,
	// Speed
	0.15f
))
{
}

WitchesWand::~WitchesWand()
{
}

Item* WitchesWand::clone()
{
	return WitchesWand::create();
}

LocalizedString* WitchesWand::getString()
{
	return Strings::Items_Equipment_Weapons_Wands_WitchesWand::create();
}

const std::string& WitchesWand::getIconResource()
{
	return ItemResources::Equipment_Weapons_Special_WitchesWand;
}

const std::string& WitchesWand::getIdentifier()
{
	return WitchesWand::SaveKey;
}

Vec2 WitchesWand::getDisplayOffset()
{
	return Vec2(0.0f, -24.0f);
}

Recipe* WitchesWand::getRecipe()
{
	return WitchesWandPlans::create();
}
