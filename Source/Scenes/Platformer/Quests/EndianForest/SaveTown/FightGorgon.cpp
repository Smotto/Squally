#include "FightGorgon.h"

#include "cocos/2d/CCActionEase.h"
#include "cocos/2d/CCActionInstant.h"
#include "cocos/2d/CCActionInterval.h"
#include "cocos/base/CCEventCustom.h"
#include "cocos/base/CCEventListenerCustom.h"
#include "cocos/base/CCValue.h"

#include "Engine/Animations/SmartAnimationNode.h"
#include "Engine/Animations/SmartAnimationSequenceNode.h"
#include "Engine/Dialogue/DialogueOption.h"
#include "Engine/Dialogue/SpeechBubble.h"
#include "Engine/Events/ObjectEvents.h"
#include "Engine/Events/QuestEvents.h"
#include "Engine/Save/SaveManager.h"
#include "Engine/Sound/WorldSound.h"
#include "Engine/Utils/GameUtils.h"
#include "Entities/Platformer/Enemies/EndianForest/Gorgon.h"
#include "Entities/Platformer/Npcs/BallmerPeaks/Aster.h"
#include "Entities/Platformer/Npcs/CastleValgrind/Merlin.h"
#include "Entities/Platformer/Npcs/DaemonsHallow/Igneus.h"
#include "Entities/Platformer/Npcs/DataMines/Alder.h"
#include "Entities/Platformer/Npcs/DataMines/Sarude.h"
#include "Entities/Platformer/Squally/Squally.h"
#include "Events/NotificationEvents.h"
#include "Events/PlatformerEvents.h"
#include "Objects/Platformer/Interactables/Ram/Ram.h"
#include "Objects/Platformer/Cinematic/CinematicMarker.h"
#include "Scenes/Platformer/Components/Entities/Dialogue/EntityDialogueBehavior.h"
#include "Scenes/Platformer/Components/Entities/Cinematic/MageCastBehavior.h"
#include "Scenes/Platformer/Components/Entities/Friendly/LookAtSquallyBehavior.h"
#include "Scenes/Platformer/Components/Entities/Stats/EntityHealthBehavior.h"
#include "Scenes/Platformer/Dialogue/DialogueSet.h"
#include "Scenes/Platformer/Hackables/HackFlags.h"
#include "Scenes/Platformer/Objectives/ObjectiveKeys.h"
#include "Scenes/Platformer/Objectives/Objectives.h"
#include "Scenes/Platformer/Save/SaveKeys.h"
#include "Scenes/Platformer/State/StateKeys.h"

#include "Resources/FXResources.h"
#include "Resources/ObjectResources.h"
#include "Resources/SoundResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

const std::string FightGorgon::MapKeyQuest = "fight-gorgon";
const std::string FightGorgon::ForceFieldTag = "force-field";
const std::string FightGorgon::MarkerTagBack = "walk-to-back";
const std::string FightGorgon::MarkerTagFront = "walk-to-front";

FightGorgon* FightGorgon::create(GameObject* owner, QuestLine* questLine)
{
	FightGorgon* instance = new FightGorgon(owner, questLine);

	instance->autorelease();

	return instance;
}

FightGorgon::FightGorgon(GameObject* owner, QuestLine* questLine) : super(owner, questLine, FightGorgon::MapKeyQuest, false)
{
	this->gorgon = nullptr;
	this->sarude = nullptr;   
	this->ram = nullptr;
	this->shieldImpact = SmartAnimationSequenceNode::create();
	this->swordImpact = SmartAnimationSequenceNode::create();
	this->knifeImpact = SmartAnimationSequenceNode::create();
	this->backPeddleSound = WorldSound::create(SoundResources::Platformer_Entities_Generic_Movement_BackPeddleSlow1);
	this->runSound = WorldSound::create(SoundResources::Platformer_Entities_Generic_Movement_RunBrief1);
	this->reboundSoundShield = WorldSound::create(SoundResources::Platformer_Defense_Rebound3);
	this->reboundSoundSword = WorldSound::create(SoundResources::Platformer_Defense_Rebound1);
	this->reboundSoundShieldLite = WorldSound::create(SoundResources::Platformer_Defense_Rebound2);
	this->impactSound = Sound::create(SoundResources::Platformer_Objects_Misc_BowlingStrike1);

	this->addChild(this->shieldImpact);
	this->addChild(this->swordImpact);
	this->addChild(this->knifeImpact);
	this->addChild(this->backPeddleSound);
	this->addChild(this->runSound);
	this->addChild(this->reboundSoundShield);
	this->addChild(this->reboundSoundSword);
	this->addChild(this->reboundSoundShieldLite);
	this->addChild(this->impactSound);
}

