#include "AncientAnvil.h"

#include "cocos/2d/CCActionInstant.h"
#include "cocos/2d/CCActionInterval.h"
#include "cocos/2d/CCActionEase.h"
#include "cocos/2d/CCSprite.h"
#include "cocos/base/CCValue.h"

#include "Engine/Inventory/Item.h"
#include "Engine/Inventory/MinMaxPool.h"
#include "Engine/Utils/GameUtils.h"
#include "Events/PlatformerEvents.h"
#include "Objects/Platformer/ItemPools/RecipePools/RecipePoolDeserializer.h"
#include "Scenes/Platformer/Inventory/Items/Recipes/Recipe.h"
#include "Scenes/Platformer/State/StateKeys.h"

#include "Resources/FXResources.h"
#include "Resources/ObjectResources.h"
#include "Resources/SoundResources.h"
#include "Resources/UIResources.h"

using namespace cocos2d;

const std::string AncientAnvil::MapKey = "ancient-anvil";

AncientAnvil* AncientAnvil::create(ValueMap& properties)
{
	AncientAnvil* instance = new AncientAnvil(properties);

	instance->autorelease();

	return instance;
}

AncientAnvil::AncientAnvil(ValueMap& properties) : super(properties, InteractObject::InteractType::Input, CSize(256.0f, 128.0f))
{
	this->anvil = Sprite::create(ObjectResources::Interactive_Anvil_AncientAnvil);
	this->shine = Sprite::create(UIResources::HUD_EmblemGlow);
	this->hammer = Sprite::create(ObjectResources::Interactive_Anvil_AnvilHammer);
	this->floatContainer = Node::create();
	this->recipePoolName = GameUtils::getKeyOrDefault(this->properties, RecipePoolDeserializer::MapKeyTypeRecipePool, Value("")).asString();
	this->recipePoolDeserializer = RecipePoolDeserializer::create();
	
	this->floatContainer->addChild(this->shine);
	this->floatContainer->addChild(this->hammer);
	this->addChild(this->anvil);
	this->addChild(this->floatContainer);
	this->addChild(this->recipePoolDeserializer);
}

AncientAnvil::~AncientAnvil()
{
}

void AncientAnvil::onEnter()
{
	super::onEnter();

	this->floatContainer->runAction(RepeatForever::create(Sequence::create(
		EaseSineInOut::create(MoveTo::create(3.0f, Vec2(0.0f, 96.0f))),
		EaseSineInOut::create(MoveTo::create(3.0f, Vec2(0.0f, 160.0f))),
		nullptr
	)));

	this->loadRecipePool();
}

void AncientAnvil::initializePositions()
{
	super::initializePositions();

	this->hammer->setPosition(Vec2(24.0f, 0.0f));
	this->shine->setPosition(Vec2(24.0f, 0.0f));
	this->floatContainer->setPosition(Vec2(0.0f, 160.0f));
	this->anvil->setPosition(Vec2(0.0f, 0.0f));
}

void AncientAnvil::initializeListeners()
{
	super::initializeListeners();
}

void AncientAnvil::onInteract(PlatformerEntity* interactingEntity)
{
	super::onInteract(interactingEntity);

	PlatformerEvents::TriggerOpenSmithing(PlatformerEvents::CraftingOpenArgs(this->recipes));
}

void AncientAnvil::activate()
{
	this->floatContainer->setVisible(true);

	this->enable();
}

void AncientAnvil::deactivate()
{
	this->floatContainer->setVisible(false);

	this->disable();
}

void AncientAnvil::loadRecipePool()
{
	ValueMap valueMap = ValueMap();

	valueMap[GameObject::MapKeyType] = RecipePoolDeserializer::MapKeyTypeRecipePool;
	valueMap[GameObject::MapKeyName] = this->recipePoolName;

	ObjectDeserializer::ObjectDeserializationRequestArgs deserializeArgs = ObjectDeserializer::ObjectDeserializationRequestArgs(valueMap, [&](ObjectDeserializer::ObjectDeserializationArgs args)
	{
		this->recipePool = static_cast<MinMaxPool*>(args.gameObject);

		for (auto next : this->recipePool->getItems({ }))
		{
			this->recipes.push_back(next);

			this->addChild(next);
		}

		this->addChild(this->recipePool);
	});

	this->recipePoolDeserializer->deserialize(&deserializeArgs);
}
