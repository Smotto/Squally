#include "TrollInTheDungeon.h"

#include "cocos/2d/CCActionEase.h"
#include "cocos/2d/CCActionInstant.h"
#include "cocos/2d/CCActionInterval.h"
#include "cocos/base/CCEventCustom.h"
#include "cocos/base/CCEventListenerCustom.h"
#include "cocos/base/CCValue.h"

#include "Engine/Animations/SmartAnimationNode.h"
#include "Engine/Dialogue/SpeechBubble.h"
#include "Engine/Events/ObjectEvents.h"
#include "Engine/Events/QuestEvents.h"
#include "Engine/Utils/GameUtils.h"
#include "Entities/Platformer/PlatformerEntity.h"
#include "Entities/Platformer/Squally/Squally.h"
#include "Events/DialogueEvents.h"
#include "Events/PlatformerEvents.h"
#include "Objects/Platformer/Cinematic/CinematicMarker.h"
#include "Objects/Platformer/Interactables/Doors/Portal.h"
#include "Scenes/Platformer/Components/Entities/Dialogue/EntityDialogueBehavior.h"
#include "Scenes/Platformer/State/StateKeys.h"

#include "Resources/SoundResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

const std::string TrollInTheDungeon::MapKeyQuest = "troll-in-the-dungeon";
const std::string TrollInTheDungeon::TagDialoguePause = "dialogue-pause";
const std::string TrollInTheDungeon::TagExit = "exit";
const std::string TrollInTheDungeon::TagExitPortal = "exit-portal";

TrollInTheDungeon* TrollInTheDungeon::create(GameObject* owner, QuestLine* questLine)
{
	TrollInTheDungeon* instance = new TrollInTheDungeon(owner, questLine);

	instance->autorelease();

	return instance;
}

TrollInTheDungeon::TrollInTheDungeon(GameObject* owner, QuestLine* questLine) : super(owner, questLine, TrollInTheDungeon::MapKeyQuest, false)
{
	this->mage = dynamic_cast<PlatformerEntity*>(owner);
}

TrollInTheDungeon::~TrollInTheDungeon()
{
}

void TrollInTheDungeon::onLoad(QuestState questState)
{
	ObjectEvents::WatchForObject<Squally>(this, [=](Squally* squally)
	{
		this->squally = squally;
	}, Squally::MapKey);
	
	if (questState == QuestState::Complete)
	{
		this->defer([=]()
		{
			this->mage->despawn();
		});
	}
}

void TrollInTheDungeon::onActivate(bool isActiveThroughSkippable, bool isInitialActivation)
{
	ObjectEvents::WatchForObject<Portal>(this, [=](Portal* exitPortal)
	{
		this->exitPortal = exitPortal;
		this->exitPortal->disable();

		this->runAction(Sequence::create(
			DelayTime::create(0.5f),
			CallFunc::create([=]()
			{
				this->runChatSequence();
			}),
			nullptr
		));
	}, TrollInTheDungeon::TagExitPortal);
}

void TrollInTheDungeon::onComplete()
{
}

void TrollInTheDungeon::onSkipped()
{
	this->removeAllListeners();
}

void TrollInTheDungeon::runChatSequence()
{
	ObjectEvents::WatchForObject<CinematicMarker>(this, [=](CinematicMarker* cinematicMarker)
	{
		this->mage->setState(StateKeys::CinematicSourceX, Value(GameUtils::getWorldCoords3D(this->mage).x));
		this->mage->setState(StateKeys::CinematicDestinationX, Value(cinematicMarker->getPositionX()));
		PlatformerEvents::TriggerCinematicHijack();

		this->mage->listenForStateWriteOnce(StateKeys::CinematicDestinationReached, [=](Value value)
		{
			this->mage->getComponent<EntityDialogueBehavior>([=](EntityDialogueBehavior* interactionBehavior)
			{
				interactionBehavior->getSpeechBubble()->runDialogue(Strings::Platformer_Quests_EndianForest_RescueGuano_Alder_A_TrollInTheDungeon::create(), SoundResources::Platformer_Entities_Generic_ChatterAnnoyed1);
			});

			this->complete();

			this->mage->runAction(Sequence::create(
				DelayTime::create(1.0f),
				CallFunc::create([=]()
				{
					PlatformerEvents::TriggerCinematicRestore();
				}),
				DelayTime::create(1.0f),
				CallFunc::create([=]()
				{
					this->runChatSequencePt2();
				}),
				nullptr
			));
		});
	}, TrollInTheDungeon::TagDialoguePause);
}

void TrollInTheDungeon::runChatSequencePt2()
{
	ObjectEvents::WatchForObject<CinematicMarker>(this, [=](CinematicMarker* cinematicMarker)
	{
		this->mage->setState(StateKeys::CinematicSourceX, Value(GameUtils::getWorldCoords3D(this->mage).x));
		this->mage->setState(StateKeys::CinematicDestinationX, Value(GameUtils::getWorldCoords3D(cinematicMarker).x));
		
		this->mage->getComponent<EntityDialogueBehavior>([=](EntityDialogueBehavior* interactionBehavior)
		{
			interactionBehavior->getSpeechBubble()->hideDialogue();
		});

		this->mage->listenForStateWriteOnce(StateKeys::CinematicDestinationReached, [=](Value value)
		{
			this->mage->runAction(Sequence::create(
				FadeTo::create(0.5f, 0),
				CallFunc::create([=]()
				{
					this->mage->despawn();

					if (this->exitPortal != nullptr)
					{
						this->exitPortal->enable();
					}
				}),
				nullptr
			));
		});

	}, TrollInTheDungeon::TagExit);
}
