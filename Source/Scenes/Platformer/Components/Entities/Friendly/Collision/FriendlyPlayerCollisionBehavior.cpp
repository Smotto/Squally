#include "FriendlyPlayerCollisionBehavior.h"

#include "Engine/Animations/SmartAnimationNode.h"
#include "Engine/Physics/CollisionObject.h"
#include "Engine/Physics/EnginePhysicsTypes.h"
#include "Entities/Platformer/PlatformerEntity.h"
#include "Scenes/Platformer/Components/Entities/Collision/EntityCollisionBehaviorBase.h"
#include "Scenes/Platformer/Level/Physics/PlatformerPhysicsTypes.h"

#include "Resources/EntityResources.h"

using namespace cocos2d;

const std::string FriendlyPlayerCollisionBehavior::MapKey = "friendly-collisions";

FriendlyPlayerCollisionBehavior* FriendlyPlayerCollisionBehavior::create(GameObject* owner)
{
	FriendlyPlayerCollisionBehavior* instance = new FriendlyPlayerCollisionBehavior(owner);

	instance->autorelease();

	return instance;
}

FriendlyPlayerCollisionBehavior::FriendlyPlayerCollisionBehavior(GameObject* owner) : super(owner)
{
	this->entity = dynamic_cast<PlatformerEntity*>(owner);

	if (this->entity == nullptr)
	{
		this->invalidate();
	}
}

FriendlyPlayerCollisionBehavior::~FriendlyPlayerCollisionBehavior()
{
}

void FriendlyPlayerCollisionBehavior::onLoad()
{
	// This is fun, but results in a pretty bad game experience due to the tendency to clip friendly entities out of bounds
	/*
	this->entity->watchForComponent<EntityCollisionBehaviorBase>([=](EntityCollisionBehaviorBase* collisionBehavior)
	{
		if (collisionBehavior->movementCollision != nullptr)
		{
			collisionBehavior->movementCollision->whenCollidesWith({ (int)PlatformerCollisionType::PlayerWeapon }, [=](CollisionData collisionData)
			{	
				return CollisionResult::CollideWithPhysics;
			});
		}
	});
	*/
}

void FriendlyPlayerCollisionBehavior::onDisable()
{
	super::onDisable();
}
