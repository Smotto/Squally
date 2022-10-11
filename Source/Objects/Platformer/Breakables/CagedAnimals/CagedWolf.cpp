#include "CagedWolf.h"

#include "cocos/2d/CCActionInstant.h"
#include "cocos/2d/CCActionInterval.h"
#include "cocos/2d/CCSprite.h"

#include "Scenes/Platformer/Save/SaveKeys.h"

#include "Resources/ObjectResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

const std::string CagedWolf::MapKey = "caged-wolf";

CagedWolf* CagedWolf::create(ValueMap& properties)
{
	CagedWolf* instance = new CagedWolf(properties);

	instance->autorelease();

	return instance;
}

CagedWolf::CagedWolf(ValueMap& properties) : super(properties, SaveKeys::SaveKeyCollectableAnimalWolf)
{
	this->animalSprite = Sprite::create(ObjectResources::Collectables_Animals_Wolf);

	this->animalNode->addChild(this->animalSprite);
}

CagedWolf::~CagedWolf()
{
}

void CagedWolf::initializePositions()
{
	super::initializePositions();

	this->animalSprite->setPosition(Vec2(0.0f, -16.0f));
}

LocalizedString* CagedWolf::getAnimalName()
{
	return Strings::Menus_Collectables_Animals_Wolf::create();
}

std::string CagedWolf::getSpriteResource()
{
	return ObjectResources::Collectables_Animals_Wolf;
}
