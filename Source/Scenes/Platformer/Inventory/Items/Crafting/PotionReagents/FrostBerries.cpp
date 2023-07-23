#include "FrostBerries.h"

#include "Engine/Inventory/CurrencyInventory.h"
#include "Scenes/Platformer/Inventory/Currencies/IOU.h"

#include "Resources/ItemResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

const std::string FrostBerries::SaveKey = "frost-berries";

FrostBerries* FrostBerries::create()
{
	FrostBerries* instance = new FrostBerries();

	instance->autorelease();

	return instance;
}

FrostBerries::FrostBerries() : super(CurrencyInventory::create({{ IOU::getIOUIdentifier(), 36 }}), ItemMeta(20000))
{
}

FrostBerries::~FrostBerries()
{
}

Item* FrostBerries::clone()
{
	return FrostBerries::create();
}

LocalizedString* FrostBerries::getString()
{
	return Strings::Items_Crafting_PotionReagents_FrostBerries::create();
}

const std::string& FrostBerries::getIconResource()
{
	return ItemResources::Crafting_Alchemy_Potions_T7_FrostBerries;
}

const std::string& FrostBerries::getIdentifier()
{
	return FrostBerries::SaveKey;
}
