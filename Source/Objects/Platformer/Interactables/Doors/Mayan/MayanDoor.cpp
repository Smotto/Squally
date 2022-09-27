#include "MayanDoor.h"

#include "cocos/2d/CCActionInstant.h"
#include "cocos/2d/CCActionInterval.h"
#include "cocos/2d/CCActionEase.h"
#include "cocos/2d/CCSprite.h"
#include "cocos/base/CCValue.h"

#include "Engine/Animations/SmartAnimationNode.h"
#include "Engine/Events/ObjectEvents.h"
#include "Engine/Hackables/HackableCode.h"
#include "Engine/Physics/CollisionObject.h"
#include "Engine/Sound/WorldSound.h"
#include "Engine/UI/SmartClippingNode.h"
#include "Engine/Utils/GameUtils.h"
#include "Engine/Utils/MathUtils.h"
#include "Events/PlatformerEvents.h"
#include "Entities/Platformer/Squally/Squally.h"
#include "Objects/Platformer/Interactables/Doors/Mayan/RegisterStone.h"
#include "Scenes/Platformer/Level/Physics/PlatformerPhysicsTypes.h"
#include "Scenes/Platformer/Hackables/HackFlags.h"

#include "Resources/ObjectResources.h"
#include "Resources/SoundResources.h"
#include "Resources/UIResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

#define LOCAL_FUNC_ID_GEM_RED 1
#define LOCAL_FUNC_ID_GEM_BLUE 2
#define LOCAL_FUNC_ID_GEM_PURPLE 3

const std::string MayanDoor::MapKey = "mayan-door";
const float MayanDoor::DoorOpenDelta = 420.0f;
const std::string MayanDoor::EventMayanDoorUnlock = "mayan-door-unlock";
const std::string MayanDoor::MapEventPush = "EVENT_STONE_PUZZLE_PUSH";
const std::string MayanDoor::MapEventPop = "EVENT_STONE_PUZZLE_POP";
const std::string MayanDoor::MapEventPopRet = "EVENT_STONE_PUZZLE_POP_RET";
const std::string MayanDoor::PropertyRegister = "register";
const std::string MayanDoor::PropertyValue = "value";
const std::string MayanDoor::SaveKeyUnlocked = "SAVE_KEY_UNLOCKED";
const std::string MayanDoor::MapEventLockInteraction = "EVENT_STONE_PUZZLE_LOCK_INTERACTION";
const std::string MayanDoor::MapEventUnlockInteraction = "EVENT_STONE_PUZZLE_UNLOCK_INTERACTION";
const std::string MayanDoor::MapEventResetPuzzle = "reset-puzzle";
const std::string MayanDoor::MapEventPuzzleComplete = "puzzle-complete";

MayanDoor* MayanDoor::create(ValueMap& properties)
{
	MayanDoor* instance = new MayanDoor(properties);

	instance->autorelease();

	return instance;
}

MayanDoor::MayanDoor(ValueMap& properties) : super(properties, CSize(478.0f, 478.0f), Vec2(0.0f, 0.0f))
{
	this->doorArrow = Sprite::create(ObjectResources::Doors_Mayan_StoneCircleDoorArrow);
	this->doorFrame = Sprite::create(ObjectResources::Doors_Mayan_StoneCircleDoor);
	this->door = Sprite::create(ObjectResources::Doors_Mayan_StoneCircleDoorInner);
	this->innerContainer = Node::create();
	this->doorContainer = Node::create();
	this->doorOpenSound = WorldSound::create(SoundResources::Platformer_Objects_Doors_StoneWall1);
	this->doorSound2Units = WorldSound::create(SoundResources::Platformer_Objects_Machines_WoodMechanism2Units);
	this->doorSound3Units = WorldSound::create(SoundResources::Platformer_Objects_Machines_WoodMechanism3Units);
	this->doorSound4Units = WorldSound::create(SoundResources::Platformer_Objects_Machines_WoodMechanism4Units);
	this->doorSound5Units = WorldSound::create(SoundResources::Platformer_Objects_Machines_WoodMechanism5Units);
	this->doorSound6Units = WorldSound::create(SoundResources::Platformer_Objects_Machines_WoodMechanism6Units);

	this->doorContainer->addChild(this->doorFrame);
	this->doorContainer->addChild(this->innerContainer);
	this->innerContainer->addChild(this->doorArrow);
	this->innerContainer->addChild(this->door);
	this->contentNode->addChild(this->doorContainer);
	this->addChild(this->doorOpenSound);
	this->addChild(this->doorSound2Units);
	this->addChild(this->doorSound3Units);
	this->addChild(this->doorSound4Units);
	this->addChild(this->doorSound5Units);
	this->addChild(this->doorSound6Units);
}

