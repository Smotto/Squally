#include "CopperAxe.h"

#include "Engine/Inventory/CurrencyInventory.h"
#include "Scenes/Platformer/Inventory/Currencies/IOU.h"
#include "Scenes/Platformer/Inventory/Items/Recipes/Weapons/Axes/CopperAxePlans.h"

#include "Resources/ItemResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

const std::string CopperAxe::SaveKey = "copper-axe";

CopperAxe* CopperAxe::create()
{
	CopperAxe* instance = new CopperAxe();

	instance->autorelease();

	return instance;
}

CopperAxe::CopperAxe() : super(CurrencyInventory::create({{ IOU::getIOUIdentifier(), 2 }}), AXE_DAMAGE_MIN(2), AXE_DAMAGE_MAX(2), ItemStats(
	// Health
	2,
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

CopperAxe::~CopperAxe()
{
}

Item* CopperAxe::clone()
{
	return CopperAxe::create();
}

LocalizedString* CopperAxe::getString()
{
	return Strings::Items_Equipment_Weapons_Axes_CopperAxe::create();
}

const std::string& CopperAxe::getIconResource()
{
	return ItemResources::Equipment_Weapons_Axes_CopperAxe;
}

const std::string& CopperAxe::getIdentifier()
{
	return CopperAxe::SaveKey;
}

CSize CopperAxe::getWeaponCollisionSize()
{
	// As expected: Weapon width, height
	return CSize(112.0f, 96.0f);
}

Vec2 CopperAxe::getWeaponCollisionOffset()
{
	// Unintuitive: x influences y position, y influences x, likely due to initial weapon rotation
	return Vec2(-12.0f, 212.0f);
}

Vec2 CopperAxe::getDisplayOffset()
{
	return Vec2(20.0f, -4.0f);
}

Recipe* CopperAxe::getRecipe()
{
	return CopperAxePlans::create();
}
