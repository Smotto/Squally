#include "QuartzWand.h"

#include "Engine/Inventory/CurrencyInventory.h"
#include "Scenes/Platformer/Inventory/Currencies/IOU.h"
#include "Scenes/Platformer/Inventory/Items/Recipes/Weapons/Wands/QuartzWandPlans.h"

#include "Resources/ItemResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

const std::string QuartzWand::SaveKey = "mana-wand";

QuartzWand* QuartzWand::create()
{
	QuartzWand* instance = new QuartzWand();

	instance->autorelease();

	return instance;
}

QuartzWand::QuartzWand() : super(CurrencyInventory::create({{ IOU::getIOUIdentifier(), 2 }}), WAND_DAMAGE_MIN(2), WAND_DAMAGE_MAX(2), ItemStats(
	// Health
	0,
	// Mana
	6,
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

QuartzWand::~QuartzWand()
{
}

Item* QuartzWand::clone()
{
	return QuartzWand::create();
}

LocalizedString* QuartzWand::getString()
{
	return Strings::Items_Equipment_Weapons_Wands_QuartzWand::create();
}

const std::string& QuartzWand::getIconResource()
{
	return ItemResources::Equipment_Weapons_Wands_QuartzWand;
}

const std::string& QuartzWand::getIdentifier()
{
	return QuartzWand::SaveKey;
}

Vec2 QuartzWand::getDisplayOffset()
{
	return Vec2(0.0f, -24.0f);
}

Recipe* QuartzWand::getRecipe()
{
	return QuartzWandPlans::create();
}
