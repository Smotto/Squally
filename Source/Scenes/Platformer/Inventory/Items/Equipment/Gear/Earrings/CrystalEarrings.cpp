#include "CrystalEarrings.h"

#include "Engine/Inventory/CurrencyInventory.h"
#include "Scenes/Platformer/Inventory/Currencies/IOU.h"
#include "Scenes/Platformer/Inventory/Items/Recipes/Gear/Earrings/CrystalEarringsRecipe.h"

#include "Resources/ItemResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

const std::string CrystalEarrings::SaveKey = "crystal-earrings";

CrystalEarrings* CrystalEarrings::create()
{
	CrystalEarrings* instance = new CrystalEarrings();

	instance->autorelease();

	return instance;
}

CrystalEarrings::CrystalEarrings() : super(CurrencyInventory::create({{ IOU::getIOUIdentifier(), 23 }}), ItemStats(
	// Health
	3,
	// Mana
	7,
	// Attack
	0,
	// Magic Attack
	0,
	// Armor
	0,
	// Speed
	0.035f
))
{
}

CrystalEarrings::~CrystalEarrings()
{
}

Item* CrystalEarrings::clone()
{
	return CrystalEarrings::create();
}

LocalizedString* CrystalEarrings::getString()
{
	return Strings::Items_Equipment_Gear_Earrings_CrystalEarrings::create();
}

const std::string& CrystalEarrings::getIconResource()
{
	return ItemResources::Equipment_Gear_Earrings_T7_CrystalEarrings;
}

const std::string& CrystalEarrings::getIdentifier()
{
	return CrystalEarrings::SaveKey;
}

Recipe* CrystalEarrings::getRecipe()
{
	return CrystalEarringsRecipe::create();
}
