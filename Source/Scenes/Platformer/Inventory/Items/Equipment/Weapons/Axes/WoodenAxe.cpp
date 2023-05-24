#include "WoodenAxe.h"

#include "Engine/Inventory/CurrencyInventory.h"
#include "Scenes/Platformer/Inventory/Currencies/IOU.h"
#include "Scenes/Platformer/Inventory/Items/Recipes/Weapons/Axes/WoodenAxePlans.h"

#include "Resources/ItemResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

const std::string WoodenAxe::SaveKey = "wooden-axe";

WoodenAxe* WoodenAxe::create()
{
	WoodenAxe* instance = new WoodenAxe();

	instance->autorelease();

	return instance;
}

WoodenAxe::WoodenAxe() : super(CurrencyInventory::create({{ IOU::getIOUIdentifier(), 2 }}), AXE_DAMAGE_MIN(1), AXE_DAMAGE_MAX(1), ItemStats(
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
	0.10f
))
{
}

WoodenAxe::~WoodenAxe()
{
}

Item* WoodenAxe::clone()
{
	return WoodenAxe::create();
}

LocalizedString* WoodenAxe::getString()
{
	return Strings::Items_Equipment_Weapons_Axes_WoodenAxe::create();
}

const std::string& WoodenAxe::getIconResource()
{
	return ItemResources::Equipment_Weapons_Axes_WoodenAxe;
}

const std::string& WoodenAxe::getIdentifier()
{
	return WoodenAxe::SaveKey;
}

CSize WoodenAxe::getWeaponCollisionSize()
{
	// As expected: Weapon width, height
	return CSize(140.0f, 92.0f);
}

Vec2 WoodenAxe::getWeaponCollisionOffset()
{
	// Unintuitive: x influences y position, y influences x, likely due to initial weapon rotation
	return Vec2(0.0f, 212.0f);
}

Vec2 WoodenAxe::getDisplayOffset()
{
	return Vec2(28.0f, -4.0f);
}

Recipe* WoodenAxe::getRecipe()
{
	return WoodenAxePlans::create();
}
