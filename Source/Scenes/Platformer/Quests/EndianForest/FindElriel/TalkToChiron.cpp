#include "TalkToChiron.h"

#include "cocos/2d/CCActionEase.h"
#include "cocos/2d/CCActionInstant.h"
#include "cocos/2d/CCActionInterval.h"
#include "cocos/base/CCEventCustom.h"
#include "cocos/base/CCEventListenerCustom.h"
#include "cocos/base/CCValue.h"

#include "Engine/Events/ObjectEvents.h"
#include "Engine/Events/QuestEvents.h"
#include "Entities/Platformer/Npcs/EndianForest/Chiron.h"
#include "Entities/Platformer/Squally/Squally.h"
#include "Events/PlatformerEvents.h"
#include "Scenes/Platformer/Components/Entities/Dialogue/EntityDialogueBehavior.h"
#include "Scenes/Platformer/Dialogue/Voices.h"

#include "Resources/SoundResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

const std::string TalkToChiron::MapKeyQuest = "talk-to-chiron";

TalkToChiron* TalkToChiron::create(GameObject* owner, QuestLine* questLine)
{
	TalkToChiron* instance = new TalkToChiron(owner, questLine);

	instance->autorelease();

	return instance;
}

TalkToChiron::TalkToChiron(GameObject* owner, QuestLine* questLine) : super(owner, questLine, TalkToChiron::MapKeyQuest, false)
{
}

TalkToChiron::~TalkToChiron()
{
}

void TalkToChiron::onLoad(QuestState questState)
{
	ObjectEvents::WatchForObject<Chiron>(this, [=](Chiron* chiron)
	{
		this->chiron = chiron;
	}, Chiron::MapKey);

	ObjectEvents::WatchForObject<Squally>(this, [=](Squally* squally)
	{
		this->squally = squally;
	}, Squally::MapKey);
}

void TalkToChiron::onActivate(bool isActiveThroughSkippable, bool isInitialActivation)
{
	this->runCinematicSequence();
}

void TalkToChiron::onComplete()
{
}

void TalkToChiron::onSkipped()
{
	this->removeAllListeners();
}

void TalkToChiron::runCinematicSequence()
{
	if (this->chiron == nullptr)
	{
		return;
	}
	
	this->chiron->watchForComponent<EntityDialogueBehavior>([=](EntityDialogueBehavior* interactionBehavior)
	{
		// Pre-text chain
		interactionBehavior->enqueuePretext(DialogueEvents::DialogueOpenArgs(
			Strings::Platformer_Quests_EndianForest_FindElriel_Chiron_G_TempleDoor::create()
				->setStringReplacementVariables(Strings::Platformer_Entities_Names_Npcs_EndianForest_Elriel::create()),
			DialogueEvents::DialogueVisualArgs(
				DialogueBox::DialogueDock::Bottom,
				DialogueBox::DialogueAlignment::Right,
				DialogueEvents::BuildPreviewNode(&this->chiron, false),
				DialogueEvents::BuildPreviewNode(&this->squally, true)
			),
			[=]()
			{
			},
			Voices::GetNextVoiceMedium(),
			false
		));
	});
}
