#include "Water.h"

#include "cocos/base/CCValue.h"

#include "Engine/Physics/CollisionObject.h"
#include "Engine/Physics/Liquid/LiquidNode.h"
#include "Engine/Utils/GameUtils.h"
#include "Engine/Utils/MathUtils.h"
#include "Scenes/Platformer/Level/Physics/PlatformerPhysicsTypes.h"

#include "Resources/UIResources.h"

using namespace cocos2d;

const std::string Water::MapKey = "water";
const Color4B Water::SurfaceColor = Color4B(105, 190, 206, 212);
const Color4B Water::BodyColor = Color4B(98, 186, 209, 64);
const float Water::WaterGravity = 0.0f;

Water* Water::create(ValueMap& properties)
{
	Water* instance = new Water(properties);

	instance->autorelease();

	return instance;
}

Water::Water(ValueMap& properties) : super(properties, 192.0f, (CollisionType)PlatformerCollisionType::Water, Water::SurfaceColor, Water::BodyColor, 0.015f, 0.005f, 0.05f)
{
}

Water::~Water()
{
}

void Water::initializeListeners()
{
	super::initializeListeners();

	this->liquidCollision->whenCollidesWith({ (int)PlatformerCollisionType::Player, (int)PlatformerCollisionType::Physics }, [=](CollisionData collisionData)
	{
		this->runObjectSplash(collisionData.other, false);

		return CollisionResult::DoNothing;
	});

	this->liquidCollision->whenStopsCollidingWith({ (int)PlatformerCollisionType::Player, (int)PlatformerCollisionType::Physics }, [=](CollisionData collisionData)
	{
		this->runObjectSplash(collisionData.other, true);

		return CollisionResult::DoNothing;
	});

	this->liquidCollision->whileCollidesWith({ (int)PlatformerCollisionType::Player, (int)PlatformerCollisionType::Physics }, [=](CollisionData collisionData)
	{
		this->applyWaterForce(collisionData.other, collisionData.dt);

		return CollisionResult::DoNothing;
	});
}

void Water::applyWaterForce(CollisionObject* target, float dt)
{
	target->setVelocity(target->getVelocity() + Vec2(0.0f, Water::WaterGravity * dt));
}
