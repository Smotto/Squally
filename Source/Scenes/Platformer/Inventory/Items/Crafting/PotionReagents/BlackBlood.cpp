#include "BlackBlood.h"

#include "Engine/Inventory/CurrencyInventory.h"
#include "Scenes/Platformer/Inventory/Currencies/IOU.h"

#include "Resources/ItemResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

const std::string BlackBlood::SaveKey = "black-blood";

BlackBlood* BlackBlood::create()
{
	BlackBlood* instance = new BlackBlood();

	instance->autorelease();

	return instance;
}

BlackBlood::BlackBlood() : super(CurrencyInventory::create({{ IOU::getIOUIdentifier(), 18 }}), ItemMeta(20000))
{
}

BlackBlood::~BlackBlood()
{
}

Item* BlackBlood::clone()
{
	return BlackBlood::create();
}

LocalizedString* BlackBlood::getString()
{
	return Strings::Items_Crafting_PotionReagents_BlackBlood::create();
}

const std::string& BlackBlood::getIconResource()
{
	return ItemResources::Crafting_Alchemy_Potions_T6_BlackBlood;
}

const std::string& BlackBlood::getIdentifier()
{
	return BlackBlood::SaveKey;
}
