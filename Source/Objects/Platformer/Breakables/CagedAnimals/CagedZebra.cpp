#include "CagedZebra.h"

#include "cocos/2d/CCActionInstant.h"
#include "cocos/2d/CCActionInterval.h"
#include "cocos/2d/CCSprite.h"

#include "Scenes/Platformer/Save/SaveKeys.h"

#include "Resources/ObjectResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

const std::string CagedZebra::MapKey = "caged-zebra";

CagedZebra* CagedZebra::create(ValueMap& properties)
{
	CagedZebra* instance = new CagedZebra(properties);

	instance->autorelease();

	return instance;
}

CagedZebra::CagedZebra(ValueMap& properties) : super(properties, SaveKeys::SaveKeyCollectableAnimalZebra)
{
	this->animalSprite = Sprite::create(ObjectResources::Collectables_Animals_Zebra);

	this->animalNode->addChild(this->animalSprite);
}

CagedZebra::~CagedZebra()
{
}

void CagedZebra::initializePositions()
{
	super::initializePositions();

	this->animalSprite->setPosition(Vec2(0.0f, -16.0f));
}

LocalizedString* CagedZebra::getAnimalName()
{
	return Strings::Menus_Collectables_Animals_Zebra::create();
}

std::string CagedZebra::getSpriteResource()
{
	return ObjectResources::Collectables_Animals_Zebra;
}
