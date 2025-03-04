#include "EntitySelectionBehavior.h"

#include "cocos/base/CCInputEvents.h"

#include "Engine/Animations/SmartAnimationNode.h"
#include "Engine/Input/ClickableNode.h"
#include "Engine/Utils/GameUtils.h"
#include "Entities/Platformer/PlatformerEnemy.h"
#include "Entities/Platformer/PlatformerEntity.h"
#include "Entities/Platformer/PlatformerFriendly.h"
#include "Events/CombatEvents.h"

#include "Resources/UIResources.h"

using namespace cocos2d;

const std::string EntitySelectionBehavior::MapKey = "entity-selection";

EntitySelectionBehavior* EntitySelectionBehavior::create(GameObject* owner)
{
	EntitySelectionBehavior* instance = new EntitySelectionBehavior(owner);

	instance->autorelease();

	return instance;
}

EntitySelectionBehavior::EntitySelectionBehavior(GameObject* owner) : super(owner)
{
	this->entity = dynamic_cast<PlatformerEntity*>(owner);

	if (this->entity == nullptr)
	{
		this->invalidate();
	}

	this->clickHitbox = ClickableNode::create(UIResources::EmptyImage, UIResources::EmptyImage);
	
	this->clickHitbox->setContentSize(this->entity->getEntitySize());
	this->clickHitbox->setAnchorPoint(Vec2(0.5f, 0.0f));
	this->clickHitbox->disableInteraction();
	this->clickHitbox->setPosition(Vec2(this->entity->getEntitySize() / 2.0f));

	this->addChild(this->clickHitbox);
}

EntitySelectionBehavior::~EntitySelectionBehavior()
{
}

void EntitySelectionBehavior::update(float dt)
{
	super::update(dt);

	Vec3 coords = GameUtils::getWorldCoords3D(this->entity, false);

	if (this->savedWorldCoords != coords)
	{
		this->savedWorldCoords = coords;

		InputEvents::TriggerMouseRequestRefresh();
	}

	if (this->canClickCallback != nullptr && this->clickHitbox != nullptr)
	{
		if (canClickCallback())
		{
			this->clickHitbox->enableInteraction();
		}
		else
		{
			this->clickHitbox->disableInteraction();
		}
	}
}

void EntitySelectionBehavior::onLoad()
{
	this->scheduleUpdate();
}

void EntitySelectionBehavior::onDisable()
{
	super::onDisable();
}

void EntitySelectionBehavior::setEntityClickCallbacks(std::function<void()> onClick, std::function<void()> onMouseOver, std::function<void()> onMouseOut)
{
	this->clickHitbox->enableInteraction();
	this->clickHitbox->setMouseClickCallback([=](InputEvents::MouseEventArgs*)
	{
		if (onClick != nullptr)
		{
			onClick();
		}
	});
	this->clickHitbox->setMouseOverCallback([=](InputEvents::MouseEventArgs*)
	{
		if (onMouseOver != nullptr)
		{
			onMouseOver();
		}
	});
	this->clickHitbox->setMouseOutCallback([=](InputEvents::MouseEventArgs*)
	{
		if (onMouseOut != nullptr)
		{
			onMouseOut();
		}
	});
}

void EntitySelectionBehavior::setClickModifier(InputEvents::KeyCode modifier)
{
	this->clickHitbox->setClickModifier(modifier);
}

void EntitySelectionBehavior::setClickableCallback(std::function<bool()> canClickCallback)
{
	this->canClickCallback = canClickCallback;
}

void EntitySelectionBehavior::clearEntityClickCallbacks()
{
	this->clickHitbox->disableInteraction();
	this->clickHitbox->setMouseClickCallback(nullptr);
	this->clickHitbox->setMouseOverCallback(nullptr);
	this->clickHitbox->setMouseOutCallback(nullptr);
}

ClickableNode* EntitySelectionBehavior::getHitbox()
{
	return this->clickHitbox;
}

PlatformerEntity* EntitySelectionBehavior::getEntity()
{
	return this->entity;
}
