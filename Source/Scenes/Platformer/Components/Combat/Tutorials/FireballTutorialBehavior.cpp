#include "FireballTutorialBehavior.h"

#include "cocos/2d/CCActionEase.h"
#include "cocos/2d/CCActionInstant.h"
#include "cocos/2d/CCActionInterval.h"
#include "cocos/base/CCEventCustom.h"
#include "cocos/base/CCEventListenerCustom.h"
#include "cocos/base/CCValue.h"

#include "Engine/Animations/SmartAnimationNode.h"
#include "Engine/Dialogue/SpeechBubble.h"
#include "Engine/Events/ObjectEvents.h"
#include "Engine/Events/HackableEvents.h"
#include "Engine/Sound/WorldSound.h"
#include "Engine/Utils/GameUtils.h"
#include "Entities/Platformer/Helpers/EndianForest/Scrappy.h"
#include "Entities/Platformer/PlatformerEntity.h"
#include "Entities/Platformer/Squally/Squally.h"
#include "Events/CombatEvents.h"
#include "Events/DialogueEvents.h"
#include "Events/PlatformerEvents.h"
#include "Objects/Platformer/Combat/Projectiles/Fireball/Fireball.h"
#include "Scenes/Platformer/Components/Entities/Dialogue/EntityDialogueBehavior.h"
#include "Scenes/Platformer/Components/Entities/Helpers/Scrappy/Combat/ScrappyHackableCueBehavior.h"
#include "Scenes/Platformer/Dialogue/Voices.h"
#include "Scenes/Platformer/Hackables/ScrappyClippy.h"

#include "Resources/SoundResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

const std::string FireballTutorialBehavior::MapKey = "fireball-tutorial";

FireballTutorialBehavior* FireballTutorialBehavior::create(GameObject* owner)
{
	FireballTutorialBehavior* instance = new FireballTutorialBehavior(owner);

	instance->autorelease();

	return instance;
}

FireballTutorialBehavior::FireballTutorialBehavior(GameObject* owner) : super(owner)
{
	this->entity = dynamic_cast<PlatformerEntity*>(owner);
	this->scrappy = nullptr;
	this->squally = nullptr;
	this->hasRunTutorial = false;
	this->tutorialInterrupt = false;

	if (this->entity == nullptr)
	{
		this->invalidate();
	}
}

FireballTutorialBehavior::~FireballTutorialBehavior()
{
}

void FireballTutorialBehavior::onLoad()
{
	this->addEventListenerIgnorePause(EventListenerCustom::create(CombatEvents::EventProjectileSpawned, [=](EventCustom* eventCustom)
	{
		CombatEvents::ProjectileSpawnedArgs* args = static_cast<CombatEvents::ProjectileSpawnedArgs*>(eventCustom->getData());

		if (args != nullptr && dynamic_cast<Fireball*>(args->projectile) != nullptr)
		{
			if (!this->hasRunTutorial)
			{
				args->projectile->registerClippyOnto(Fireball::HackIdentifierFireballSpeed, [=]
				{
					return ScrappyClippy::create(Strings::Menus_Hacking_Abilities_Abilities_Fireball_ApplySpeed_FireballClippy::create()
						->setStringReplacementVariables(Strings::Menus_Hacking_Abilities_Abilities_Fireball_ApplySpeed_StopFireball::create()));
				});

				this->runTutorial();
			}
		}
	}));

	this->addEventListenerIgnorePause(EventListenerCustom::create(HackableEvents::EventHackerModeEnable, [=](EventCustom* eventCustom)
	{
		if (this->hasRunTutorial)
		{
			this->tutorialInterrupt = true;
		}
	}));

	ObjectEvents::WatchForObject<Scrappy>(this, [=](Scrappy* scrappy)
	{
		this->scrappy = scrappy;

		this->scrappy->watchForComponent<ScrappyHackableCueBehavior>([=](ScrappyHackableCueBehavior* scrappyHackableCueBehavior)
		{
			scrappyHackableCueBehavior->disable();
		});
	}, Scrappy::MapKey);

	ObjectEvents::WatchForObject<Squally>(this, [=](Squally* squally)
	{
		this->squally = squally;
	}, Squally::MapKey);
}

void FireballTutorialBehavior::onDisable()
{
	super::onDisable();
}

void FireballTutorialBehavior::runTutorial()
{
	if (this->hasRunTutorial || this->scrappy == nullptr)
	{
		return;
	}

	static const float TutorialDelay = 0.25f;

	this->hasRunTutorial = true;

	this->runAction(Sequence::create(
		DelayTime::create(TutorialDelay),
		CallFunc::create([=]()
		{
			if (this->tutorialInterrupt)
			{
				return;
			}

			CombatEvents::TriggerPauseTimelineCinematic();
			HackableEvents::TriggerDisallowHackerMode();

			DialogueEvents::TriggerOpenDialogue(DialogueEvents::DialogueOpenArgs(
				Strings::Menus_Hacking_HackWarning::create(),
				DialogueEvents::DialogueVisualArgs(
					DialogueBox::DialogueDock::Top,
					DialogueBox::DialogueAlignment::Left,
					DialogueEvents::BuildPreviewNode(&this->scrappy, false),
					DialogueEvents::BuildPreviewNode(&this->squally, true)
				),
				[=]()
				{
					CombatEvents::TriggerResumeTimelineCinematic();
					HackableEvents::TriggerAllowHackerMode();
				},
				Voices::GetNextVoiceMedium(Voices::VoiceType::Droid)
			));
		}),
		nullptr
	));
}
