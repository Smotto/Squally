#include "EntitySoulBehavior.h"

#include "cocos/2d/CCSprite.h"

#include "Engine/Animations/SmartAnimationNode.h"
#include "Engine/Animations/SmartAnimationSequenceNode.h"
#include "Engine/Events/ObjectEvents.h"
#include "Engine/Input/ClickableNode.h"
#include "Engine/Input/Input.h"
#include "Engine/Inventory/MinMaxPool.h"
#include "Engine/Particles/SmartParticles.h"
#include "Engine/Physics/CollisionObject.h"
#include "Engine/Physics/EnginePhysicsTypes.h"
#include "Engine/Save/SaveManager.h"
#include "Engine/Sound/WorldSound.h"
#include "Engine/UI/Mouse.h"
#include "Engine/UI/UIBoundObject.h"
#include "Engine/Utils/GameUtils.h"
#include "Entities/Platformer/Helpers/CastleValgrind/Grim.h"
#include "Entities/Platformer/Squally/Squally.h"
#include "Entities/Platformer/PlatformerEntity.h"
#include "Events/HelperEvents.h"
#include "Objects/Platformer/ItemPools/PocketPools/PocketPoolDeserializer.h"
#include "Objects/Platformer/ItemPools/ErrorPool.h"
#include "Menus/CursorSets.h"
#include "Scenes/Platformer/Components/Entities/EntitySelectionBehavior.h"
#include "Scenes/Platformer/Components/Entities/Haunted/EntityHauntedBehavior.h"
#include "Scenes/Platformer/Level/Physics/PlatformerPhysicsTypes.h"
#include "Scenes/Platformer/Save/SaveKeys.h"
#include "Scenes/Platformer/State/StateKeys.h"

#include "Resources/ParticleResources.h"
#include "Resources/SoundResources.h"
#include "Resources/FXResources.h"
#include "Resources/UIResources.h"

using namespace cocos2d;

const std::string EntitySoulBehavior::MapKey = "soul";
const std::string EntitySoulBehavior::SavePropertyKeyWasSoulHarvested = "WAS_SOUL_HARVESTED";

EntitySoulBehavior* EntitySoulBehavior::create(GameObject* owner)
{
	EntitySoulBehavior* instance = new EntitySoulBehavior(owner);

	instance->autorelease();

	return instance;
}

EntitySoulBehavior::EntitySoulBehavior(GameObject* owner) : super(owner)
{
	this->soulHarvestContainer = Node::create();
	this->soulHarvestGlow = Sprite::create(UIResources::HUD_EmblemGlow);
	this->soulHarvestIcon = Sprite::create(UIResources::Menus_Icons_AngelFigurine);
	this->soulFx = SmartParticles::create(ParticleResources::Platformer_Combat_Abilities_Speed);
	this->soulHarvestFx = SmartAnimationSequenceNode::create();
	this->healFxSqually = SmartAnimationSequenceNode::create();
	this->soulHarvestSfx = WorldSound::create(SoundResources::Platformer_Spells_Curse1);
	this->entity = dynamic_cast<PlatformerEntity*>(owner);

	this->soulFx->setStartColor(Color4F::PURPLE);
	this->soulFx->setEndColor(Color4F(0.5f, 0.0f, 0.5f, 0.0f));
	this->soulHarvestGlow->setScale(0.75f);
	this->soulHarvestIcon->setScale(0.75f);
	this->soulHarvestFx->setAnimationAnchor(Vec2(0.5f, 0.0f));
	this->healFxSqually->setAnimationAnchor(Vec2(0.5f, 0.0f));

	if (this->entity == nullptr)
	{
		this->invalidate();
	}

	this->soulHarvestContainer->addChild(this->soulHarvestGlow);
	this->soulHarvestContainer->addChild(this->soulHarvestIcon);
	this->addChild(this->soulHarvestSfx);
	this->addChild(this->soulFx);
	this->addChild(this->soulHarvestFx);
	this->addChild(this->healFxSqually);
	this->addChild(this->soulHarvestContainer);
}

EntitySoulBehavior::~EntitySoulBehavior()
{
}

void EntitySoulBehavior::initializePositions()
{
	super::initializePositions();

	// this->soulFx->setPositionY(this->entity->getEntityBottomPointRelative().y);
	this->soulHarvestContainer->setPosition(Vec2(0.0f, -32.0f));
}

