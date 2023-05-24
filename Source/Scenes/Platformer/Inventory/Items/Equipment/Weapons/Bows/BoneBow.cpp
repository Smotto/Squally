#include "BoneBow.h"

#include "Engine/Inventory/CurrencyInventory.h"
#include "Scenes/Platformer/Inventory/Currencies/IOU.h"
#include "Scenes/Platformer/Inventory/Items/Recipes/Weapons/Bows/BoneBowPlans.h"

#include "Resources/ItemResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

const std::string BoneBow::SaveKey = "bone-bow";

BoneBow* BoneBow::create()
{
	BoneBow* instance = new BoneBow();

	instance->autorelease();

	return instance;
}

BoneBow::BoneBow() : super(CurrencyInventory::create({{ IOU::getIOUIdentifier(), 2 }}), BOW_DAMAGE_MIN(6), BOW_DAMAGE_MAX(6), ItemStats(
	// Health
	0,
	// Mana
	5,
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

BoneBow::~BoneBow()
{
}

Item* BoneBow::clone()
{
	return BoneBow::create();
}

LocalizedString* BoneBow::getString()
{
	return Strings::Items_Equipment_Weapons_Bows_BoneBow::create();
}

const std::string& BoneBow::getIconResource()
{
	return ItemResources::Equipment_Weapons_Bows_BoneBow;
}

const std::string& BoneBow::getIdentifier()
{
	return BoneBow::SaveKey;
}

Vec2 BoneBow::getDisplayOffset()
{
	return Vec2(0.0f, -24.0f);
}

Recipe* BoneBow::getRecipe()
{
	return BoneBowPlans::create();
}
