#include "TempleDoor.h"

#include "cocos/2d/CCActionInstant.h"
#include "cocos/2d/CCActionInterval.h"
#include "cocos/2d/CCActionEase.h"
#include "cocos/2d/CCSprite.h"
#include "cocos/base/CCValue.h"

#include "Engine/Physics/CollisionObject.h"
#include "Engine/Sound/WorldSound.h"
#include "Engine/UI/SmartClippingNode.h"
#include "Engine/Utils/GameUtils.h"
#include "Scenes/Platformer/Level/Physics/PlatformerCollisionType.h"

#include "Resources/ObjectResources.h"
#include "Resources/SoundResources.h"

using namespace cocos2d;

const std::string TempleDoor::MapKey = "temple-door";
const std::string TempleDoor::PropertyColor = "color";
const float TempleDoor::DoorOpenDelta = 320.0f;
const std::string TempleDoor::UnlockedSaveKey = "TEMPLE_DOOR_UNLOCKED";

TempleDoor* TempleDoor::create(ValueMap& properties)
{
	TempleDoor* instance = new TempleDoor(properties);

	instance->autorelease();

	return instance;
}

TempleDoor::TempleDoor(ValueMap& properties) : super(properties, Size(192.0f, 528.0f), Vec2(0.0f, 0.0f))
{
	this->topCollision = CollisionObject::create(CollisionObject::createBox(Size(420.0f, 64.0f)), (CollisionType)PlatformerCollisionType::PassThrough, CollisionObject::Properties(false, false));
	this->emblem = nullptr;
	this->emblemOffset = Vec2::ZERO;
	this->doorOpenSound = WorldSound::create(SoundResources::Platformer_Objects_Doors_StoneWall1);

	std::string color = GameUtils::getKeyOrDefault(this->properties, TempleDoor::PropertyColor, Value("")).asString();

	// if (color == "yellow" || color == "yellow-skull" || color == "yellow-up" || color == "yellow-down")
	{
		this->back = Sprite::create(ObjectResources::Doors_Temple_TempleDoorBack);
		this->door = Sprite::create(ObjectResources::Doors_Temple_TempleDoor);
		this->doorClip = SmartClippingNode::create(this->door, Size(212.0f, 386.0f));
	}
	
	if (color == "yellow-skull" || color == "green-skull")
	{
		this->emblem = Sprite::create(ObjectResources::Doors_Temple_Skull);
		this->emblemOffset = Vec2(0.0f, 116.0f);
	}
	else if (color == "yellow-scarab" || color == "green-scarab")
	{
		this->emblem = Sprite::create(ObjectResources::Doors_Temple_Scarab);
		this->emblemOffset = Vec2(0.0f, 116.0f);
	}
	else if (color == "yellow-up" || color == "green-up")
	{
		this->emblem = Sprite::create(ObjectResources::Doors_Temple_UpArrow);
		this->emblemOffset = Vec2(0.0f, 172.0f);
	}
	else if (color == "yellow-down" || color == "green-down")
	{
		this->emblem = Sprite::create(ObjectResources::Doors_Temple_DownArrow);
		this->emblemOffset = Vec2(0.0f, 172.0f);
	}

	this->contentNode->addChild(this->back);
	this->contentNode->addChild(this->doorClip);

	if (this->emblem != nullptr)
	{
		this->contentNode->addChild(this->emblem);
	}

	this->addChild(this->topCollision);
	this->addChild(this->doorOpenSound);
}

TempleDoor::~TempleDoor()
{
}

void TempleDoor::onEnter()
{
	super::onEnter();
	
	if (this->loadObjectStateOrDefault(TempleDoor::UnlockedSaveKey, Value(true)).asBool())
	{
		this->unlock(false);
	}
	else
	{
		this->lock(false);
	}
}

void TempleDoor::initializePositions()
{
	super::initializePositions();

	if (this->emblem != nullptr)
	{
		this->emblem->setPosition(this->emblemOffset);
	}

	this->topCollision->setPosition(Vec2(0.0f, 238.0f));
	this->doorClip->setPosition(Vec2(0.0f, -66.0f));
}

void TempleDoor::lock(bool animate)
{
	super::lock(animate);

	this->saveObjectState(TempleDoor::UnlockedSaveKey, Value(false));
	
	float currentProgress = this->door->getPositionY() / TempleDoor::DoorOpenDelta;

	if (animate)
	{
		this->door->stopAllActions();
		this->door->runAction(MoveTo::create(5.0f * currentProgress, Vec2::ZERO));

		this->doorOpenSound->play();
	}
	else
	{
		this->door->setPosition(Vec2::ZERO);
	}
}

void TempleDoor::unlock(bool animate)
{
	super::unlock(animate);

	this->saveObjectState(TempleDoor::UnlockedSaveKey, Value(true));

	float currentProgress = 1.0f - this->door->getPositionY() / TempleDoor::DoorOpenDelta;

	if (animate)
	{
		this->door->stopAllActions();
		this->door->runAction(MoveTo::create(5.0f * currentProgress, Vec2(0.0f, TempleDoor::DoorOpenDelta)));
		this->doorOpenSound->play();
	}
	else
	{
		this->door->setPosition(Vec2(0.0f, TempleDoor::DoorOpenDelta));
	}
}
