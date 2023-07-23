#include "Iron.h"

#include "Engine/Inventory/CurrencyInventory.h"
#include "Scenes/Platformer/Inventory/Currencies/IOU.h"

#include "Resources/ItemResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

const std::string Iron::SaveKey = "iron";

Iron* Iron::create()
{
	Iron* instance = new Iron();

	instance->autorelease();

	return instance;
}

Iron::Iron() : super(CurrencyInventory::create({{ IOU::getIOUIdentifier(), 8 * 2 }}), ItemMeta(20000))
{
}

Iron::~Iron()
{
}

Item* Iron::clone()
{
	return Iron::create();
}

LocalizedString* Iron::getString()
{
	return Strings::Items_Crafting_Ore_Iron::create();
}

const std::string& Iron::getIconResource()
{
	return ItemResources::Crafting_Smithing_T2_Iron;
}

const std::string& Iron::getIdentifier()
{
	return Iron::SaveKey;
}
