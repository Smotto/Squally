#include "QueenEliseInterim.h"

#include "cocos/2d/CCActionEase.h"
#include "cocos/2d/CCActionInstant.h"
#include "cocos/2d/CCActionInterval.h"
#include "cocos/2d/CCSprite.h"
#include "cocos/base/CCEventCustom.h"
#include "cocos/base/CCEventListenerCustom.h"
#include "cocos/base/CCValue.h"

#include "Engine/Camera/GameCamera.h"
#include "Engine/Events/ObjectEvents.h"
#include "Engine/Quests/QuestLine.h"
#include "Engine/Save/SaveManager.h"
#include "Entities/Platformer/Helpers/EndianForest/Guano.h"
#include "Entities/Platformer/Helpers/EndianForest/Scrappy.h"
#include "Entities/Platformer/Npcs/FirewallFissure/QueenElise.h"
#include "Entities/Platformer/Squally/Squally.h"
#include "Events/NotificationEvents.h"
#include "Events/PlatformerEvents.h"
#include "Objects/Platformer/Interactables/Doors/Portal.h"
#include "Scenes/Platformer/Components/Entities/Dialogue/EntityDialogueBehavior.h"
#include "Scenes/Platformer/Components/Entities/Visual/EntityQuestVisualBehavior.h"
#include "Scenes/Platformer/Dialogue/Voices.h"
#include "Scenes/Platformer/Hackables/HackFlags.h"
#include "Scenes/Platformer/Inventory/Items/PlatformerItems.h"
#include "Scenes/Platformer/Objectives/ObjectiveKeys.h"
#include "Scenes/Platformer/Objectives/Objectives.h"
#include "Scenes/Platformer/Save/SaveKeys.h"

#include "Resources/EntityResources.h"
#include "Resources/ItemResources.h"
#include "Resources/SoundResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

const std::string QueenEliseInterim::MapKeyQuest = "queen-elise-interim";

QueenEliseInterim* QueenEliseInterim::create(GameObject* owner, QuestLine* questLine)
{
	QueenEliseInterim* instance = new QueenEliseInterim(owner, questLine);

	instance->autorelease();

	return instance;
}

QueenEliseInterim::QueenEliseInterim(GameObject* owner, QuestLine* questLine) : super(owner, questLine, QueenEliseInterim::MapKeyQuest, false)
{
}

QueenEliseInterim::~QueenEliseInterim()
{
}

void QueenEliseInterim::onLoad(QuestState questState)
{
	ObjectEvents::WatchForObject<Guano>(this, [=](Guano* guano)
	{
		this->guano = guano;
	}, Guano::MapKey);

	ObjectEvents::WatchForObject<Scrappy>(this, [=](Scrappy* scrappy)
	{
		this->scrappy = scrappy;
	}, Scrappy::MapKey);

	ObjectEvents::WatchForObject<Squally>(this, [=](Squally* squally)
	{
		this->squally = squally;
	}, Squally::MapKey);

	ObjectEvents::WatchForObject<QueenElise>(this, [=](QueenElise* queenElise)
	{
		this->queenElise = queenElise;

		if (questState == QuestState::Active || questState == QuestState::ActiveThroughSkippable)
		{
			this->setPostText();
		}
	});
}

void QueenEliseInterim::onActivate(bool isActiveThroughSkippable, bool isInitialActivation)
{
}

void QueenEliseInterim::onComplete()
{	
	Objectives::SetCurrentObjective(ObjectiveKeys::FFCraftBomb);
	PlatformerEvents::TriggerGiveItems(PlatformerEvents::GiveItemsArgs({ UnstableConcoction::create(), UnstableElement::create() }));
}

void QueenEliseInterim::onSkipped()
{
	this->removeAllListeners();
}

void QueenEliseInterim::setPostText()
{
	this->defer([=]()
	{
		this->queenElise->watchForComponent<EntityDialogueBehavior>([=](EntityDialogueBehavior* interactionBehavior)
		{
			interactionBehavior->enqueuePretext(DialogueEvents::DialogueOpenArgs(
				Strings::Platformer_Quests_FirewallFissure_DefeatAsmodeus_QueenElise_M_GoodWork::create(),
				DialogueEvents::DialogueVisualArgs(
					DialogueBox::DialogueDock::Bottom,
					DialogueBox::DialogueAlignment::Right,
					DialogueEvents::BuildPreviewNode(&this->squally, false),
					DialogueEvents::BuildPreviewNode(&this->queenElise, true)
				),
				[=]()
				{
				},
				Voices::GetNextVoiceLong(),
				true
			));
		});
	});
}
