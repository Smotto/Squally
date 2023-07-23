#include "Dreadmourn.h"

#include "Engine/Inventory/CurrencyInventory.h"
#include "Scenes/Platformer/Inventory/Currencies/IOU.h"
#include "Scenes/Platformer/Inventory/Items/Recipes/Weapons/Special/DreadmournPlans.h"

#include "Resources/ItemResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

const std::string Dreadmourn::SaveKey = "dreadmourn";

Dreadmourn* Dreadmourn::create()
{
	Dreadmourn* instance = new Dreadmourn();

	instance->autorelease();

	return instance;
}

Dreadmourn::Dreadmourn() : super(CurrencyInventory::create({{ IOU::getIOUIdentifier(), 2 }}), 40, 48, ItemStats(
	// Health
	0,
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

Dreadmourn::~Dreadmourn()
{
}

Item* Dreadmourn::clone()
{
	return Dreadmourn::create();
}

LocalizedString* Dreadmourn::getString()
{
	return Strings::Items_Equipment_Weapons_Swords_Dreadmourn::create();
}

const std::string& Dreadmourn::getIconResource()
{
	return ItemResources::Equipment_Weapons_Special_Dreadmourn;
}

const std::string& Dreadmourn::getIdentifier()
{
	return Dreadmourn::SaveKey;
}

Recipe* Dreadmourn::getRecipe()
{
	return DreadmournPlans::create();
}
