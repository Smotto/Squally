#include "TalkToMages.h"

#include "cocos/2d/CCActionEase.h"
#include "cocos/2d/CCActionInstant.h"
#include "cocos/2d/CCActionInterval.h"
#include "cocos/base/CCEventCustom.h"
#include "cocos/base/CCEventListenerCustom.h"
#include "cocos/base/CCValue.h"

#include "Engine/Dialogue/SpeechBubble.h"
#include "Engine/Events/ObjectEvents.h"
#include "Engine/Events/QuestEvents.h"
#include "Engine/Save/SaveManager.h"
#include "Entities/Platformer/Helpers/EndianForest/Scrappy.h"
#include "Entities/Platformer/Npcs/Mages/Sarude.h"
#include "Entities/Platformer/Squally/Squally.h"
#include "Events/NotificationEvents.h"
#include "Events/PlatformerEvents.h"
#include "Scenes/Platformer/Components/Entities/Dialogue/EntityDialogueBehavior.h"
#include "Scenes/Platformer/Dialogue/DialogueSet.h"
#include "Scenes/Platformer/Dialogue/Voices.h"
#include "Scenes/Platformer/Hackables/HackFlags.h"
#include "Scenes/Platformer/Objectives/ObjectiveKeys.h"
#include "Scenes/Platformer/Objectives/Objectives.h"
#include "Scenes/Platformer/Quests/EndianForest/SaveTown/FightGorgon.h"
#include "Scenes/Platformer/Save/SaveKeys.h"

#include "Resources/ObjectResources.h"
#include "Resources/SoundResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

const std::string TalkToMages::MapKeyQuest = "talk-to-mages";

TalkToMages* TalkToMages::create(GameObject* owner, QuestLine* questLine)
{
	TalkToMages* instance = new TalkToMages(owner, questLine);

	instance->autorelease();

	return instance;
}

TalkToMages::TalkToMages(GameObject* owner, QuestLine* questLine) : super(owner, questLine, TalkToMages::MapKeyQuest, false)
{
}

TalkToMages::~TalkToMages()
{
}

void TalkToMages::onLoad(QuestState questState)
{
	ObjectEvents::WatchForObject<Squally>(this, [=](Squally* squally)
	{
		this->squally = squally;
	}, Squally::MapKey);
	
	ObjectEvents::WatchForObject<Scrappy>(this, [=](Scrappy* scrappy)
	{
		this->scrappy = scrappy;
	}, Scrappy::MapKey);

	ObjectEvents::WatchForObject<Sarude>(this, [=](Sarude* sarude)
	{
		this->sarude = sarude;
		this->setPostText();
	}, Sarude::MapKey);
}

void TalkToMages::onActivate(bool isActiveThroughSkippable)
{
	this->listenForMapEventOnce(TalkToMages::MapKeyQuest, [=](ValueMap args)
	{
		this->runCinematicSequencePart1();
	});
}

void TalkToMages::onComplete()
{
	Objectives::SetCurrentObjective(ObjectiveKeys::EFAmbush);
	
	this->setPostText();
}

void TalkToMages::onSkipped()
{
}

void TalkToMages::runCinematicSequencePart1()
{
	DialogueEvents::TriggerOpenDialogue(DialogueEvents::DialogueOpenArgs(
		Strings::Platformer_Quests_EndianForest_SaveTown_Sarude_A_YouMadeIt::create(),
		DialogueEvents::DialogueVisualArgs(
			DialogueBox::DialogueDock::Bottom,
			DialogueBox::DialogueAlignment::Right,
			DialogueEvents::BuildPreviewNode(&this->squally, false),
			DialogueEvents::BuildPreviewNode(&this->sarude, true)
		),
		[=]()
		{
			this->runCinematicSequencePart2();
		},
		Voices::GetNextVoiceMedium(),
		false
	));
}

void TalkToMages::runCinematicSequencePart2()
{
	DialogueEvents::TriggerOpenDialogue(DialogueEvents::DialogueOpenArgs(
		Strings::Platformer_Quests_EndianForest_SaveTown_Sarude_B_WhatsThePlan::create(),
		DialogueEvents::DialogueVisualArgs(
			DialogueBox::DialogueDock::Bottom,
			DialogueBox::DialogueAlignment::Left,
			DialogueEvents::BuildPreviewNode(&this->scrappy, false),
			DialogueEvents::BuildPreviewNode(&this->sarude, true)
		),
		[=]()
		{
			this->runCinematicSequencePart3();
		},
		Voices::GetNextVoiceShort(Voices::VoiceType::Droid),
		false
	));
}

void TalkToMages::runCinematicSequencePart3()
{
	DialogueEvents::TriggerOpenDialogue(DialogueEvents::DialogueOpenArgs(
		Strings::Platformer_Quests_EndianForest_SaveTown_Sarude_C_Infiltrate::create(),
		DialogueEvents::DialogueVisualArgs(
			DialogueBox::DialogueDock::Bottom,
			DialogueBox::DialogueAlignment::Right,
			DialogueEvents::BuildPreviewNode(&this->squally, false),
			DialogueEvents::BuildPreviewNode(&this->sarude, true)
		),
		[=]()
		{
			this->runCinematicSequencePart4();
		},
		Voices::GetNextVoiceMedium(),
		false
	));
}

void TalkToMages::runCinematicSequencePart4()
{
	DialogueEvents::TriggerOpenDialogue(DialogueEvents::DialogueOpenArgs(
		Strings::Platformer_Quests_EndianForest_SaveTown_Sarude_D_GoNow::create(),
		DialogueEvents::DialogueVisualArgs(
			DialogueBox::DialogueDock::Bottom,
			DialogueBox::DialogueAlignment::Right,
			DialogueEvents::BuildPreviewNode(&this->squally, false),
			DialogueEvents::BuildPreviewNode(&this->sarude, true)
		),
		[=]()
		{
			this->complete();
		},
		Voices::GetNextVoiceMedium(),
		true
	));
}

void TalkToMages::setPostText()
{
	if (this->sarude == nullptr)
	{
		return;
	}

	if (QuestTask::getQuestStateForTask(this->questLine, FightGorgon::MapKeyQuest) == QuestState::Complete)
	{
		return;
	}
	
	this->defer([=]()
	{
		this->sarude->watchForComponent<EntityDialogueBehavior>([=](EntityDialogueBehavior* interactionBehavior)
		{
			interactionBehavior->enqueuePretext(DialogueEvents::DialogueOpenArgs(
				Strings::Platformer_Quests_EndianForest_SaveTown_Sarude_D_GoNow::create(),
				DialogueEvents::DialogueVisualArgs(
					DialogueBox::DialogueDock::Bottom,
					DialogueBox::DialogueAlignment::Right,
					DialogueEvents::BuildPreviewNode(&this->squally, false),
					DialogueEvents::BuildPreviewNode(&this->sarude, true)
				),
				[=]()
				{
					this->setPostText();
				},
				Voices::GetNextVoiceMedium(),
				true
			));
		});
	});
}
