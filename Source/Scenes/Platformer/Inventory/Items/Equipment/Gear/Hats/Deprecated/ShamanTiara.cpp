#include "ShamanTiara.h"

#include "Engine/Inventory/CurrencyInventory.h"
#include "Scenes/Platformer/Inventory/Currencies/IOU.h"

#include "Resources/ItemResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

const std::string ShamanTiara::SaveKey = "shaman-tiara";

ShamanTiara* ShamanTiara::create()
{
	ShamanTiara* instance = new ShamanTiara();

	instance->autorelease();

	return instance;
}

ShamanTiara::ShamanTiara() : super(CurrencyInventory::create({{ IOU::getIOUIdentifier(), 2 }}), ItemStats(
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

ShamanTiara::~ShamanTiara()
{
}

Item* ShamanTiara::clone()
{
	return ShamanTiara::create();
}

LocalizedString* ShamanTiara::getString()
{
	return Strings::Items_Equipment_Gear_Hats_ShamanTiara::create();
}

const std::string& ShamanTiara::getIconResource()
{
	return ItemResources::Equipment_Gear_Hats__Deprecated_ShamanTiara;
}

const std::string& ShamanTiara::getIdentifier()
{
	return ShamanTiara::SaveKey;
}

Vec2 ShamanTiara::getDisplayOffset()
{
	return Vec2(8.0f, -10.0f);
}

Recipe* ShamanTiara::getRecipe()
{
	return nullptr;
}
