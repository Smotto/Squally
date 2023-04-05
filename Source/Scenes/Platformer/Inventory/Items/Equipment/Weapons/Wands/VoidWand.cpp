#include "VoidWand.h"

#include "Engine/Inventory/CurrencyInventory.h"
#include "Scenes/Platformer/Inventory/Currencies/IOU.h"
#include "Scenes/Platformer/Inventory/Items/Recipes/Weapons/Wands/VoidWandPlans.h"

#include "Resources/ItemResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

const std::string VoidWand::SaveKey = "void-wand";

VoidWand* VoidWand::create()
{
	VoidWand* instance = new VoidWand();

	instance->autorelease();

	return instance;
}

VoidWand::VoidWand() : super(CurrencyInventory::create({{ IOU::getIOUIdentifier(), 2 }}), 24, 28, ItemStats(
	// Health
	0,
	// Mana
	25,
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

VoidWand::~VoidWand()
{
}

Item* VoidWand::clone()
{
	return VoidWand::create();
}

LocalizedString* VoidWand::getString()
{
	return Strings::Items_Equipment_Weapons_Wands_VoidWand::create();
}

const std::string& VoidWand::getIconResource()
{
	return ItemResources::Equipment_Weapons_Wands_VoidWand;
}

const std::string& VoidWand::getIdentifier()
{
	return VoidWand::SaveKey;
}

Vec2 VoidWand::getDisplayOffset()
{
	return Vec2(0.0f, -24.0f);
}

Recipe* VoidWand::getRecipe()
{
	return VoidWandPlans::create();
}
