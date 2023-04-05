#include "ChieftainsWand.h"

#include "Engine/Inventory/CurrencyInventory.h"
#include "Scenes/Platformer/Inventory/Currencies/IOU.h"
#include "Scenes/Platformer/Inventory/Items/Recipes/Weapons/Wands/ChieftainsWandPlans.h"

#include "Resources/ItemResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

const std::string ChieftainsWand::SaveKey = "chieftains-wand";

ChieftainsWand* ChieftainsWand::create()
{
	ChieftainsWand* instance = new ChieftainsWand();

	instance->autorelease();

	return instance;
}

ChieftainsWand::ChieftainsWand() : super(CurrencyInventory::create({{ IOU::getIOUIdentifier(), 2 }}), 20, 24, ItemStats(
	// Health
	0,
	// Mana
	12,
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

ChieftainsWand::~ChieftainsWand()
{
}

Item* ChieftainsWand::clone()
{
	return ChieftainsWand::create();
}

LocalizedString* ChieftainsWand::getString()
{
	return Strings::Items_Equipment_Weapons_Wands_ChieftainsWand::create();
}

const std::string& ChieftainsWand::getIconResource()
{
	return ItemResources::Equipment_Weapons_Special_ChieftainsWand;
}

const std::string& ChieftainsWand::getIdentifier()
{
	return ChieftainsWand::SaveKey;
}

Vec2 ChieftainsWand::getDisplayOffset()
{
	return Vec2(0.0f, -24.0f);
}

Recipe* ChieftainsWand::getRecipe()
{
	return ChieftainsWandPlans::create();
}
