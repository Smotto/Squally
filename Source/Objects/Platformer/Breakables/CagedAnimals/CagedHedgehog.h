#pragma once

#include "Objects/Platformer/Breakables/CagedAnimals/CagedAnimal.h"

namespace cocos2d
{
	class Sprite;
};

class CagedHedgehog : public CagedAnimal
{
public:
	static CagedHedgehog* create(cocos2d::ValueMap& properties);

	static const std::string MapKey;

protected:
	CagedHedgehog(cocos2d::ValueMap& properties);
	virtual ~CagedHedgehog();

	void initializePositions() override;

	LocalizedString* getAnimalName() override;
	std::string getSpriteResource() override;

private:
	typedef CagedAnimal super;

	cocos2d::Sprite* animalSprite = nullptr;
};
