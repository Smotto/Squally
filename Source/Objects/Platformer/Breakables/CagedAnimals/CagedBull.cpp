#include "CagedBull.h"

#include "cocos/2d/CCActionInstant.h"
#include "cocos/2d/CCActionInterval.h"
#include "cocos/2d/CCSprite.h"

#include "Scenes/Platformer/Save/SaveKeys.h"

#include "Resources/ObjectResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

const std::string CagedBull::MapKey = "caged-bull";

CagedBull* CagedBull::create(ValueMap& properties)
{
	CagedBull* instance = new CagedBull(properties);

	instance->autorelease();

	return instance;
}

CagedBull::CagedBull(ValueMap& properties) : super(properties, SaveKeys::SaveKeyCollectableAnimalBull)
{
	this->animalSprite = Sprite::create(ObjectResources::Collectables_Animals_Bull);

	this->animalNode->addChild(this->animalSprite);
}

CagedBull::~CagedBull()
{
}

void CagedBull::initializePositions()
{
	super::initializePositions();

	this->animalSprite->setPosition(Vec2(0.0f, -16.0f));
}

LocalizedString* CagedBull::getAnimalName()
{
	return Strings::Menus_Collectables_Animals_Bull::create();
}

std::string CagedBull::getSpriteResource()
{
	return ObjectResources::Collectables_Animals_Bull;
}
