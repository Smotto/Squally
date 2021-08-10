#include "Chest.h"

#include "cocos/2d/CCSprite.h"

#include "Resources/ObjectResources.h"

using namespace cocos2d;

const std::string Chest::MapKey = "chest";

Chest* Chest::create(cocos2d::ValueMap& properties)
{
	Chest* instance = new Chest(properties);

	instance->autorelease();

	return instance;
}

Chest::Chest(cocos2d::ValueMap& properties) : super(properties, CSize(128.0f, 112.0f))
{
	Sprite* chestOpenFrontSprite = Sprite::create(ObjectResources::Interactive_Chests_ChestOpen);
	Sprite* chestClosedSprite = Sprite::create(ObjectResources::Interactive_Chests_ChestClosed);

	this->chestOpen->addChild(chestOpenFrontSprite);
	this->chestClosed->addChild(chestClosedSprite);
}

Chest::~Chest()
{
}
