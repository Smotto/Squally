#include "BeatTutorialA.h"

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
#include "Scenes/Platformer/Components/Entities/Friendly/Hexus/EndianForest/Gauntlet/TutorialABehavior.h"

#include "Resources/HexusResources.h"
#include "Resources/SoundResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

const std::string BeatTutorialA::MapKeyQuest = "beat-tutorial-A";
const std::string BeatTutorialA::WinLossTrackIdentifier = "Tutorial-A";
const std::string BeatTutorialA::QuestPortalTag = "quest-portal";

BeatTutorialA* BeatTutorialA::create(GameObject* owner, QuestLine* questLine)
{
	BeatTutorialA* instance = new BeatTutorialA(owner, questLine);

	instance->autorelease();

	return instance;
}

BeatTutorialA::BeatTutorialA(GameObject* owner, QuestLine* questLine) : super(owner, questLine, BeatTutorialA::MapKeyQuest, false)
{
	this->mage = dynamic_cast<PlatformerEntity*>(owner);
}

BeatTutorialA::~BeatTutorialA()
{
}

void BeatTutorialA::onLoad(QuestState questState)
{
	this->defer([=]()
	{
		TutorialABehavior* tutorialBehavior = TutorialABehavior::create(this->mage);

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
	}, BeatTutorialA::QuestPortalTag);
}

void BeatTutorialA::onActivate(bool isActiveThroughSkippable, bool isInitialActivation)
{
}

void BeatTutorialA::onComplete()
{
	if (this->portal != nullptr)
	{
		this->portal->openPortal(true);
	}
}

void BeatTutorialA::onSkipped()
{
	this->removeAllListeners();
}