FightGorgon::~FightGorgon()
{
}

void FightGorgon::onLoad(QuestState questState)
{
	ObjectEvents::WatchForObject<Sarude>(this, [=](Sarude* sarude)
	{
		this->sarude = sarude;
	}, Sarude::MapKey);

	if (questState == QuestState::Active || questState == QuestState::ActiveThroughSkippable)
	{
		this->runMageAnims();
	}

	ObjectEvents::WatchForObject<Squally>(this, [=](Squally* squally)
	{
		this->squally = squally;
	}, Squally::MapKey);

	ObjectEvents::WatchForObject<GameObject>(this, [=](GameObject* forceField)
	{
		this->forceField = forceField;

		if (questState == QuestState::Complete)
		{
			this->forceField->setVisible(false);
		}
	}, FightGorgon::ForceFieldTag);

	ObjectEvents::WatchForObject<Gorgon>(this, [=](Gorgon* gorgon)
	{
		this->gorgon = gorgon;

		if (this->gorgon->getRuntimeStateOrDefault(StateKeys::IsAlive, Value(true)).asBool())
		{
			if (questState == QuestState::Active)
			{
				this->defer([=]()
				{
					this->killRammedEnemies();
					this->gorgon->attachComponent(LookAtSquallyBehavior::create(this->gorgon));
				});

				this->listenForMapEventOnce(FightGorgon::MarkerTagBack, [=](ValueMap)
				{
					this->flickerOutForceField();
					this->runCinematicSequencePart1();
				});

				this->listenForMapEventOnce(FightGorgon::MarkerTagFront, [=](ValueMap)
				{
					this->flickerOutForceField();
					this->runCinematicSequencePart1Alt();
				});
			}
			else if (questState == QuestState::ActiveThroughSkippable)
			{
				this->runGorgonLoop();
			}

			// Listen for non-combat death. Just a debugging tool.
			this->gorgon->listenForStateWrite(StateKeys::IsAlive, [=](Value value)
			{
				if (!value.asBool())
				{
					this->complete();
				}
			});
		}
		else
		{
			// Gorgon dead! Complete quest.
			this->defer([=]()
			{
				this->complete();
			});
		}

	}, Gorgon::MapKey);

	ObjectEvents::WatchForObject<Ram>(this, [=](Ram* ram)
	{
		this->ram = ram;
		this->ram->setVisible(questState == QuestState::Active);
		this->ram->getAnimations()->playAnimation("Broken", SmartAnimationNode::AnimationPlayMode::Repeat);
	}, Ram::MapKey);
}

void FightGorgon::onActivate(bool isActiveThroughSkippable)
{
}

void FightGorgon::onComplete()
{
	Objectives::SetCurrentObjective(ObjectiveKeys::EFReturnToQueenAgain);

	// Might not be set yet due to potential timing race
	this->forceField->setVisible(false);
}

void FightGorgon::onSkipped()
{
}

