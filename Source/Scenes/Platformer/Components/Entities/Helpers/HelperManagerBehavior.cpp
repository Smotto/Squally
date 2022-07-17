#include "HelperManagerBehavior.h"

#include "cocos/2d/CCActionEase.h"
#include "cocos/2d/CCActionInstant.h"
#include "cocos/2d/CCActionInterval.h"
#include "cocos/base/CCEventCustom.h"
#include "cocos/base/CCEventListenerCustom.h"
#include "cocos/base/CCValue.h"

#include "Deserializers/Platformer/PlatformerEntityDeserializer.h"
#include "Engine/Events/ObjectEvents.h"
#include "Engine/Input/Input.h"
#include "Engine/Save/SaveManager.h"
#include "Engine/Utils/GameUtils.h"
#include "Entities/Platformer/PlatformerEntity.h"
#include "Entities/Platformer/PlatformerEntities.h"
#include "Events/HelperEvents.h"
#include "Events/NotificationEvents.h"
#include "Scenes/Platformer/Components/Entities/Helpers/Gecky/GeckyBehaviorGroup.h"
#include "Scenes/Platformer/Components/Entities/Helpers/Guano/GuanoBehaviorGroup.h"
#include "Scenes/Platformer/Components/Entities/Helpers/GuanoPetrified/GuanoPetrifiedBehaviorGroup.h"
#include "Scenes/Platformer/Components/Entities/Helpers/Scrappy/ScrappyBehaviorGroup.h"
#include "Scenes/Platformer/Save/SaveKeys.h"
#include "Scenes/Platformer/State/StateKeys.h"

#include "Resources/EntityResources.h"
#include "Resources/SoundResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

const std::string HelperManagerBehavior::MapKey = "entity-helper-manager";

HelperManagerBehavior* HelperManagerBehavior::create(GameObject* owner)
{
	HelperManagerBehavior* instance = new HelperManagerBehavior(owner);

	instance->autorelease();

	return instance;
}

HelperManagerBehavior::HelperManagerBehavior(GameObject* owner) : super(owner)
{
	this->entity = dynamic_cast<PlatformerEntity*>(owner);

	if (this->entity == nullptr)
	{
		this->invalidate();
	}
	
	this->platformerEntityDeserializer = PlatformerEntityDeserializer::create();

	this->addChild(this->platformerEntityDeserializer);
}

HelperManagerBehavior::~HelperManagerBehavior()
{
}

void HelperManagerBehavior::onLoad()
{
	this->buildComponentMap();

	this->defer([=]()
	{
		this->spawnHelper(SaveManager::GetProfileDataOrDefault(SaveKeys::SaveKeyHelperName, Value("")).asString(), false);

		this->entity->listenForStateWrite(StateKeys::CurrentHelper, [=](Value value)
		{
			this->spawnHelperFromState();
		});
	});
}

PlatformerEntity* HelperManagerBehavior::getHelperRef()
{
	return this->helperRef;
}

void HelperManagerBehavior::despawnCurrentHelper()
{
	if (this->helperRef != nullptr)
	{
		this->helperRef->despawn();
		this->helperRef = nullptr;
	}
}

void HelperManagerBehavior::spawnHelperFromState()
{
	std::string helperName = this->entity->getRuntimeStateOrDefault(StateKeys::CurrentHelper, Value("")).asString();
	bool notify = this->entity->getRuntimeStateOrDefault(StateKeys::NotifyNewHelper, Value(false)).asBool();
	Vec2 spawnCoords = Vec2::ZERO;
	bool warpToSpawn = false;

	if (this->entity->hasRuntimeState(StateKeys::NewHelperSpawnX) || this->entity->hasRuntimeState(StateKeys::NewHelperSpawnY))
	{
		spawnCoords = Vec2(
			this->entity->getRuntimeStateOrDefaultFloat(StateKeys::NewHelperSpawnX, 0.0f),
			this->entity->getRuntimeStateOrDefaultFloat(StateKeys::NewHelperSpawnY, 0.0f)
		);

		warpToSpawn = true;
	}
	
	this->spawnHelper(helperName, notify);

	if (warpToSpawn)
	{
		GameUtils::setWorldCoords(this->helperRef, spawnCoords);
	}
}

void HelperManagerBehavior::spawnHelper(std::string helperName, bool notify)
{
	this->despawnCurrentHelper();

	ValueMap properties = ValueMap();

	SaveManager::SaveProfileData(SaveKeys::SaveKeyHelperName, Value(helperName));

	std::string helperBehavior = this->getHelperComponent(helperName);
	
	properties[GameObject::MapKeyType] = PlatformerEntityDeserializer::MapKeyTypeEntity;
	properties[GameObject::MapKeyName] = Value(helperName);
	properties[GameObject::MapKeyTags] = Value(Squally::TeamTag);
	properties[GameObject::MapKeyComponent] = Value(helperBehavior);
	properties[GameObject::MapKeyFlipX] = Value(true);

	ObjectDeserializer::ObjectDeserializationRequestArgs args = ObjectDeserializer::ObjectDeserializationRequestArgs(
		properties,
		[=] (ObjectDeserializer::ObjectDeserializationArgs deserializeArgs)
		{
			PlatformerEntity* helper = dynamic_cast<PlatformerEntity*>(deserializeArgs.gameObject);

			this->helperRef = helper;

			if (helper != nullptr)
			{
				if (notify)
				{
					NotificationEvents::TriggerNotification(NotificationEvents::NotificationArgs(
						nullptr,
						Strings::Platformer_Notifications_Party_HelperJoinedParty::create()->setStringReplacementVariables(helper->getEntityName()),
						helper->getEmblemResource(),
						SoundResources::Notifications_NotificationGood2
					));
				}

				ObjectEvents::TriggerObjectSpawn(RequestObjectSpawnArgs(
					this->entity,
					deserializeArgs.gameObject,
					SpawnMethod::Below,
					PositionMode::SetToOwner,
					[&]()
					{
					},
					[&]()
					{
					}
				));
			}
		}
	);

	this->platformerEntityDeserializer->deserialize(&args);

	this->entity->clearState(StateKeys::NotifyNewHelper);
	this->entity->clearState(StateKeys::NewHelperSpawnX);
	this->entity->clearState(StateKeys::NewHelperSpawnY);
}

void HelperManagerBehavior::onDisable()
{
	super::onDisable();
}

const std::string& HelperManagerBehavior::getHelperComponent(const std::string& helperName)
{
	static const std::string Empty = "";

	if (this->componentMap.find(helperName) != this->componentMap.end())
	{
		return this->componentMap[helperName];
	}

	return Empty;
}

void HelperManagerBehavior::buildComponentMap()
{
	this->componentMap[Gecky::MapKey] = GeckyBehaviorGroup::MapKey;
	this->componentMap[Guano::MapKey] = GuanoBehaviorGroup::MapKey;
	this->componentMap[GuanoPetrified::MapKey] = GuanoPetrifiedBehaviorGroup::MapKey;
	this->componentMap[Snowman::MapKey] = GuanoBehaviorGroup::MapKey;
}
