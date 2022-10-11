#include "CagedSnake.h"

#include "cocos/2d/CCActionInstant.h"
#include "cocos/2d/CCActionInterval.h"
#include "cocos/2d/CCSprite.h"

#include "Scenes/Platformer/Save/SaveKeys.h"

#include "Resources/ObjectResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

const std::string CagedSnake::MapKey = "caged-snake";

CagedSnake* CagedSnake::create(ValueMap& properties)
{
	CagedSnake* instance = new CagedSnake(properties);

	instance->autorelease();

	return instance;
}

CagedSnake::CagedSnake(ValueMap& properties) : super(properties, SaveKeys::SaveKeyCollectableAnimalSnake)
{
	this->animalSprite = Sprite::create(ObjectResources::Collectables_Animals_Snake);

	this->animalNode->addChild(this->animalSprite);
}

CagedSnake::~CagedSnake()
{
}

void CagedSnake::initializePositions()
{
	super::initializePositions();

	this->animalSprite->setPosition(Vec2(0.0f, -16.0f));
}

LocalizedString* CagedSnake::getAnimalName()
{
	return Strings::Menus_Collectables_Animals_Snake::create();
}

std::string CagedSnake::getSpriteResource()
{
	return ObjectResources::Collectables_Animals_Snake;
}