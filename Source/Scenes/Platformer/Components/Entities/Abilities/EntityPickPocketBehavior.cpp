#include "EntityPickPocketBehavior.h"

#include "cocos/2d/CCSprite.h"

#include "Engine/Animations/SmartAnimationNode.h"
#include "Engine/Events/ObjectEvents.h"
#include "Engine/Input/ClickableNode.h"
#include "Engine/Input/Input.h"
#include "Engine/Inventory/MinMaxPool.h"
#include "Engine/Physics/CollisionObject.h"
#include "Engine/Physics/EnginePhysicsTypes.h"
#include "Engine/Save/SaveManager.h"
#include "Engine/UI/Mouse.h"
#include "Engine/Utils/GameUtils.h"
#include "Entities/Platformer/Helpers/EndianForest/Guano.h"
#include "Entities/Platformer/Squally/Squally.h"
#include "Entities/Platformer/PlatformerEntity.h"
#include "Events/HelperEvents.h"
#include "Objects/Platformer/ItemPools/PocketPools/PocketPoolDeserializer.h"
#include "Objects/Platformer/ItemPools/ErrorPool.h"
#include "Menus/CursorSets.h"
#include "Scenes/Platformer/Components/Entities/EntitySelectionBehavior.h"
#include "Scenes/Platformer/Level/Physics/PlatformerPhysicsTypes.h"
#include "Scenes/Platformer/Save/SaveKeys.h"
#include "Scenes/Platformer/State/StateKeys.h"

#include "Resources/UIResources.h"

using namespace cocos2d;

const std::string EntityPickPocketBehavior::MapKey = "pick-pocket";
const std::string EntityPickPocketBehavior::SavePropertyKeyWasPickPocketed = "WAS_PICKPOCKETED";
const std::string EntityPickPocketBehavior::MapKeyPocketPool = "pocket-pool";

EntityPickPocketBehavior* EntityPickPocketBehavior::create(GameObject* owner)
{
	EntityPickPocketBehavior* instance = new EntityPickPocketBehavior(owner);

	instance->autorelease();

	return instance;
}

EntityPickPocketBehavior::EntityPickPocketBehavior(GameObject* owner) : super(owner)
{
	this->entity = dynamic_cast<PlatformerEntity*>(owner);
	this->pocketPoolDeserializer = PocketPoolDeserializer::create();
	this->pickPocketIcon = Sprite::create(UIResources::Menus_Icons_Chest);

	this->pickPocketIcon->setScale(0.75f);

	if (this->entity == nullptr)
	{
		this->invalidate();
	}

	this->addChild(this->pocketPoolDeserializer);
	this->addChild(this->pickPocketIcon);
}

EntityPickPocketBehavior::~EntityPickPocketBehavior()
{
}

void EntityPickPocketBehavior::initializePositions()
{
	super::initializePositions();

	this->pickPocketIcon->setPosition(Vec2(0.0f, -32.0f));
}

void EntityPickPocketBehavior::onLoad()
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
				return this->canPickPocket();
			});
			selectionBehavior->setEntityClickCallbacks([=]()
			{
				this->attemptPickPocket();
			},
			[=]()
			{
				if (this->currentHelperName == Guano::MapKey && this->canPickPocket())
				{
					CursorSets::SetActiveCursorSet(CursorSets::PickPocket);
				}
			},
			[=]()
			{
				CursorSets::SetActiveCursorSet(CursorSets::Default);
			});
		});

		ObjectEvents::TriggerBindObjectToUI(RelocateObjectArgs(this->pickPocketIcon));
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

	ValueMap valueMap = ValueMap();

	valueMap[GameObject::MapKeyType] = PocketPoolDeserializer::MapKeyTypePocketPool;
	valueMap[GameObject::MapKeyName] = this->entity->getPropertyOrDefault(EntityPickPocketBehavior::MapKeyPocketPool, Value("")).asString();

	ObjectDeserializer::ObjectDeserializationRequestArgs deserializeArgs = ObjectDeserializer::ObjectDeserializationRequestArgs(valueMap, [=](ObjectDeserializer::ObjectDeserializationArgs args)
	{
		MinMaxPool* pocketPool = static_cast<MinMaxPool*>(args.gameObject);

		this->pocketPool = pocketPool;

		this->addChild(this->pocketPool);
	});
	
	this->pocketPoolDeserializer->deserialize(&deserializeArgs);

	if (this->pocketPool == nullptr)
	{
		this->pocketPool = ErrorPool::create();

		this->addChild(this->pocketPool);
	}
}

void EntityPickPocketBehavior::onDisable()
{
	super::onDisable();

	this->updateIconVisibility();
}

void EntityPickPocketBehavior::attemptPickPocket()
{
	if (this->canPickPocket())
	{
		HelperEvents::TriggerRequestPickPocket(HelperEvents::RequestPickPocketArgs(
			this->entity,
			this->pocketPool,
			[=]()
			{
				this->onPickPocketed();
			},
			EntityPickPocketBehavior::SavePropertyKeyWasPickPocketed
		));
	}
}

bool EntityPickPocketBehavior::canPickPocket()
{
	return this->entity != nullptr 
		&& !this->isInvalidated()
		&& this->currentHelperName == Guano::MapKey
		&& this->entity->getRuntimeStateOrDefaultBool(StateKeys::IsAlive, true)
		&& !this->wasPickPocketed();
}

bool EntityPickPocketBehavior::wasPickPocketed()
{
	return this->entity->loadObjectStateOrDefault(EntityPickPocketBehavior::SavePropertyKeyWasPickPocketed, Value(false)).asBool();
}

void EntityPickPocketBehavior::onPickPocketed()
{
	this->updateIconVisibility();
	
	this->entity->getComponent<EntitySelectionBehavior>([=](EntitySelectionBehavior* selection)
	{
		selection->clearEntityClickCallbacks();
		
		this->refreshCursorState();
	});
}

void EntityPickPocketBehavior::updateIconVisibility()
{
	this->pickPocketIcon->setVisible(this->canPickPocket());
}

void EntityPickPocketBehavior::refreshCursorState()
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
				selection->getOwner()->getComponent<EntityPickPocketBehavior>([](EntityPickPocketBehavior* pickPocketBehavior)
				{
					if (pickPocketBehavior->canPickPocket())
					{
						CursorSets::SetActiveCursorSet(CursorSets::PickPocket);
					}
				});
			}
		}, false, false)
	);

	InputEvents::TriggerMouseRequestRefresh();
}
