#include "WoodenShield.h"

#include "Engine/Inventory/CurrencyInventory.h"
#include "Scenes/Platformer/Inventory/Currencies/IOU.h"

#include "Resources/ItemResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

const std::string WoodenShield::SaveKey = "wooden-shield";

WoodenShield* WoodenShield::create()
{
	WoodenShield* instance = new WoodenShield();

	instance->autorelease();

	return instance;
}

WoodenShield::WoodenShield() : super(CurrencyInventory::create({{ IOU::getIOUIdentifier(), 2 }}), 3, 6, ItemStats(
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
	0.05f
))
{
}

WoodenShield::~WoodenShield()
{
}

Item* WoodenShield::clone()
{
	return WoodenShield::create();
}

const std::string& WoodenShield::getItemName()
{
	return WoodenShield::SaveKey;
}

LocalizedString* WoodenShield::getString()
{
	return Strings::Items_Equipment_Offhands_Shields_WoodenShield::create();
}

const std::string& WoodenShield::getIconResource()
{
	return ItemResources::Equipment_Gear_Hats_WoodenPikeHelm;
}

const std::string& WoodenShield::getSerializationKey()
{
	return WoodenShield::SaveKey;
}
