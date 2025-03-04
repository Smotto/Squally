#include "ScreamingDoor.h"

#include "cocos/2d/CCSprite.h"
#include "cocos/base/CCValue.h"

#include "Objects/Platformer/Interactables/Doors/Screaming/TrackingEye.h"
#include "Objects/Platformer/Interactables/Doors/Screaming/TrackingEyeController.h"

#include "Resources/ObjectResources.h"

using namespace cocos2d;

const std::string ScreamingDoor::MapKey = "screaming-door";

ScreamingDoor* ScreamingDoor::create(ValueMap& properties)
{
	ScreamingDoor* instance = new ScreamingDoor(properties);

	instance->autorelease();

	return instance;
}

ScreamingDoor::ScreamingDoor(ValueMap& properties) : super(properties, CSize(256.0f, 420.0f), Vec2(0.0f, -320.0f))
{
	ValueMap trackingEyeProperties = ValueMap();

	this->screamingDoor = Sprite::create(ObjectResources::Doors_ScreamingDoor_ScreamingDoor);
	this->screamingDoorLocked = Sprite::create(ObjectResources::Doors_ScreamingDoor_ScreamingDoorLocked);
	this->leftEye = TrackingEye::create(trackingEyeProperties);
	this->rightEye = TrackingEye::create(trackingEyeProperties);
	this->eyeController = TrackingEyeController::create(trackingEyeProperties);

	this->leftEye->pair(this->rightEye);
	this->rightEye->pair(this->leftEye);

	this->screamingDoorLocked->setVisible(false);

	this->contentNode->addChild(this->screamingDoor);
	this->contentNode->addChild(this->screamingDoorLocked);
	this->contentNode->addChild(this->leftEye);
	this->contentNode->addChild(this->rightEye);
	this->contentNode->addChild(this->eyeController);
}

ScreamingDoor::~ScreamingDoor()
{
}

void ScreamingDoor::onEnter()
{
	super::onEnter();
}

void ScreamingDoor::initializePositions()
{
	super::initializePositions();

	this->screamingDoor->setPosition(Vec2(0.0f, 0.0f));
	this->leftEye->setPosition(Vec2(-108.0f, 76.0f));
	this->rightEye->setPosition(Vec2(96.0f, 76.0f));
}

void ScreamingDoor::initializeListeners()
{
	super::initializeListeners();
}

void ScreamingDoor::lock(bool animate)
{
	super::lock(animate);

	this->screamingDoor->setVisible(false);
	this->screamingDoorLocked->setVisible(true);
}

void ScreamingDoor::unlock(bool animate)
{
	super::unlock(animate);

	this->screamingDoor->setVisible(true);
	this->screamingDoorLocked->setVisible(false);
}
