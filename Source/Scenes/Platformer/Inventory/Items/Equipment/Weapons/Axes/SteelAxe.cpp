#include "SteelAxe.h"

#include "Engine/Inventory/CurrencyInventory.h"
#include "Scenes/Platformer/Inventory/Currencies/IOU.h"
#include "Scenes/Platformer/Inventory/Items/Recipes/Weapons/Axes/SteelAxePlans.h"

#include "Resources/ItemResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

const std::string SteelAxe::SaveKey = "steel-axe";

SteelAxe* SteelAxe::create()
{
	SteelAxe* instance = new SteelAxe();

	instance->autorelease();

	return instance;
}

SteelAxe::SteelAxe() : super(CurrencyInventory::create({{ IOU::getIOUIdentifier(), 2 }}), AXE_DAMAGE_MIN(3), AXE_DAMAGE_MAX(3), ItemStats(
	// Health
	1,
	// Mana
	2,
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

SteelAxe::~SteelAxe()
{
}

Item* SteelAxe::clone()
{
	return SteelAxe::create();
}

LocalizedString* SteelAxe::getString()
{
	return Strings::Items_Equipment_Weapons_Axes_SteelAxe::create();
}

const std::string& SteelAxe::getIconResource()
{
	return ItemResources::Equipment_Weapons_Axes_SteelAxe;
}

const std::string& SteelAxe::getIdentifier()
{
	return SteelAxe::SaveKey;
}

CSize SteelAxe::getWeaponCollisionSize()
{
	// As expected: Weapon width, height
	return CSize(112.0f, 96.0f);
}

Vec2 SteelAxe::getWeaponCollisionOffset()
{
	// Unintuitive: x influences y position, y influences x, likely due to initial weapon rotation
	return Vec2(0.0f, 212.0f);
}

Vec2 SteelAxe::getDisplayOffset()
{
	return Vec2(2.0f, -20.0f);
}

Recipe* SteelAxe::getRecipe()
{
	return SteelAxePlans::create();
}
