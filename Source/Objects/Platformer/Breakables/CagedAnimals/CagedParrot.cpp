#include "CagedParrot.h"

#include "cocos/2d/CCActionInstant.h"
#include "cocos/2d/CCActionInterval.h"
#include "cocos/2d/CCSprite.h"

#include "Scenes/Platformer/Save/SaveKeys.h"

#include "Resources/ObjectResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

const std::string CagedParrot::MapKey = "caged-parrot";

CagedParrot* CagedParrot::create(ValueMap& properties)
{
	CagedParrot* instance = new CagedParrot(properties);

	instance->autorelease();

	return instance;
}

CagedParrot::CagedParrot(ValueMap& properties) : super(properties, SaveKeys::SaveKeyCollectableAnimalParrot)
{
	this->animalSprite = Sprite::create(ObjectResources::Collectables_Animals_Parrot);

	this->animalNode->addChild(this->animalSprite);
}

CagedParrot::~CagedParrot()
{
}

void CagedParrot::initializePositions()
{
	super::initializePositions();

	this->animalSprite->setPosition(Vec2(0.0f, -16.0f));
}

LocalizedString* CagedParrot::getAnimalName()
{
	return Strings::Menus_Collectables_Animals_Parrot::create();
}

std::string CagedParrot::getSpriteResource()
{
	return ObjectResources::Collectables_Animals_Parrot;
}
