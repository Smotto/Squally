#include "DemonRoot.h"

#include "Engine/Inventory/CurrencyInventory.h"
#include "Scenes/Platformer/Inventory/Currencies/IOU.h"

#include "Resources/ItemResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

const std::string DemonRoot::SaveKey = "demon-root";

DemonRoot* DemonRoot::create()
{
	DemonRoot* instance = new DemonRoot();

	instance->autorelease();

	return instance;
}

DemonRoot::DemonRoot() : super(CurrencyInventory::create({{ IOU::getIOUIdentifier(), 44 }}), ItemMeta(20000))
{
}

DemonRoot::~DemonRoot()
{
}

Item* DemonRoot::clone()
{
	return DemonRoot::create();
}

LocalizedString* DemonRoot::getString()
{
	return Strings::Items_Crafting_PotionReagents_DemonRoot::create();
}

const std::string& DemonRoot::getIconResource()
{
	return ItemResources::Crafting_Alchemy_Potions_T6_DemonRoot;
}

const std::string& DemonRoot::getIdentifier()
{
	return DemonRoot::SaveKey;
}