MayanDoor::~MayanDoor()
{
}

void MayanDoor::onEnter()
{
	super::onEnter();

	bool isUnlocked = this->loadObjectStateOrDefault(MayanDoor::SaveKeyUnlocked, Value(!this->isLocked)).asBool();
	
	if (isUnlocked)
	{
		this->unlock(false);
	}
	else
	{
		this->lock(false);
	}
}

void MayanDoor::onEnterTransitionDidFinish()
{
	super::onEnterTransitionDidFinish();
	
	if (this->isLocked)
	{
		this->discoverStones();
	}
	else
	{
		this->broadcastMapEvent(MayanDoor::MapEventLockInteraction, ValueMap());
	}
}

void MayanDoor::initializePositions()
{
	super::initializePositions();

	const float Radius = 356.0f;
	const float RedGemAngle = 4.0f * float(M_PI) / 6.0f;
	const float PurpleGemAngle = 2.0f * float(M_PI) / 6.0f;

	this->doorArrow->setPosition(Vec2(0.0f, 180.0f));
	this->doorFrame->setPosition(Vec2(0.0f, 0.0f));
	this->innerContainer->setPosition(Vec2(0.0f, 0.0f));
}

void MayanDoor::initializeListeners()
{
	super::initializeListeners();

	this->listenForMapEvent(MayanDoor::EventMayanDoorUnlock, [=](ValueMap args)
	{
		if (this->isLocked)
		{
			this->tryUnlock();
		}
	});

	this->listenForMapEvent(MayanDoor::MapEventLockInteraction, [=](ValueMap args)
	{
		if (this->isLocked)
		{
			this->isUnlocking = true;
			this->disable();
		}
	});

	this->listenForMapEvent(MayanDoor::MapEventUnlockInteraction, [=](ValueMap args)
	{
		if (this->isLocked)
		{
			this->isUnlocking = false;
			this->enable();
		}
	});
}

void MayanDoor::discoverStones()
{
	ObjectEvents::QueryObject<RegisterStone>([=](RegisterStone* stone)
	{
		this->registerStones.push_back(stone);
	}, "eax");

	ObjectEvents::QueryObject<RegisterStone>([=](RegisterStone* stone)
	{
		this->registerStones.push_back(stone);
	}, "ebx");

	ObjectEvents::QueryObject<RegisterStone>([=](RegisterStone* stone)
	{
		this->registerStones.push_back(stone);
	}, "ecx");

	ObjectEvents::QueryObject<RegisterStone>([=](RegisterStone* stone)
	{
		this->registerStones.push_back(stone);
	}, "edx");

	ObjectEvents::QueryObject<RegisterStone>([=](RegisterStone* stone)
	{
		this->registerStones.push_back(stone);
	}, "edi");

	ObjectEvents::QueryObject<RegisterStone>([=](RegisterStone* stone)
	{
		this->registerStones.push_back(stone);
	}, "esi");
}