void FightGorgon::runCinematicSequencePart1()
{
	this->runAction(Sequence::create(
		DelayTime::create(0.25f),
		CallFunc::create([=]()
		{
			ObjectEvents::WatchForObject<CinematicMarker>(this, [=](CinematicMarker* marker)
			{
				this->squally->setState(StateKeys::CinematicSourceX, Value(GameUtils::getWorldCoords(this->squally).x));
				this->squally->setState(StateKeys::CinematicDestinationX, Value(GameUtils::getWorldCoords(marker).x));
			}, FightGorgon::MarkerTagBack);

			this->squally->listenForStateWriteOnce(StateKeys::CinematicDestinationReached, [=](Value value)
			{
				this->runCinematicSequencePart2();
			});
		}),
		nullptr
	));
}

void FightGorgon::runCinematicSequencePart1Alt()
{
	this->runAction(Sequence::create(
		DelayTime::create(0.25f),
		CallFunc::create([=]()
		{
			ObjectEvents::WatchForObject<CinematicMarker>(this, [=](CinematicMarker* marker)
			{
				this->squally->setState(StateKeys::CinematicSourceX, Value(GameUtils::getWorldCoords(this->squally).x));
				this->squally->setState(StateKeys::CinematicDestinationX, Value(GameUtils::getWorldCoords(marker).x));
			}, FightGorgon::MarkerTagFront);

			this->squally->listenForStateWriteOnce(StateKeys::CinematicDestinationReached, [=](Value value)
			{
				this->runCinematicSequencePart2();
			});
		}),
		nullptr
	));
}

void FightGorgon::runCinematicSequencePart2()
{
	DialogueEvents::TriggerOpenDialogue(DialogueEvents::DialogueOpenArgs(
		Strings::Platformer_Quests_EndianForest_SaveTown_Gorgon_A_MeSmash::create(),
		DialogueEvents::DialogueVisualArgs(
			DialogueBox::DialogueDock::Bottom,
			DialogueBox::DialogueAlignment::Left,
			DialogueEvents::BuildPreviewNode(&this->gorgon, false),
			DialogueEvents::BuildPreviewNode(&this->squally, true)
		),
		[=]()
		{
			this->runCinematicSequencePart3();
		},
		SoundResources::Platformer_Entities_Orc_GruntDeep1,
		true
	));
}

void FightGorgon::runCinematicSequencePart3()
{
	// Player position does not get saved during cinematics. In this case, it would make for better gameplay to do so here.
	PlatformerEvents::TriggerSavePosition();

	PlatformerEvents::TriggerEngageEnemy(PlatformerEvents::EngageEnemyArgs(this->gorgon, true));
}

void FightGorgon::killRammedEnemies()
{
	ObjectEvents::QueryObjects<PlatformerEnemy>([=](PlatformerEnemy* enemy)
	{
		if (enemy != this->gorgon)
		{
			enemy->getComponent<EntityHealthBehavior>([=](EntityHealthBehavior* healthBehavior)
			{
				healthBehavior->kill();
			});
		}
	}, PlatformerEnemy::PlatformerEnemyTag);

	this->impactSound->play();
}

void FightGorgon::flickerOutForceField()
{
	if (this->forceField == nullptr)
	{
		return;
	}

	this->forceField->runAction(Sequence::create(
		DelayTime::create(2.25f),
		FadeTo::create(0.25f, 0),
		FadeTo::create(0.25f, 255),
		FadeTo::create(0.25f, 0),
		FadeTo::create(0.25f, 255),
		FadeTo::create(0.25f, 0),
		nullptr
	));
}

void FightGorgon::runMageAnims()
{
	ObjectEvents::WatchForObject<Igneus>(this, [=](Igneus* igneus)
	{
		this->defer([=]()
		{
			igneus->attachComponent(MageCastBehavior::create(igneus));
		});
	}, Igneus::MapKey);

	ObjectEvents::WatchForObject<Alder>(this, [=](Alder* alder)
	{
		this->defer([=]()
		{
			alder->attachComponent(MageCastBehavior::create(alder));
		});
	}, Alder::MapKey);

	ObjectEvents::WatchForObject<Sarude>(this, [=](Sarude* sarude)
	{
		this->defer([=]()
		{
			sarude->attachComponent(MageCastBehavior::create(sarude));
		});
	}, Sarude::MapKey);

	ObjectEvents::WatchForObject<Aster>(this, [=](Aster* aster)
	{
		this->defer([=]()
		{
			aster->attachComponent(MageCastBehavior::create(aster));
		});
	}, Aster::MapKey);

	ObjectEvents::WatchForObject<Merlin>(this, [=](Merlin* merlin)
	{
		this->defer([=]()
		{
			merlin->attachComponent(MageCastBehavior::create(merlin));
		});
	}, Merlin::MapKey);
}

