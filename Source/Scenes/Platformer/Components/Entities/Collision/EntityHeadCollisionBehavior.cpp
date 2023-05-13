#include "EntityHeadCollisionBehavior.h"

#include "Engine/Animations/SmartAnimationNode.h"
#include "Engine/Physics/CollisionObject.h"
#include "Engine/Physics/EnginePhysicsTypes.h"
#include "Entities/Platformer/PlatformerEntity.h"
#include "Entities/Platformer/Squally/Squally.h"
#include "Scenes/Platformer/Level/Physics/PlatformerPhysicsTypes.h"
#include "Scenes/Platformer/State/StateKeys.h"

#include "Resources/EntityResources.h"

using namespace cocos2d;

const std::string EntityHeadCollisionBehavior::MapKey = "entity-head-collisions";
const float EntityHeadCollisionBehavior::HeadCollisionPadding = 16.0f;
const float EntityHeadCollisionBehavior::HeadCollisionSize = 48.0f;

EntityHeadCollisionBehavior* EntityHeadCollisionBehavior::create(GameObject* owner)
{
	EntityHeadCollisionBehavior* instance = new EntityHeadCollisionBehavior(owner);

	instance->autorelease();

	return instance;
}

EntityHeadCollisionBehavior::EntityHeadCollisionBehavior(GameObject* owner) : super(owner)
{
	this->entity = dynamic_cast<PlatformerEntity*>(owner);

	if (this->entity == nullptr)
	{
		this->invalidate();
	}

	this->toggleQueryable(false);
}

EntityHeadCollisionBehavior::~EntityHeadCollisionBehavior()
{
}

void EntityHeadCollisionBehavior::onLoad()
{
	this->defer([=]()
	{
		// this->buildHeadCollisionDetector();
		this->toggleQueryable(true);
	});
}

void EntityHeadCollisionBehavior::onDisable()
{
	super::onDisable();
	
	if (this->headCollision != nullptr)
	{
		this->headCollision->setPhysicsFlagEnabled(false);
	}
}

bool EntityHeadCollisionBehavior::hasHeadCollisionWith(CollisionObject* collisonObject)
{
	if (this->headCollision == nullptr)
	{
		return false;
	}

	Node* currentCollisionGroup = collisonObject->getParent();

	for (CollisionObject* next : this->headCollision->getCurrentCollisions())
	{
		if (next->getParent() != currentCollisionGroup)
		{
			continue;
		}

		if (next->hasCollisionType(CollisionType(PlatformerCollisionType::PassThrough))
			|| next->hasCollisionType(CollisionType(PlatformerCollisionType::SolidRoof)))
		{
			return true;
		}
	}

	return false;
}

void EntityHeadCollisionBehavior::buildHeadCollisionDetector()
{
	if (this->headCollision != nullptr)
	{
		return;
	}
	
	this->headCollision = CollisionObject::create(
		CollisionObject::createCapsulePolygon(
			CSize(std::max((this->entity->getEntitySize()).width + EntityHeadCollisionBehavior::HeadCollisionPadding * 2.0f, 8.0f), EntityHeadCollisionBehavior::HeadCollisionSize)
		),
		(int)PlatformerCollisionType::HeadDetector,
		CollisionObject::Properties(false, false),
		Color4F::YELLOW
	);

	Vec2 entityCenter = this->entity->getEntityCenterPoint();
	Vec2 offset = entityCenter + Vec2(0.0f, this->entity->getEntitySize().height / 2.0f);

	this->headCollision->setPosition(offset);
	
	this->addChild(this->headCollision);

	this->headCollision->whenCollidesWith({ (int)PlatformerCollisionType::PassThrough, (int)PlatformerCollisionType::SolidRoof }, [=](CollisionData collisionData)
	{
		return CollisionResult::DoNothing;
	});
}
