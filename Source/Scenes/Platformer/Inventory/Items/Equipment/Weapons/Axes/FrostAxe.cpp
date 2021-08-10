#include "FrostAxe.h"

#include "Engine/Inventory/CurrencyInventory.h"
#include "Scenes/Platformer/Inventory/Currencies/IOU.h"
#include "Scenes/Platformer/Inventory/Items/Recipes/Weapons/Axes/FrostAxePlans.h"

#include "Resources/ItemResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

const std::string FrostAxe::SaveKey = "frost-axe";

FrostAxe* FrostAxe::create()
{
	FrostAxe* instance = new FrostAxe();

	instance->autorelease();

	return instance;
}

FrostAxe::FrostAxe() : super(CurrencyInventory::create({{ IOU::getIOUIdentifier(), 2 }}), 4, 7, ItemStats(
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
	0.10f
))
{
}

FrostAxe::~FrostAxe()
{
}

Item* FrostAxe::clone()
{
	return FrostAxe::create();
}

LocalizedString* FrostAxe::getString()
{
	return Strings::Items_Equipment_Weapons_Axes_FrostAxe::create();
}

const std::string& FrostAxe::getIconResource()
{
	return ItemResources::Equipment_Weapons_Special_FrostAxe;
}

const std::string& FrostAxe::getIdentifier()
{
	return FrostAxe::SaveKey;
}

CSize FrostAxe::getWeaponCollisionSize()
{
	// As expected: Weapon width, height
	return CSize(112.0f, 96.0f);
}

Vec2 FrostAxe::getWeaponCollisionOffset()
{
	// Unintuitive: x influences y position, y influences x, likely due to initial weapon rotation
	return Vec2(0.0f, 212.0f);
}

Vec2 FrostAxe::getDisplayOffset()
{
	return Vec2(28.0f, -4.0f);
}

Recipe* FrostAxe::getRecipe()
{
	return FrostAxePlans::create();
}
