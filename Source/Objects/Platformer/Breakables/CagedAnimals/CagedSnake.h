#pragma once

#include "Objects/Platformer/Breakables/CagedAnimals/CagedAnimal.h"

namespace cocos2d
{
	class Sprite;
};

class CagedSnake : public CagedAnimal
{
public:
	static CagedSnake* create(cocos2d::ValueMap& properties);

	static const std::string MapKey;

protected:
	CagedSnake(cocos2d::ValueMap& properties);
	virtual ~CagedSnake();

	void initializePositions() override;

	LocalizedString* getAnimalName() override;
	std::string getSpriteResource() override;

private:
	typedef CagedAnimal super;

	cocos2d::Sprite* animalSprite = nullptr;
};
