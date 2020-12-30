#include "LogicGate.h"

#include "cocos/2d/CCActionInstant.h"
#include "cocos/2d/CCActionInterval.h"
#include "cocos/2d/CCActionEase.h"
#include "cocos/2d/CCSprite.h"
#include "cocos/base/CCValue.h"

#include "Engine/Events/ObjectEvents.h"
#include "Engine/Localization/LocalizedString.h"
#include "Engine/Hackables/HackableCode.h"
#include "Engine/Physics/CollisionObject.h"
#include "Engine/Save/SaveManager.h"
#include "Engine/Sound/WorldSound.h"
#include "Engine/Utils/GameUtils.h"
#include "Engine/Utils/MathUtils.h"
#include "Scenes/Platformer/Level/Physics/PlatformerCollisionType.h"

#include "Resources/ObjectResources.h"
#include "Resources/SoundResources.h"
#include "Resources/UIResources.h"

using namespace cocos2d;

const std::string LogicGate::MapKey = "logic-gate";
const std::string LogicGate::PropertyGateColor = "gate-color";
const std::string LogicGate::PropertyDelta = "delta";
const std::string LogicGate::PropertyAnswer = "answer";

LogicGate* LogicGate::create(ValueMap& properties)
{
	LogicGate* instance = new LogicGate(properties);
	
	instance->autorelease();

	return instance;
}

LogicGate::LogicGate(ValueMap& properties) : super(properties)
{
	this->colorName = GameUtils::getKeyOrDefault(this->properties, Brazier::PropertyColor, Value("")).asString();
	this->operationName = GameUtils::getKeyOrDefault(this->properties, Brazier::PropertyOperation, Value("")).asString();
	this->gateCollision = CollisionObject::create(CollisionObject::createBox(Size(285.0f, 676.0f)), (CollisionType)PlatformerCollisionType::Solid, CollisionObject::Properties(false, false));
	this->torchColor = Brazier::StrToColor(this->colorName);
	this->operation = Brazier::StrToOperation(this->operationName);
	this->delta = GameUtils::getKeyOrDefault(this->properties, LogicGate::PropertyDelta, Value(512.0f)).asFloat();
	this->answer = GameUtils::getKeyOrDefault(this->properties, LogicGate::PropertyAnswer, Value(false)).asBool();
	this->openSound = WorldSound::create(SoundResources::Platformer_Objects_Machines_WoodMechanism1);
	this->isOpen = false;
	this->wasOpen = false;
	this->firstRun = false;

	std::string gateColor = GameUtils::getKeyOrDefault(this->properties, LogicGate::PropertyGateColor, Value("")).asString();

	if (gateColor == "yellow")
	{
		this->gate = Sprite::create(ObjectResources::Puzzles_LogicGate_LogicGateYellow);
	}
	else
	{
		this->gate = Sprite::create(ObjectResources::Puzzles_LogicGate_LogicGateGray);
	}

	this->gateCollision->addChild(this->gate);
	this->addChild(this->gateCollision);
	this->addChild(this->openSound);
}

LogicGate::~LogicGate()
{
}

void LogicGate::onEnter()
{
	super::onEnter();

	this->computeIsOpen(true);

	this->listenForMapEvent(Brazier::MapEventTorchLogicSwitchPrefix + this->colorName, [=](ValueMap)
	{
		this->computeIsOpen(false);
	});

	this->listenForMapEvent(Brazier::MapEventSolveTorches, [=](ValueMap)
	{
		this->saveObjectState(Brazier::SaveKeyIsSolved, Value(true));
		this->checkComplete();
		this->computeIsOpen(false);
	});

	this->listenForMapEvent(Brazier::MapEventCheckComplete, [=](ValueMap)
	{
		this->checkComplete();
		this->computeIsOpen(false);
	});
}

void LogicGate::initializeListeners()
{
	super::initializeListeners();
}

bool LogicGate::isSolved()
{
	return this->isOpen == this->answer;
}

void LogicGate::computeIsOpen(bool isInstant)
{
	if (this->loadObjectStateOrDefault(Brazier::SaveKeyIsSolved, Value(false)).asBool())
	{
		this->isOpen = this->answer;
		this->runGateAnim(isInstant);
		return;
	}

	this->isOpen = false;
	this->firstRun = true;

	ObjectEvents::QueryObjects(QueryObjectsArgs<Brazier>([=](Brazier* logicTorch)
	{
		switch (this->operation)
		{
			case Brazier::Operation::And:
			{
				this->isOpen = firstRun ? logicTorch->isOn() : (this->isOpen & logicTorch->isOn());
				break;
			}
			case Brazier::Operation::Xor:
			{
				this->isOpen ^= logicTorch->isOn();
				break;
			}
			default:
			case Brazier::Operation::Or:
			{
				this->isOpen |= logicTorch->isOn();
				break;
			}
		}

		firstRun = false;
	}), this->operationName + "_" + this->colorName);

	this->runGateAnim(isInstant);
}

void LogicGate::checkComplete()
{
	if (this->loadObjectStateOrDefault(Brazier::SaveKeyIsSolved, Value(false)).asBool())
	{
		return;
	}

	// Note: using variable firstRun here instead of local variable "areAllSolved", because reference capture lambdas are glitchy
	this->firstRun = this->isSolved();

	ObjectEvents::QueryObjects(QueryObjectsArgs<LogicGate>([=](LogicGate* logicGate)
	{
		this->firstRun &= logicGate->isSolved();
	}));

	bool areAllSolved = this->firstRun;

	if (areAllSolved)
	{
		this->broadcastMapEvent(Brazier::MapEventSolveTorches, ValueMap());
	}
}

void LogicGate::runGateAnim(bool isInstant)
{
	Vec2 endPosition = this->isOpen ? Vec2(0.0f, -this->delta) : Vec2::ZERO;
	float max = std::abs(this->delta);
	float progress = std::abs(this->gateCollision->getPositionY()) / max;
	float relativeProgress = this->isOpen ? (1.0f - progress) : progress;

	this->gateCollision->stopAllActions();

	if (isInstant)
	{
		this->gateCollision->setPosition(endPosition);
	}
	else
	{
		this->gateCollision->runAction(MoveTo::create(relativeProgress * 3.0f, endPosition));

		if (this->wasOpen != this->isOpen)
		{
			this->openSound->play();
		}
	}

	this->wasOpen = this->isOpen;
}
