#include "EFURTransportationBehavior.h"

#include "Engine/Dialogue/DialogueOption.h"
#include "Engine/Events/ObjectEvents.h"
#include "Engine/Maps/GameObject.h"
#include "Events/DialogueEvents.h"
#include "Entities/Platformer/Helpers/EndianForest/Scrappy.h"
#include "Entities/Platformer/PlatformerEntity.h"
#include "Entities/Platformer/Squally/Squally.h"
#include "Objects/Platformer/Interactables/Doors/Portal.h"
#include "Scenes/Platformer/Components/Entities/Dialogue/EntityDialogueBehavior.h"
#include "Scenes/Platformer/Components/Entities/Friendly/LookAtSquallyBehavior.h"
#include "Scenes/Platformer/Dialogue/DialogueSet.h"
#include "Scenes/Platformer/Dialogue/Voices.h"

#include "Resources/SoundResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

const std::string EFURTransportationBehavior::MapKey = "ef-ur-transportation";
const std::string EFURTransportationBehavior::QuestTagBackPortal = "back-portal";
const std::string EFURTransportationBehavior::QuestTagLeavePortal = "leave-portal";

EFURTransportationBehavior* EFURTransportationBehavior::create(GameObject* owner)
{
	EFURTransportationBehavior* instance = new EFURTransportationBehavior(owner);

	instance->autorelease();

	return instance;
}

EFURTransportationBehavior::EFURTransportationBehavior(GameObject* owner) : super(owner)
{
	this->entity = dynamic_cast<PlatformerEntity*>(owner);
	this->squally = nullptr;
	this->innerChoices = nullptr;
	this->backPortal = nullptr;
	this->leavePortal = nullptr;

	if (this->entity == nullptr)
	{
		this->invalidate();
	}
}

EFURTransportationBehavior::~EFURTransportationBehavior()
{
}

void EFURTransportationBehavior::onLoad()
{
	ObjectEvents::WatchForObject<Squally>(this, [=](Squally* squally)
	{
		this->squally = squally;
	}, Squally::MapKey);

	ObjectEvents::WatchForObject<Scrappy>(this, [=](Scrappy* scrappy)
	{
		this->scrappy = scrappy;
	}, Scrappy::MapKey);

	ObjectEvents::WatchForObject<Portal>(this, [=](Portal* portal)
	{
		this->backPortal = portal;
	}, EFURTransportationBehavior::QuestTagBackPortal);

	ObjectEvents::WatchForObject<Portal>(this, [=](Portal* portal)
	{
		this->leavePortal = portal;
	}, EFURTransportationBehavior::QuestTagLeavePortal);

	this->entity->watchForComponent<EntityDialogueBehavior>([=](EntityDialogueBehavior* interactionBehavior)
	{
		this->innerChoices = DialogueSet::create(DialogueEvents::DialogueVisualArgs(
			DialogueBox::DialogueDock::Bottom,
			DialogueBox::DialogueAlignment::Left,
			DialogueEvents::BuildPreviewNode(&this->scrappy, false),
			DialogueEvents::BuildPreviewNode(&this->entity, true)
		));

		this->innerChoices->addDialogueOption(DialogueOption::create(
			Strings::Platformer_Dialogue_Transportation_Yes::create(),
			[=]()
			{
				interactionBehavior->setActiveDialogueSet(interactionBehavior->getMainDialogueSet(), false);

				if (this->leavePortal != nullptr)
				{
					this->leavePortal->loadMap();
				}
			}),
			1.0f
		);

		innerChoices->addDialogueOption(DialogueOption::create(
			Strings::Platformer_Dialogue_Transportation_No::create(),
			[=]()
			{
				interactionBehavior->setActiveDialogueSet(interactionBehavior->getMainDialogueSet(), false);
			}),
			0.9f
		);

		interactionBehavior->addDialogueSet(innerChoices);

		interactionBehavior->getMainDialogueSet()->addDialogueOption(DialogueOption::create(
			Strings::Platformer_Dialogue_Transportation_AreWeThereYet::create(),
			[=]()
			{
				DialogueEvents::TriggerOpenDialogue(DialogueEvents::DialogueOpenArgs(
					Strings::Platformer_Dialogue_Transportation_AreYouReady::create(),
					DialogueEvents::DialogueVisualArgs(
						DialogueBox::DialogueDock::Bottom,
						DialogueBox::DialogueAlignment::Right,
						DialogueEvents::BuildPreviewNode(&this->scrappy, false),
						DialogueEvents::BuildPreviewNode(&this->entity, true)
					),
					[=]()
					{
						interactionBehavior->setActiveDialogueSet(innerChoices);
					},
					Voices::GetNextVoiceQuestion()
				));
			}),
			1.0f
		);

		interactionBehavior->getMainDialogueSet()->addDialogueOption(DialogueOption::create(
			Strings::Platformer_Dialogue_Transportation_IForgotSomething::create(),
			[=]()
			{
				DialogueEvents::TriggerOpenDialogue(DialogueEvents::DialogueOpenArgs(
					Strings::Platformer_Dialogue_Transportation_Seriously::create(),
					DialogueEvents::DialogueVisualArgs(
						DialogueBox::DialogueDock::Bottom,
						DialogueBox::DialogueAlignment::Right,
						DialogueEvents::BuildPreviewNode(&this->scrappy, false),
						DialogueEvents::BuildPreviewNode(&this->entity, true)
					),
					[=]()
					{
						if (this->backPortal != nullptr)
						{
							this->backPortal->loadMap();
						}
					},
					Voices::GetNextVoiceQuestion()
				));
			}),
			0.9f
		);
	});
}
