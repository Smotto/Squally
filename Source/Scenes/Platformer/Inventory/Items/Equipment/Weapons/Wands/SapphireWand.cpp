#include "SapphireWand.h"

#include "Engine/Inventory/CurrencyInventory.h"
#include "Scenes/Platformer/Inventory/Currencies/IOU.h"
#include "Scenes/Platformer/Inventory/Items/Recipes/Weapons/Wands/SapphireWandPlans.h"

#include "Resources/ItemResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

const std::string SapphireWand::SaveKey = "sapphire-wand";

SapphireWand* SapphireWand::create()
{
	SapphireWand* instance = new SapphireWand();

	instance->autorelease();

	return instance;
}

SapphireWand::SapphireWand() : super(CurrencyInventory::create({{ IOU::getIOUIdentifier(), 2 }}), 9, 10, ItemStats(
	// Health
	0,
	// Mana
	14,
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

SapphireWand::~SapphireWand()
{
}

Item* SapphireWand::clone()
{
	return SapphireWand::create();
}

LocalizedString* SapphireWand::getString()
{
	return Strings::Items_Equipment_Weapons_Wands_SapphireWand::create();
}

const std::string& SapphireWand::getIconResource()
{
	return ItemResources::Equipment_Weapons_Wands_SapphireWand;
}

const std::string& SapphireWand::getIdentifier()
{
	return SapphireWand::SaveKey;
}

Vec2 SapphireWand::getDisplayOffset()
{
	return Vec2(0.0f, -24.0f);
}

Recipe* SapphireWand::getRecipe()
{
	return SapphireWandPlans::create();
}
