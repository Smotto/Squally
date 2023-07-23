#pragma once

#include "Scenes/Platformer/Inventory/Items/Recipes/Gear/Rings/RingRecipe.h"

class SilverRingRecipe : public RingRecipe
{
public:
	static SilverRingRecipe* create();

	std::vector<Item*> craft() override;
	Item* clone() override;
	LocalizedString* getString() override;
	const std::string& getIconResource() override;
	std::string getCraftedItemIconResource() override;
	const std::string& getIdentifier() override;

	static const std::string SaveKey;

protected:
	SilverRingRecipe();
	virtual ~SilverRingRecipe();
	
	std::vector<std::tuple<Item*, int>> getReagentsInternal() override;

private:
	typedef RingRecipe super;
};
