#pragma once

#include "Objects/Platformer/Breakables/CagedAnimals/CagedAnimal.h"

namespace cocos2d
{
	class Sprite;
};

class CagedLizard : public CagedAnimal
{
public:
	static CagedLizard* create(cocos2d::ValueMap& properties);

	static const std::string MapKey;

protected:
	CagedLizard(cocos2d::ValueMap& properties);
	virtual ~CagedLizard();

	void initializePositions() override;

	LocalizedString* getAnimalName() override;
	std::string getSpriteResource() override;

private:
	typedef CagedAnimal super;

	cocos2d::Sprite* animalSprite = nullptr;
};
