#pragma once

#include "Scenes/Platformer/Inventory/Items/Crafting/Crafting.h"

class LocalizedString;

class BlackMushroom : public Crafting
{
public:
	static BlackMushroom* create();

	Item* clone() override;
	LocalizedString* getString() override;
	const std::string& getIconResource() override;
	const std::string& getIdentifier() override;

	static const std::string SaveKey;

protected:
	BlackMushroom();
	virtual ~BlackMushroom();

private:
	typedef Crafting super;
};
