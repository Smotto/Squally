#include "DemonicMace.h"

#include "Engine/Inventory/CurrencyInventory.h"
#include "Scenes/Platformer/Inventory/Currencies/IOU.h"
#include "Scenes/Platformer/Inventory/Items/Recipes/Weapons/Maces/DemonicMacePlans.h"

#include "Resources/ItemResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

const std::string DemonicMace::SaveKey = "demonic-mace";

DemonicMace* DemonicMace::create()
{
	DemonicMace* instance = new DemonicMace();

	instance->autorelease();

	return instance;
}

DemonicMace::DemonicMace() : super(CurrencyInventory::create({{ IOU::getIOUIdentifier(), 2 }}), 32, 40, ItemStats(
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

DemonicMace::~DemonicMace()
{
}

Item* DemonicMace::clone()
{
	return DemonicMace::create();
}

const std::string& DemonicMace::getItemName()
{
	return DemonicMace::SaveKey;
}

LocalizedString* DemonicMace::getString()
{
	return Strings::Items_Equipment_Weapons_Maces_DemonicMace::create();
}

const std::string& DemonicMace::getIconResource()
{
	return ItemResources::Equipment_Weapons_Maces_DemonicMace;
}

const std::string& DemonicMace::getIdentifier()
{
	return DemonicMace::SaveKey;
}

Vec2 DemonicMace::getDisplayOffset()
{
	return Vec2(0.0f, -24.0f);
}

Recipe* DemonicMace::getRecipe()
{
	return DemonicMacePlans::create();
}
