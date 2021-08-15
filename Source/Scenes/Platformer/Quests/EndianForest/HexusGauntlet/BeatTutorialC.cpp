#include "BeatTutorialC.h"

#include "cocos/2d/CCActionEase.h"
#include "cocos/2d/CCActionInstant.h"
#include "cocos/2d/CCActionInterval.h"
#include "cocos/base/CCEventCustom.h"
#include "cocos/base/CCEventListenerCustom.h"
#include "cocos/base/CCValue.h"

#include "Engine/Events/ObjectEvents.h"
#include "Entities/Platformer/Squally/Squally.h"
#include "Events/DialogueEvents.h"
#include "Events/PlatformerEvents.h"
#include "Objects/Platformer/Interactables/Doors/MagePortals/MagePortal.h"
#include "Scenes/Platformer/Components/Entities/Friendly/Hexus/EndianForest/Gauntlet/TutorialCBehavior.h"

#include "Resources/HexusResources.h"
#include "Resources/SoundResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

const std::string BeatTutorialC::MapKeyQuest = "beat-tutorial-C";
const std::string BeatTutorialC::WinLossTrackIdentifier = "Tutorial-C";
const std::string BeatTutorialC::QuestPortalTag = "quest-portal";

BeatTutorialC* BeatTutorialC::create(GameObject* owner, QuestLine* questLine)
{
	BeatTutorialC* instance = new BeatTutorialC(owner, questLine);

	instance->autorelease();

	return instance;
}

BeatTutorialC::BeatTutorialC(GameObject* owner, QuestLine* questLine) : super(owner, questLine, BeatTutorialC::MapKeyQuest, false)
{
	this->mage = dynamic_cast<PlatformerEntity*>(owner);
}

BeatTutorialC::~BeatTutorialC()
{
}

void BeatTutorialC::onLoad(QuestState questState)
{
	this->defer([=]()
	{
		TutorialCBehavior* tutorialBehavior = TutorialCBehavior::create(this->mage);

		tutorialBehavior->registerWinCallback([=]()
		{
			this->complete();
		});
		
    	this->mage->attachComponent(tutorialBehavior);
	});

	ObjectEvents::WatchForObject<Squally>(this, [=](Squally* squally)
	{
		this->squally = squally;
	}, Squally::MapKey);

	ObjectEvents::WatchForObject<MagePortal>(this, [=](MagePortal* portal)
	{
		this->portal = portal;
		
		if (questState == QuestState::Complete)
		{
			this->portal->openPortal(true);
		}
		else
		{
			this->portal->closePortal(true);
		}
	}, BeatTutorialC::QuestPortalTag);
}

void BeatTutorialC::onActivate(bool isActiveThroughSkippable)
{
}

void BeatTutorialC::onComplete()
{
	if (this->portal != nullptr)
	{
		this->portal->openPortal(true);
	}
}

void BeatTutorialC::onSkipped()
{
	this->removeAllListeners();
}
