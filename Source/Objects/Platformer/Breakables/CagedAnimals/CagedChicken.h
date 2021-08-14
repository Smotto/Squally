#pragma once

#include "Objects/Platformer/Breakables/CagedAnimals/CagedAnimal.h"

namespace cocos2d
{
	class Sprite;
};

class CagedChicken : public CagedAnimal
{
public:
	static CagedChicken* create(cocos2d::ValueMap& properties);

	static const std::string MapKey;

protected:
	CagedChicken(cocos2d::ValueMap& properties);
	virtual ~CagedChicken();

	void initializePositions() override;

	LocalizedString* getAnimalName() override;
	std::string getSpriteResource() override;

private:
	typedef CagedAnimal super;

	cocos2d::Sprite* animalSprite = nullptr;
};
