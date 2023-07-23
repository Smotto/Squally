#include "VoidFlower.h"

#include "Engine/Inventory/CurrencyInventory.h"
#include "Scenes/Platformer/Inventory/Currencies/IOU.h"

#include "Resources/ItemResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

const std::string VoidFlower::SaveKey = "void-flower";

VoidFlower* VoidFlower::create()
{
	VoidFlower* instance = new VoidFlower();

	instance->autorelease();

	return instance;
}

VoidFlower::VoidFlower() : super(CurrencyInventory::create({{ IOU::getIOUIdentifier(), 69 }}), ItemMeta(20000))
{
}

VoidFlower::~VoidFlower()
{
}

Item* VoidFlower::clone()
{
	return VoidFlower::create();
}

LocalizedString* VoidFlower::getString()
{
	return Strings::Items_Crafting_PotionReagents_VoidFlower::create();
}

const std::string& VoidFlower::getIconResource()
{
	return ItemResources::Crafting_Alchemy_Potions_T8_VoidFlower;
}

const std::string& VoidFlower::getIdentifier()
{
	return VoidFlower::SaveKey;
}
