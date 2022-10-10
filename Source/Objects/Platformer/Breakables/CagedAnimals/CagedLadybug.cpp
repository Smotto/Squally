#include "CagedLadybug.h"

#include "cocos/2d/CCActionInstant.h"
#include "cocos/2d/CCActionInterval.h"
#include "cocos/2d/CCSprite.h"

#include "Scenes/Platformer/Save/SaveKeys.h"

#include "Resources/ObjectResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

const std::string CagedLadybug::MapKey = "caged-ladybug";

CagedLadybug* CagedLadybug::create(ValueMap& properties)
{
	CagedLadybug* instance = new CagedLadybug(properties);

	instance->autorelease();

	return instance;
}

CagedLadybug::CagedLadybug(ValueMap& properties) : super(properties, SaveKeys::SaveKeyCollectableAnimalLadybug)
{
	this->animalSprite = Sprite::create(ObjectResources::Collectables_Animals_Ladybug);

	this->animalNode->addChild(this->animalSprite);
}

CagedLadybug::~CagedLadybug()
{
}

void CagedLadybug::initializePositions()
{
	super::initializePositions();

	this->animalSprite->setPosition(Vec2(0.0f, -16.0f));
}

LocalizedString* CagedLadybug::getAnimalName()
{
	return Strings::Menus_Collectables_Animals_Ladybug::create();
}

std::string CagedLadybug::getSpriteResource()
{
	return ObjectResources::Collectables_Animals_Ladybug;
}
