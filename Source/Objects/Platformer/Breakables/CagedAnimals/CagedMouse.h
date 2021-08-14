#pragma once

#include "Objects/Platformer/Breakables/CagedAnimals/CagedAnimal.h"

namespace cocos2d
{
	class Sprite;
};

class CagedMouse : public CagedAnimal
{
public:
	static CagedMouse* create(cocos2d::ValueMap& properties);

	static const std::string MapKey;

protected:
	CagedMouse(cocos2d::ValueMap& properties);
	virtual ~CagedMouse();

	void initializePositions() override;

	LocalizedString* getAnimalName() override;
	std::string getSpriteResource() override;

private:
	typedef CagedAnimal super;

	cocos2d::Sprite* animalSprite = nullptr;
};