void MayanDoor::tryUnlock()
{
	if (this->isUnlocking)
	{
		return;
	}

	this->broadcastMapEvent(MayanDoor::MapEventLockInteraction, ValueMap());

	int index = 0;
	std::vector<int> combination = std::vector<int>();
	bool willUnlock = true;

	for (auto next : this->registerStones)
	{
		combination.push_back(next->getValue());

		willUnlock &= (next->getValue() == next->getCorrectValue());
	}

	auto getRotation = [=](int rotationIndex)
	{
		return -float(rotationIndex) / 12.0f * 360.0f;
	};

	const float RotationSpeedPerUnit = 0.25f;
	const float RotationSpeedPerUnitReturn = 0.5f;
	std::vector<float> rotations = std::vector<float>();
	std::vector<float> delays = std::vector<float>();

	for (auto next : combination)
	{
		rotations.push_back(getRotation(next));
	}

	rotations.push_back(getRotation(0));

	auto getDist = [=](int indexFrom, int indexTo)
	{
		int distBetweenWrapped = ((indexFrom < 6) ? indexFrom : (12 - indexFrom)) + ((indexTo < 6) ? indexTo : (12 - indexTo));
		int distBetween = std::abs(indexFrom - indexTo);
		
		return float(std::min(distBetweenWrapped, distBetween));
	};

	int previousIndex = 0;

	// Return to 0 at the end
	combination.push_back(0);

	for (auto next : combination)
	{
		delays.push_back(getDist(previousIndex, next) * RotationSpeedPerUnit);
		previousIndex = next;
	}

	Vector<FiniteTimeAction*> actions = Vector<FiniteTimeAction*>();

	for (int index = 0; index < std::min(int(rotations.size()), int(delays.size())); index++)
	{
		actions.pushBack(CallFunc::create([=]()
		{
			if (index < int(this->registerStones.size()))
			{
				this->registerStones[index]->runFx();
			}

			if (delays[index] <= 0.15f)
			{
			}
			else if (delays[index] <= 0.5f)
			{
				this->doorSound2Units->play();
			}
			else if (delays[index] <= 0.75f)
			{
				this->doorSound3Units->play();
			}
			else if (delays[index] <= 1.0f)
			{
				this->doorSound4Units->play();
			}
			else if (delays[index] <= 1.25f)
			{
				this->doorSound5Units->play();
			}
			else if (delays[index] <= 1.5f)
			{
				this->doorSound6Units->play();
			}
		}));
		actions.pushBack(RotateTo::create(std::max(delays[index], RotationSpeedPerUnit), rotations[index]));
		actions.pushBack(DelayTime::create(0.5f));
	}

	actions.pushBack(CallFunc::create([=]()
	{
		if (willUnlock)
		{
			this->unlock(true);
		}
		else
		{
			this->broadcastMapEvent(MayanDoor::MapEventResetPuzzle, ValueMap());
		}
	}));

	this->innerContainer->runAction(Sequence::create(actions));
}

void MayanDoor::lock(bool animate)
{
	super::lock(animate);

	this->saveObjectState(MayanDoor::SaveKeyUnlocked, Value(false));

	this->toggleHackable(true);
	
	float currentProgress = this->doorContainer->getPositionX() / MayanDoor::DoorOpenDelta;

	if (animate)
	{
		this->doorContainer->stopAllActions();
		this->doorContainer->runAction(MoveTo::create(5.0f * currentProgress, Vec2::ZERO));
		this->doorContainer->runAction(RotateTo::create(5.0f * currentProgress, 0.0f));

		if (this->doorOpenSound != nullptr)
		{
			this->doorOpenSound->play();
		}
	}
	else
	{
		this->doorContainer->setPosition(Vec2::ZERO);
		this->doorContainer->setRotation(0.0f);
	}
}

void MayanDoor::unlock(bool animate)
{
	super::unlock(animate);

	this->broadcastMapEvent(MayanDoor::MapEventPuzzleComplete, ValueMap());
	this->saveObjectState(MayanDoor::SaveKeyUnlocked, Value(true));

	this->toggleHackable(false);

	float currentProgress = 1.0f - this->doorContainer->getPositionX() / MayanDoor::DoorOpenDelta;

	if (animate)
	{
		this->doorContainer->stopAllActions();
		this->doorContainer->runAction(MoveTo::create(5.0f * currentProgress, Vec2(MayanDoor::DoorOpenDelta, 0.0f)));
		this->doorContainer->runAction(RotateTo::create(5.0f * currentProgress, 180.0f));
		
		if (this->doorOpenSound != nullptr)
		{
			this->doorOpenSound->play();
		}
	}
	else
	{
		this->doorContainer->setPosition(Vec2(MayanDoor::DoorOpenDelta, 0.0f));
		this->doorContainer->setRotation(180.0f);
	}

	this->enable();
}
