#pragma once

#include "Objects/Platformer/Breakables/CagedAnimals/CagedAnimal.h"

namespace cocos2d
{
	class Sprite;
};

class CagedHorse : public CagedAnimal
{
public:
	static CagedHorse* create(cocos2d::ValueMap& properties);

	static const std::string MapKey;

protected:
	CagedHorse(cocos2d::ValueMap& properties);
	virtual ~CagedHorse();

	void initializePositions() override;

	LocalizedString* getAnimalName() override;
	std::string getSpriteResource() override;

private:
	typedef CagedAnimal super;

	cocos2d::Sprite* animalSprite = nullptr;
};
