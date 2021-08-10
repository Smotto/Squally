#pragma once

#include "Scenes/Platformer/Inventory/Items/Equipment/Weapons/Axes/Axe.h"

class Scythe : public Axe
{
public:
	static Scythe* create();

	Item* clone() override;
	LocalizedString* getString() override;
	const std::string& getIconResource() override;
	const std::string& getIdentifier() override;
	cocos2d::CSize getWeaponCollisionSize() override;
	cocos2d::Vec2 getWeaponCollisionOffset() override;
	cocos2d::Vec2 getDisplayOffset() override;
	Recipe* getRecipe() override;

	static const std::string SaveKey;

protected:
	Scythe();
	virtual ~Scythe();

private:
	typedef Axe super;
};
