#include "RopedMovementBehavior.h"

#include "cocos/2d/CCSprite.h"
#include "cocos/base/CCEventCustom.h"
#include "cocos/base/CCEventListenerCustom.h"

#include "Engine/Animations/SmartAnimationNode.h"
#include "Engine/Events/ObjectEvents.h"
#include "Entities/Platformer/PlatformerEntity.h"
#include "Entities/Platformer/Helpers/EndianForest/Scrappy.h"
#include "Scenes/Platformer/Components/Entities/Helpers/HelperFollowMovementBehavior.h"
#include "Scenes/Platformer/Components/Entities/Helpers/Scrappy/ScrappyRopeCarryBehavior.h"

#include "Resources/ObjectResources.h"

using namespace cocos2d;

const std::string RopedMovementBehavior::MapKey = "roped-movement";

RopedMovementBehavior* RopedMovementBehavior::create(GameObject* owner)
{
	RopedMovementBehavior* instance = new RopedMovementBehavior(owner);

	instance->autorelease();

	return instance;
}

RopedMovementBehavior::RopedMovementBehavior(GameObject* owner) : super(owner)
{
	this->entity = dynamic_cast<PlatformerEntity*>(owner);
	this->waistRope = Sprite::create(ObjectResources::Cinematic_ScrappyRope_GuanoRope);

	this->waistRope->setVisible(false);

	if (this->entity == nullptr)
	{
		this->invalidate();
	}

	this->addChild(this->waistRope);
}

RopedMovementBehavior::~RopedMovementBehavior()
{
}

void RopedMovementBehavior::onLoad()
{
	ObjectEvents::WatchForObject<Scrappy>(this, [=](Scrappy* scrappy)
	{
		this->scrappy = scrappy;
	
		this->scrappy->watchForComponent<ScrappyRopeCarryBehavior>([=](ScrappyRopeCarryBehavior* ropeCarryBehavior)
		{
			ropeCarryBehavior->setCarriedObject(this->entity);
		});
	}, Scrappy::MapKey);
	
	this->entity->watchForComponent<HelperFollowMovementBehavior>([=](HelperFollowMovementBehavior* followBehavior)
	{
		followBehavior->disableFollow();
	});

	this->waistRope->setVisible(true);
	this->waistRope->setPosition(Vec2(0.0f, 32.0f));

	this->scheduleUpdate();
}

void RopedMovementBehavior::onDisable()
{
	super::onDisable();
}

void RopedMovementBehavior::update(float dt)
{
	super::update(dt);

	if (!this->isDetached && this->entity != nullptr && this->scrappy != nullptr)
	{
		this->entity->getAnimations()->setFlippedX(this->scrappy->isFlippedX());
	}
}

void RopedMovementBehavior::detach()
{
	if (this->scrappy != nullptr)
	{
		this->scrappy->watchForComponent<ScrappyRopeCarryBehavior>([=](ScrappyRopeCarryBehavior* ropeCarryBehavior)
		{
			ropeCarryBehavior->setCarriedObject(nullptr);
		});
	}

	this->waistRope->setVisible(false);
	this->isDetached = true;
}
