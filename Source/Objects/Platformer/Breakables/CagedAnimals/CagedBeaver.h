#pragma once

#include "Objects/Platformer/Breakables/CagedAnimals/CagedAnimal.h"

namespace cocos2d
{
	class Sprite;
};

class CagedBeaver : public CagedAnimal
{
public:
	static CagedBeaver* create(cocos2d::ValueMap& properties);

	static const std::string MapKey;

protected:
	CagedBeaver(cocos2d::ValueMap& properties);
	virtual ~CagedBeaver();

	void initializePositions() override;

	LocalizedString* getAnimalName() override;
	std::string getSpriteResource() override;

private:
	typedef CagedAnimal super;

	cocos2d::Sprite* animalSprite = nullptr;
};
