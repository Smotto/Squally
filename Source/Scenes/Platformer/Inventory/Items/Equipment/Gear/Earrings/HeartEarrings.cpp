#include "HeartEarrings.h"

#include "Engine/Inventory/CurrencyInventory.h"
#include "Scenes/Platformer/Inventory/Currencies/IOU.h"
#include "Scenes/Platformer/Inventory/Items/Recipes/Gear/Earrings/HeartEarringsRecipe.h"

#include "Resources/ItemResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

const std::string HeartEarrings::SaveKey = "heart-earrings";

HeartEarrings* HeartEarrings::create()
{
	HeartEarrings* instance = new HeartEarrings();

	instance->autorelease();

	return instance;
}

HeartEarrings::HeartEarrings() : super(CurrencyInventory::create({{ IOU::getIOUIdentifier(), 23 }}), ItemStats(
	// Health
	2,
	// Mana
	6,
	// Attack
	0,
	// Magic Attack
	0,
	// Armor
	0,
	// Speed
	0.03f
))
{
}

HeartEarrings::~HeartEarrings()
{
}

Item* HeartEarrings::clone()
{
	return HeartEarrings::create();
}

LocalizedString* HeartEarrings::getString()
{
	return Strings::Items_Equipment_Gear_Earrings_HeartEarrings::create();
}

const std::string& HeartEarrings::getIconResource()
{
	return ItemResources::Equipment_Gear_Earrings_T6_HeartEarrings;
}

const std::string& HeartEarrings::getIdentifier()
{
	return HeartEarrings::SaveKey;
}

Recipe* HeartEarrings::getRecipe()
{
	return HeartEarringsRecipe::create();
}
