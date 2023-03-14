#include "GoldenBow.h"

#include "Engine/Inventory/CurrencyInventory.h"
#include "Scenes/Platformer/Inventory/Currencies/IOU.h"
#include "Scenes/Platformer/Inventory/Items/Recipes/Weapons/Bows/GoldenBowPlans.h"

#include "Resources/ItemResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

const std::string GoldenBow::SaveKey = "golden-bow";

GoldenBow* GoldenBow::create()
{
	GoldenBow* instance = new GoldenBow();

	instance->autorelease();

	return instance;
}

GoldenBow::GoldenBow() : super(CurrencyInventory::create({{ IOU::getIOUIdentifier(), 2 }}), 11, 15, ItemStats(
	// Health
	0,
	// Mana
	4,
	// Attack
	0,
	// Magic Attack
	0,
	// Armor
	0,
	// Speed
	0.25f
))
{
}

GoldenBow::~GoldenBow()
{
}

Item* GoldenBow::clone()
{
	return GoldenBow::create();
}

LocalizedString* GoldenBow::getString()
{
	return Strings::Items_Equipment_Weapons_Bows_GoldenBow::create();
}

const std::string& GoldenBow::getIconResource()
{
	return ItemResources::Equipment_Weapons_Bows_GoldenBow;
}

const std::string& GoldenBow::getIdentifier()
{
	return GoldenBow::SaveKey;
}

Vec2 GoldenBow::getDisplayOffset()
{
	return Vec2(-10.0f, -74.0f);
}

Recipe* GoldenBow::getRecipe()
{
	return GoldenBowPlans::create();
}
