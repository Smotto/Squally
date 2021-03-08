#include "GlowingPendant.h"

#include "Engine/Inventory/CurrencyInventory.h"
#include "Scenes/Platformer/Inventory/Currencies/IOU.h"

#include "Resources/ItemResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

const std::string GlowingPendant::SaveKey = "glowing-pendant";

GlowingPendant* GlowingPendant::create()
{
	GlowingPendant* instance = new GlowingPendant();

	instance->autorelease();

	return instance;
}

GlowingPendant::GlowingPendant() : super(CurrencyInventory::create({{ IOU::getIOUIdentifier(), 15 }}), ItemStats(
	// Health
	2,
	// Mana
	2,
	// Attack
	1,
	// Magic Attack
	1,
	// Armor
	0,
	// Speed
	0.05f
))
{
}

GlowingPendant::~GlowingPendant()
{
}

Item* GlowingPendant::clone()
{
	return GlowingPendant::create();
}

const std::string& GlowingPendant::getItemName()
{
	return GlowingPendant::SaveKey;
}

LocalizedString* GlowingPendant::getString()
{
	return Strings::Items_Equipment_Gear_Necklaces_GlowingPendant::create();
}

const std::string& GlowingPendant::getIconResource()
{
	return ItemResources::Equipment_Gear_Necklaces_GlowingPendant;
}

const std::string& GlowingPendant::getIdentifier()
{
	return GlowingPendant::SaveKey;
}
