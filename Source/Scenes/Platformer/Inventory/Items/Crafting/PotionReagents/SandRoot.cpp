#include "SandRoot.h"

#include "Engine/Inventory/CurrencyInventory.h"
#include "Scenes/Platformer/Inventory/Currencies/IOU.h"

#include "Resources/ItemResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

const std::string SandRoot::SaveKey = "sand-root";

SandRoot* SandRoot::create()
{
	SandRoot* instance = new SandRoot();

	instance->autorelease();

	return instance;
}

SandRoot::SandRoot() : super(CurrencyInventory::create({{ IOU::getIOUIdentifier(), 18 }}), ItemMeta(20000))
{
}

SandRoot::~SandRoot()
{
}

Item* SandRoot::clone()
{
	return SandRoot::create();
}

LocalizedString* SandRoot::getString()
{
	return Strings::Items_Crafting_PotionReagents_SandRoot::create();
}

const std::string& SandRoot::getIconResource()
{
	return ItemResources::Crafting_Alchemy_Potions_T3_SandRoot;
}

const std::string& SandRoot::getIdentifier()
{
	return SandRoot::SaveKey;
}
