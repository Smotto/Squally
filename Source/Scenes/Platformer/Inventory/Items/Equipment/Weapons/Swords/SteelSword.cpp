#include "SteelSword.h"

#include "Engine/Inventory/CurrencyInventory.h"
#include "Scenes/Platformer/Inventory/Currencies/IOU.h"
#include "Scenes/Platformer/Inventory/Items/Recipes/Weapons/Swords/SteelSwordPlans.h"

#include "Resources/ItemResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

const std::string SteelSword::SaveKey = "steel-sword";

SteelSword* SteelSword::create()
{
	SteelSword* instance = new SteelSword();

	instance->autorelease();

	return instance;
}

SteelSword::SteelSword() : super(CurrencyInventory::create({{ IOU::getIOUIdentifier(), 6 }}), SWORD_DAMAGE_MIN(3), SWORD_DAMAGE_MAX(3), ItemStats(
	// Health
	3,
	// Mana
	0,
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

SteelSword::~SteelSword()
{
}

Item* SteelSword::clone()
{
	return SteelSword::create();
}

LocalizedString* SteelSword::getString()
{
	return Strings::Items_Equipment_Weapons_Swords_SteelSword::create();
}

const std::string& SteelSword::getIconResource()
{
	return ItemResources::Equipment_Weapons_Swords_SteelSword;
}

const std::string& SteelSword::getIdentifier()
{
	return SteelSword::SaveKey;
}

Recipe* SteelSword::getRecipe()
{
	return SteelSwordPlans::create();
}