void FightGorgon::positionImpactFx()
{
	if (this->gorgon == nullptr)
	{
		return;
	}

	float flipMultiplier = this->gorgon->getAnimations()->getFlippedX() ? -1.0f : 1.0f;
	Vec2 entityCenter = this->gorgon->getEntityCenterPoint();

	this->shieldImpact->setPosition(entityCenter + Vec2(256.0f * flipMultiplier, 128.0f));
	this->knifeImpact->setPosition(entityCenter + Vec2(256.0f * flipMultiplier, 144.0f));
	this->swordImpact->setPosition(entityCenter + Vec2(420.0f * flipMultiplier, 256.0f));

	this->reboundSoundShield->setPosition(this->shieldImpact->getPosition());
	this->reboundSoundShieldLite->setPosition(this->knifeImpact->getPosition());
	this->reboundSoundSword->setPosition(this->swordImpact->getPosition());
}

void FightGorgon::runGorgonLoop()
{
	if (this->gorgon == nullptr)
	{
		return;
	}

	this->gorgon->getAnimations()->clearAnimationPriority();

	this->runAction(Sequence::create(
		DelayTime::create(0.5f),
		CallFunc::create([=]()
		{
			this->positionImpactFx();
			this->knifeImpact->playAnimation(FXResources::EnergyBurst_EnergyBurst_0000, 0.05f, true);
			this->reboundSoundSword->play();
		}),
		DelayTime::create(0.5f),
		CallFunc::create([=]()
		{
			this->swordImpact->playAnimation(FXResources::EnergyBurst_EnergyBurst_0000, 0.05f, true);
			this->reboundSoundSword->play();
		}),
		nullptr
	));

	this->gorgon->getAnimations()->playAnimation("AttackRebound", SmartAnimationNode::AnimationPlayMode::Callback, SmartAnimationNode::AnimParams(1.0f, 0.25f), [=]()
	{
		this->gorgon->getAnimations()->clearAnimationPriority();

		this->runAction(Sequence::create(
			DelayTime::create(0.5f),
			CallFunc::create([=]()
			{
				this->positionImpactFx();
				this->shieldImpact->playAnimation(FXResources::EnergyBurst_EnergyBurst_0000, 0.05f, true);
				this->reboundSoundShieldLite->play();
			}),
			nullptr
		));

		this->gorgon->getAnimations()->playAnimation("AttackStrongRebound", SmartAnimationNode::AnimationPlayMode::Callback, SmartAnimationNode::AnimParams(1.0f, 0.25f), [=]()
		{
			this->gorgon->getAnimations()->clearAnimationPriority();
			this->backPeddleSound->play();

			const float BackPeddleTime = 1.75f;
			const float TotalTime = 2.25f;

			this->runAction(Sequence::create(
				DelayTime::create(BackPeddleTime),
				CallFunc::create([=]()
				{
					this->runSound->play();
				}),
				DelayTime::create(TotalTime - BackPeddleTime),
				CallFunc::create([=]()
				{
					this->positionImpactFx();
					this->shieldImpact->playAnimation(FXResources::EnergyBurst_EnergyBurst_0000, 0.05f, true);
					this->reboundSoundShield->play();
				}),
				nullptr
			));

			this->gorgon->getAnimations()->playAnimation("AttackChargeRebound", SmartAnimationNode::AnimationPlayMode::Callback, SmartAnimationNode::AnimParams(1.0f, 0.25f), [=]()
			{
				this->runGorgonLoop();
			});
		});
	});
}