void EntitySoulBehavior::onLoad()
{

	this->defer([=]()
	{
		this->updateIconVisibility();
		
		if (this->entity->getComponent<EntitySelectionBehavior>() == nullptr)
		{
			this->entity->attachComponent(EntitySelectionBehavior::create(this->entity));
		}

		this->entity->listenForStateWrite(StateKeys::IsAlive, [=](Value value)
		{
			this->updateIconVisibility();
		});

		this->entity->watchForComponent<EntitySelectionBehavior>([=](EntitySelectionBehavior* selectionBehavior)
		{
			selectionBehavior->setClickModifier(InputEvents::KeyCode::KEY_SHIFT);
			selectionBehavior->setClickableCallback([=]()
			{
				return this->canSoulHarvest(true);
			});
			selectionBehavior->setEntityClickCallbacks([=]()
			{
				this->attemptSoulHarvest();
			},
			[=]()
			{
				if (this->currentHelperName == Grim::MapKey && this->canSoulHarvest(true))
				{
					CursorSets::SetActiveCursorSet(CursorSets::SoulHarvest);
				}
			},
			[=]()
			{
				CursorSets::SetActiveCursorSet(CursorSets::Default);
			});
		});

		ObjectEvents::TriggerBindObjectToUI(RelocateObjectArgs(this->soulHarvestContainer));
	});
	
	ObjectEvents::WatchForObject<Squally>(this, [=](Squally* squally)
	{
		this->squally = squally;
		this->currentHelperName = SaveManager::GetProfileDataOrDefault(SaveKeys::SaveKeyHelperName, Value("")).asString();
			this->updateIconVisibility();

		this->squally->listenForStateWrite(StateKeys::CurrentHelper, [=](Value value)
		{
			this->currentHelperName = value.asString();
			this->updateIconVisibility();
		});
	}, Squally::MapKey);
}

void EntitySoulBehavior::onDisable()
{
	super::onDisable();
}

void EntitySoulBehavior::attemptSoulHarvest()
{
	if (this->canSoulHarvest(true))
	{
		HelperEvents::TriggerRequestSoulHarvest(HelperEvents::RequestSoulHarvestArgs(
			this->entity,
			[=]()
			{
				this->onSoulHarvested();
			},
			EntitySoulBehavior::SavePropertyKeyWasSoulHarvested
		));
	}
}

bool EntitySoulBehavior::canSoulHarvest(bool checkZ)
{
	bool isHelperZAligned = true;

	if (checkZ)
	{
		ObjectEvents::QueryObjects<PlatformerHelper>([&](PlatformerHelper* helper)
		{
			if (std::abs(GameUtils::getDepth(helper) - GameUtils::getDepth(entity)) > 16.0f)
			{
				isHelperZAligned = false;
			}
		}, Squally::TeamTag);
	}
	
	return this->entity != nullptr
		&& !this->isInvalidated()
		&& this->currentHelperName == Grim::MapKey
		&& isHelperZAligned
		&& !this->entity->getRuntimeStateOrDefaultBool(StateKeys::IsAlive, true)
		&& !this->wasSoulHarvested();
}

bool EntitySoulBehavior::wasSoulHarvested()
{
	return this->entity->loadObjectStateOrDefault(EntitySoulBehavior::SavePropertyKeyWasSoulHarvested, Value(false)).asBool();
}

void EntitySoulBehavior::onSoulHarvested()
{
	this->updateIconVisibility();

	this->soulHarvestFx->playAnimation(FXResources::Curse_Curse_0000, 0.05f, true);
	this->soulHarvestSfx->play();

	ObjectEvents::QueryObject<Squally>([=](Squally* squally)
	{
		// Jank, but should work since Squally should be on the same layer as the target entity
		GameUtils::setWorldCoords3D(this->healFxSqually, GameUtils::getWorldCoords3D(squally));
		this->healFxSqually->playAnimation(FXResources::Restore_Restore_0000, 0.05f, true);
	}, Squally::MapKey);

	
	this->entity->getComponent<EntitySelectionBehavior>([=](EntitySelectionBehavior* selection)
	{
		selection->clearEntityClickCallbacks();
		
		this->refreshCursorState();
	});

	// Lazy way to update the quest. Correct way would be to fire an event, but I don't want to do that.
	this->entity->getComponent<EntityHauntedBehavior>([=](EntityHauntedBehavior* entityHauntedBehavior)
	{
		entityHauntedBehavior->onSoulHarvested();
	});
}

void EntitySoulBehavior::updateIconVisibility()
{
	if (this->canSoulHarvest(false))
	{
		this->soulFx->start();
	}
	else
	{
		this->soulFx->stop();
	}

	this->soulHarvestContainer->setVisible(this->canSoulHarvest(false));
}

void EntitySoulBehavior::refreshCursorState()
{
	CursorSets::SetActiveCursorSet(CursorSets::Default);

	// Fire a hit test to see if any other entities are still selected. Restore the pickpocket cursor set if so.
	InputEvents::TriggerMouseHitTest(InputEvents::MouseHitTestArgs(
		Input::GetMouseEvent().mouseCoords,
		[=](Node* node)
		{
			EntitySelectionBehavior* selection = GameUtils::GetFirstParentOfType<EntitySelectionBehavior>(node);

			if (selection != nullptr)
			{
				selection->getOwner()->getComponent<EntitySoulBehavior>([](EntitySoulBehavior* pickPocketBehavior)
				{
					if (pickPocketBehavior->canSoulHarvest(true))
					{
						CursorSets::SetActiveCursorSet(CursorSets::SoulHarvest);
					}
				});
			}
		}, false, false)
	);

	InputEvents::TriggerMouseRequestRefresh();
}
