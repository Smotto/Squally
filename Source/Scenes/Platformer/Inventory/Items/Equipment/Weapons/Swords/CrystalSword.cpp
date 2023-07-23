#include "CrystalSword.h"

#include "Engine/Inventory/CurrencyInventory.h"
#include "Scenes/Platformer/Inventory/Currencies/IOU.h"
#include "Scenes/Platformer/Inventory/Items/Recipes/Weapons/Swords/CrystalSwordPlans.h"

#include "Resources/ItemResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

const std::string CrystalSword::SaveKey = "crystal-sword";

CrystalSword* CrystalSword::create()
{
	CrystalSword* instance = new CrystalSword();

	instance->autorelease();

	return instance;
}

CrystalSword::CrystalSword() : super(CurrencyInventory::create({{ IOU::getIOUIdentifier(), 2 }}), SWORD_DAMAGE_MIN(8), SWORD_DAMAGE_MAX(8), ItemStats(
	// Health
	4,
	// Mana
	4,
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

CrystalSword::~CrystalSword()
{
}

Item* CrystalSword::clone()
{
	return CrystalSword::create();
}

LocalizedString* CrystalSword::getString()
{
	return Strings::Items_Equipment_Weapons_Swords_CrystalSword::create();
}

const std::string& CrystalSword::getIconResource()
{
	return ItemResources::Equipment_Weapons_Swords_CrystalSword;
}

const std::string& CrystalSword::getIdentifier()
{
	return CrystalSword::SaveKey;
}

Recipe* CrystalSword::getRecipe()
{
	return CrystalSwordPlans::create();
}
