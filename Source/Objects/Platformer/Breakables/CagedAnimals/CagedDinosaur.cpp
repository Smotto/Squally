#include "CagedDinosaur.h"

#include "cocos/2d/CCActionInstant.h"
#include "cocos/2d/CCActionInterval.h"
#include "cocos/2d/CCSprite.h"

#include "Scenes/Platformer/Save/SaveKeys.h"

#include "Resources/ObjectResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

const std::string CagedDinosaur::MapKey = "caged-dinosaur";

CagedDinosaur* CagedDinosaur::create(ValueMap& properties)
{
	CagedDinosaur* instance = new CagedDinosaur(properties);

	instance->autorelease();

	return instance;
}

CagedDinosaur::CagedDinosaur(ValueMap& properties) : super(properties, SaveKeys::SaveKeyCollectableAnimalDinosaur)
{
	this->animalSprite = Sprite::create(ObjectResources::Collectables_Animals_Dinosaur);

	this->animalNode->addChild(this->animalSprite);
}

CagedDinosaur::~CagedDinosaur()
{
}

void CagedDinosaur::initializePositions()
{
	super::initializePositions();

	this->animalSprite->setPosition(Vec2(0.0f, -16.0f));
}

LocalizedString* CagedDinosaur::getAnimalName()
{
	return Strings::Menus_Collectables_Animals_Dinosaur::create();
}

std::string CagedDinosaur::getSpriteResource()
{
	return ObjectResources::Collectables_Animals_Dinosaur;
}
