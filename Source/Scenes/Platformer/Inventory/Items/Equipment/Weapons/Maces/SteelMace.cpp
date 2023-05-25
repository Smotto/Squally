#include "SteelMace.h"

#include "Engine/Inventory/CurrencyInventory.h"
#include "Scenes/Platformer/Inventory/Currencies/IOU.h"
#include "Scenes/Platformer/Inventory/Items/Recipes/Weapons/Maces/SteelMacePlans.h"

#include "Resources/ItemResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

const std::string SteelMace::SaveKey = "steel-mace";

SteelMace* SteelMace::create()
{
	SteelMace* instance = new SteelMace();

	instance->autorelease();

	return instance;
}

SteelMace::SteelMace() : super(CurrencyInventory::create({{ IOU::getIOUIdentifier(), 2 }}), MACE_DAMAGE_MIN(3), MACE_DAMAGE_MAX(3), ItemStats(
	// Health
	1,
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

SteelMace::~SteelMace()
{
}

Item* SteelMace::clone()
{
	return SteelMace::create();
}

LocalizedString* SteelMace::getString()
{
	return Strings::Items_Equipment_Weapons_Maces_SteelMace::create();
}

const std::string& SteelMace::getIconResource()
{
	return ItemResources::Equipment_Weapons_Maces_SteelMace;
}

const std::string& SteelMace::getIdentifier()
{
	return SteelMace::SaveKey;
}

Vec2 SteelMace::getDisplayOffset()
{
	return Vec2(0.0f, -24.0f);
}

Recipe* SteelMace::getRecipe()
{
	return SteelMacePlans::create();
}
