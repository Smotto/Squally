#include "CagedWorm.h"

#include "cocos/2d/CCActionInstant.h"
#include "cocos/2d/CCActionInterval.h"
#include "cocos/2d/CCSprite.h"

#include "Scenes/Platformer/Save/SaveKeys.h"

#include "Resources/ObjectResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

const std::string CagedWorm::MapKey = "caged-worm";

CagedWorm* CagedWorm::create(ValueMap& properties)
{
	CagedWorm* instance = new CagedWorm(properties);

	instance->autorelease();

	return instance;
}

CagedWorm::CagedWorm(ValueMap& properties) : super(properties, SaveKeys::SaveKeyCollectableAnimalWorm)
{
	this->animalSprite = Sprite::create(ObjectResources::Collectables_Animals_Worm);

	this->animalNode->addChild(this->animalSprite);
}

CagedWorm::~CagedWorm()
{
}

void CagedWorm::initializePositions()
{
	super::initializePositions();

	this->animalSprite->setPosition(Vec2(0.0f, -16.0f));
}

LocalizedString* CagedWorm::getAnimalName()
{
	return Strings::Menus_Collectables_Animals_Worm::create();
}

std::string CagedWorm::getSpriteResource()
{
	return ObjectResources::Collectables_Animals_Worm;
}
