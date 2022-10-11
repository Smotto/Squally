#include "CagedBee.h"

#include "cocos/2d/CCActionInstant.h"
#include "cocos/2d/CCActionInterval.h"
#include "cocos/2d/CCSprite.h"

#include "Scenes/Platformer/Save/SaveKeys.h"

#include "Resources/ObjectResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

const std::string CagedBee::MapKey = "caged-bee";

CagedBee* CagedBee::create(ValueMap& properties)
{
	CagedBee* instance = new CagedBee(properties);

	instance->autorelease();

	return instance;
}

CagedBee::CagedBee(ValueMap& properties) : super(properties, SaveKeys::SaveKeyCollectableAnimalBee)
{
	this->animalSprite = Sprite::create(ObjectResources::Collectables_Animals_Bee);

	this->animalNode->addChild(this->animalSprite);
}

CagedBee::~CagedBee()
{
}

void CagedBee::initializePositions()
{
	super::initializePositions();

	this->animalSprite->setPosition(Vec2(0.0f, -16.0f));
}

LocalizedString* CagedBee::getAnimalName()
{
	return Strings::Menus_Collectables_Animals_Bee::create();
}

std::string CagedBee::getSpriteResource()
{
	return ObjectResources::Collectables_Animals_Bee;
}
