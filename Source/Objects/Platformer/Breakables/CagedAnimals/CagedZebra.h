#pragma once

#include "Objects/Platformer/Breakables/CagedAnimals/CagedAnimal.h"

namespace cocos2d
{
	class Sprite;
};

class CagedZebra : public CagedAnimal
{
public:
	static CagedZebra* create(cocos2d::ValueMap& properties);

	static const std::string MapKey;

protected:
	CagedZebra(cocos2d::ValueMap& properties);
	virtual ~CagedZebra();

	void initializePositions() override;

	LocalizedString* getAnimalName() override;
	std::string getSpriteResource() override;

private:
	typedef CagedAnimal super;

	cocos2d::Sprite* animalSprite = nullptr;
};
