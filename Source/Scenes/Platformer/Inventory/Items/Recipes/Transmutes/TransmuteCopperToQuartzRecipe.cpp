#include "TransmuteCopperToQuartzRecipe.h"

#include "Engine/Inventory/CurrencyInventory.h"
#include "Scenes/Platformer/Inventory/Currencies/IOU.h"
#include "Scenes/Platformer/Inventory/Items/PlatformerItems.h"

#include "Resources/ItemResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

const std::string TransmuteCopperToQuartzRecipe::SaveKey = "copper-to-quartz-recipe";

TransmuteCopperToQuartzRecipe* TransmuteCopperToQuartzRecipe::create()
{
	TransmuteCopperToQuartzRecipe* instance = new TransmuteCopperToQuartzRecipe();

	instance->autorelease();

	return instance;
}

TransmuteCopperToQuartzRecipe::TransmuteCopperToQuartzRecipe() : super(CurrencyInventory::create({{ IOU::getIOUIdentifier(), 2 }}))
{
}

TransmuteCopperToQuartzRecipe::~TransmuteCopperToQuartzRecipe()
{
}

std::vector<Item*> TransmuteCopperToQuartzRecipe::craft()
{
	return { Quartz::create() };
}

std::vector<std::tuple<Item*, int>> TransmuteCopperToQuartzRecipe::getReagentsInternal()
{
	return
	{
		{ Copper::create(), 4 },
		{ Coal::create(), 1 },
	};
}

Item* TransmuteCopperToQuartzRecipe::clone()
{
	return TransmuteCopperToQuartzRecipe::create();
}

LocalizedString* TransmuteCopperToQuartzRecipe::getString()
{
	return Strings::Items_Crafting_Recipes_TransmuteCopperToQuartz::create();
}

const std::string& TransmuteCopperToQuartzRecipe::getIconResource()
{
	return ItemResources::Misc_SCROLL_1;
}

std::string TransmuteCopperToQuartzRecipe::getCraftedItemIconResource()
{
	return ItemResources::Crafting_Smithing_T2_Quartz;
}

const std::string& TransmuteCopperToQuartzRecipe::getIdentifier()
{
	return TransmuteCopperToQuartzRecipe::SaveKey;
}
