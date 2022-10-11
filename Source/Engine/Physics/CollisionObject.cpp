#include "CollisionObject.h"

#include "cocos/2d/CCDrawNode.h"
#include "cocos/base/CCDirector.h"
#include "cocos/base/CCEventCustom.h"
#include "cocos/base/CCEventListenerCustom.h"
#include "cocos/base/CCEventDispatcher.h"
#include "cocos/base/CCValue.h"

#include "Engine/Camera/GameCamera.h"
#include "Engine/DeveloperMode/DeveloperModeController.h"
#include "Engine/Events/CollisionEvents.h"
#include "Engine/Events/ObjectEvents.h"
#include "Engine/GlobalDirector.h"
#include "Engine/Physics/CollisionResolver.h"
#include "Engine/SmartScene.h"
#include "Engine/Utils/AlgoUtils.h"
#include "Engine/Utils/GameUtils.h"
#include "Engine/Utils/LogUtils.h"
#include "Engine/Utils/MathUtils.h"

using namespace cocos2d;

const std::string CollisionObject::MapKeyTypeCollision = "collision";
const std::string CollisionObject::MapKeyCollisionTypeNone = "none";
const std::string CollisionObject::MapKeyFriction = "friction";

std::map<CollisionType, std::vector<CollisionObject*>> CollisionObject::CollisionObjects = std::map<CollisionType, std::vector<CollisionObject*>>();

const float CollisionObject::DefaultGravity = -768.0f;
const float CollisionObject::DefaultMaxHorizontalSpeed = 600.0f;
const float CollisionObject::DefaultMaxLaunchSpeed = 720.0f;
const float CollisionObject::DefaultMaxFallSpeed = -840.0f;
const float CollisionObject::DefaultHorizontalDampening = 0.75f;
const float CollisionObject::DefaultVerticalDampening = 1.0f;
const float CollisionObject::CollisionZThreshold = 12.0f;
unsigned int CollisionObject::UniverseId = 0;
unsigned int CollisionObject::AlternateUniverseCounter = 0;

CollisionObject* CollisionObject::create(const ValueMap& properties, std::vector<Vec2> points, CollisionType collisionType, Properties collisionProperties, Color4F debugColor)
{
	CollisionObject* instance = new CollisionObject(properties, points, collisionType, collisionProperties, debugColor);

	instance->autorelease();

	return instance;
}

CollisionObject* CollisionObject::create(std::vector<Vec2> points, CollisionType collisionType, Properties collisionProperties, Color4F debugColor)
{
	ValueMap valueMap = ValueMap();

	return CollisionObject::create(valueMap, points, collisionType, collisionProperties, debugColor);
}

CollisionObject::CollisionObject(const ValueMap& properties, std::vector<Vec2> points, CollisionType collisionType, Properties collisionProperties, Color4F debugColor) :
	super(properties)
{
	this->collisionType = collisionType;
	this->points = points;
	this->pointsRotated = points;
	this->segments = AlgoUtils::buildSegmentsFromPoints(this->points);
	this->segmentsRotated = this->segments;
	this->shape = this->determineShape();
	this->boundsRect = AlgoUtils::getPolygonRect(this->pointsRotated);
	this->currentCollisions = &this->collisionsRed;
	this->previousCollisions = &this->collisionsBlack;
	this->collisionProperties = collisionProperties;
	this->gravity = Vec2(0.0f, CollisionObject::DefaultGravity);
	this->horizontalDampening = CollisionObject::DefaultHorizontalDampening;
	this->verticalDampening = CollisionObject::DefaultVerticalDampening;
	this->debugColor = debugColor;
	this->collisionDepth = CollisionObject::CollisionZThreshold;
	this->universeId = CollisionObject::UniverseId;

	this->setGravityFlagEnabled(collisionProperties.isDynamic, (int)EngineGravityFlags::Default);

	for (int shift = 0; shift <= sizeof(int) * 8; shift++)
	{
		if ((1 << shift) & this->collisionType)
		{
			this->collisionTypes.push_back(1 << shift);
		}
	}
}

CollisionObject::~CollisionObject()
{
}

void CollisionObject::onEnter()
{
	super::onEnter();

	CollisionObject::RegisterCollisionObject(this);

	this->currentCollisions->clear();
	this->previousCollisions->clear();

	this->scheduleUpdate();
}

void CollisionObject::onEnterTransitionDidFinish()
{
	super::onEnterTransitionDidFinish();
}

void CollisionObject::onExit()
{
	super::onExit();

	CollisionObject::UnregisterCollisionObject(this);
}

void CollisionObject::onDeveloperModeEnable(int debugLevel)
{
	super::onDeveloperModeEnable(debugLevel);

	this->drawDebugShapes();
}

void CollisionObject::onDeveloperModeDisable()
{
	super::onDeveloperModeDisable();
	
	if (this->debugDrawNode != nullptr)
	{
		this->debugDrawNode->setVisible(false);
	}
}

void CollisionObject::onDespawn()
{
	super::onDespawn();

	CollisionObject::UnregisterCollisionObject(this);
}

void CollisionObject::update(float dt)
{
	super::update(dt);

	this->propagateRotation();
	this->runPhysics(1.0f / 60.0f);

	this->drawDebugConnectors();
}

void CollisionObject::runPhysics(float dt)
{
	if (!this->getPhysicsEnabled() || this->isDespawned())
	{
		return;
	}

	// Apply gravity & accel
	this->velocity += (this->getGravityEnabled()) ? (this->gravity * dt) : Vec2::ZERO;
	this->velocity += this->acceleration * dt;

	if (this->velocity != Vec2::ZERO)
	{
		// Apply dampening
		this->velocity.x *= this->horizontalDampening;
		this->velocity.y *= this->verticalDampening;

		// Enforce constraints by calling setter
		this->setVelocity(this->velocity);

		const float rotationInRad = this->cachedRotation * float(M_PI) / 180.0f;
		const float cosTheta = std::cos(rotationInRad);
		const float sinTheta = std::sin(rotationInRad);

		const Vec3 positionUpdates = Vec3(
			(this->velocity.x * cosTheta - this->velocity.y * sinTheta) * dt,
			(this->velocity.x * sinTheta + this->velocity.y * cosTheta) * dt,
			0.0f
		);

		this->setThisOrBindPosition(this->getThisOrBindPosition() + positionUpdates);
	}

	// Pass off current collisions to previous
	this->previousCollisions->clear();
	this->previousCollisions = this->currentCollisions;
	this->currentCollisions = this->previousCollisions == &this->collisionsBlack ? &this->collisionsRed : &this->collisionsBlack;
	
	if (!this->getCollisionEnabled())
	{
		return;
	}

	for (auto collisionType : this->collidesWithTypes)
	{
		for (auto collisionObject : CollisionObject::CollisionObjects[collisionType])
		{
			if (collisionObject->isDespawned())
			{
				continue;
			}
			
			CollisionResolver::resolveCollision(this, collisionObject, [=]()
			{
				CollisionResult collisionResult = CollisionResult::DoNothing;

				this->currentCollisions->insert(collisionObject);

				if (this->previousCollisions->find(collisionObject) == this->previousCollisions->end())
				{
					// Run new collision events
					for (auto event : this->collisionStartEvents[collisionType])
					{
						CollisionResult nextResult = event.collisionEvent(CollisionData(collisionObject, dt));

						collisionResult = nextResult == CollisionResult::DoNothing ? collisionResult : nextResult;
					}
				}
				else
				{
					// Run collision-sustain events
					for (auto event : this->collisionSustainEvents[collisionType])
					{
						CollisionResult nextResult = event.collisionEvent(CollisionData(collisionObject, dt));

						collisionResult = nextResult == CollisionResult::DoNothing ? collisionResult : nextResult;
					}
				}

				return collisionResult;
			});
		}
	}

	// Run collision end events
	for (auto collisionObject : *this->previousCollisions)
	{
		if (this->currentCollisions->find(collisionObject) == this->currentCollisions->end())
		{
			for (auto collisionType : collisionObject->getCollisionTypes())
			{
				for (auto event : this->collisionEndEvents[collisionType])
				{
					event.collisionEvent(CollisionData(collisionObject, dt));
				}
			}
		}
	}
}

void CollisionObject::setCollisionDepth(float collisionDepth)
{
	this->collisionDepth = collisionDepth;

	// Invalidate debug info
	this->debugInfoSpawned = false;
}

void CollisionObject::setGravityFlagEnabled(bool isEnabled, int flagIndex)
{
	gravityDisabledFlags = MathUtils::setBit(gravityDisabledFlags, flagIndex, !isEnabled);
}

bool CollisionObject::getGravityFlagEnabled(int flagIndex)
{
	return !MathUtils::getBit(gravityDisabledFlags, flagIndex);
}

bool CollisionObject::getGravityEnabled()
{
	return gravityDisabledFlags == 0;
}

void CollisionObject::setPhysicsFlagEnabled(bool isEnabled, int flagIndex)
{
	physicsDisabledFlags = MathUtils::setBit(physicsDisabledFlags, flagIndex, !isEnabled);
}

bool CollisionObject::getPhysicsFlagEnabled(int flagIndex)
{
	return !MathUtils::getBit(physicsDisabledFlags, flagIndex);
}

bool CollisionObject::getPhysicsEnabled()
{
	return physicsDisabledFlags == 0;
}

void CollisionObject::setCollisionFlagEnabled(bool isEnabled, int flagIndex)
{
	collisionDisabledFlags = MathUtils::setBit(collisionDisabledFlags, flagIndex, !isEnabled);
}

bool CollisionObject::getCollisionFlagEnabled(int flagIndex)
{
	return !MathUtils::getBit(collisionDisabledFlags, flagIndex);
}

bool CollisionObject::getCollisionEnabled()
{
	return collisionDisabledFlags == 0;
}

Vec2 CollisionObject::getVelocity()
{
	return this->velocity;
}

Vec2 CollisionObject::getAcceleration()
{
	return this->acceleration;
}

void CollisionObject::setVelocity(Vec2 velocity)
{
	this->velocity.x = MathUtils::clamp(velocity.x, -CollisionObject::DefaultMaxHorizontalSpeed, CollisionObject::DefaultMaxHorizontalSpeed);
	this->velocity.y = MathUtils::clamp(velocity.y, CollisionObject::DefaultMaxFallSpeed, CollisionObject::DefaultMaxLaunchSpeed);
}

void CollisionObject::setGravity(cocos2d::Vec2 gravity)
{
	this->gravity = gravity;
}

Vec2 CollisionObject::getGravity()
{
	return this->gravity;
}

void CollisionObject::setAcceleration(Vec2 acceleration)
{
	this->acceleration = acceleration;
}

void CollisionObject::setVelocityX(float velocityX)
{
	this->setVelocity(Vec2(velocityX, this->getVelocity().y));
}

void CollisionObject::setVelocityY(float velocityY)
{
	this->setVelocity(Vec2(this->getVelocity().x, velocityY));
}

void CollisionObject::setAccelerationX(float accelerationX)
{
	this->setAcceleration(Vec2(accelerationX, this->getAcceleration().y));
}

void CollisionObject::setAccelerationY(float accelerationY)
{
	this->setAcceleration(Vec2(this->getAcceleration().x, accelerationY));
}

void CollisionObject::setHorizontalDampening(float horizontalDampening)
{
	this->horizontalDampening = MathUtils::clamp(horizontalDampening, 0.0f, 1.0f);
}

void CollisionObject::setVerticalDampening(float verticalDampening)
{
	this->verticalDampening = MathUtils::clamp(verticalDampening, 0.0f, 1.0f);
}

const std::vector<cocos2d::Vec2>& CollisionObject::getPoints()
{
	return this->points;
}

void CollisionObject::setPoints(std::vector<Vec2> points)
{
	this->points = points;
	this->pointsRotated = this->points;
	this->segments = AlgoUtils::buildSegmentsFromPoints(this->points);
	this->segmentsRotated = this->segments;
	this->shape = this->determineShape();
	this->boundsRect = AlgoUtils::getPolygonRect(this->pointsRotated);
	
	this->propagateRotation(true);
	
	// Invalidate debug info
	this->debugInfoSpawned = false;

	if (DeveloperModeController::getDebugLevel() > 0)
	{
		this->drawDebugShapes();
	}
}

CollisionShape CollisionObject::getShape()
{
	return this->shape;
}
	
CollisionType CollisionObject::getCollisionType()
{
	return this->collisionType;
}
	
std::vector<CollisionType> CollisionObject::getCollisionTypes()
{
	return this->collisionTypes;
}

bool CollisionObject::hasCollisionType(CollisionType collisionType)
{
	return (this->collisionType & int(collisionType)) != 0;
}

bool CollisionObject::hasCollisionWith(CollisionObject* collisonObject)
{
	Node* currentCollisionGroup = collisonObject->getParent();

	for (auto next : this->getCurrentCollisions())
	{
		if (next->getParent() != currentCollisionGroup)
		{
			continue;
		}

		return true;
	}

	return false;
}

const std::set<CollisionObject*>& CollisionObject::getCurrentCollisions()
{
	return *this->currentCollisions;
}

bool CollisionObject::hasCollisions()
{
	return !this->currentCollisions->empty();
}

bool CollisionObject::isCollidingWithSingleGroup()
{
	if (!this->hasCollisions())
	{
		return false;
	}

	Node* parent = nullptr;

	for (auto next : *this->currentCollisions)
	{
		if (parent == nullptr)
		{
			parent = next->getParent();
		}
		else if (parent != next->getParent())
		{
			return false;
		}
	}

	return true;
}

bool CollisionObject::isCollidingWith(CollisionObject* collisionObject)
{
	return std::find(this->currentCollisions->begin(), this->currentCollisions->end(), collisionObject) != this->currentCollisions->end();
}

bool CollisionObject::wasCollidingWith(CollisionObject* collisionObject)
{
	return std::find(this->previousCollisions->begin(), this->previousCollisions->end(), collisionObject) != this->previousCollisions->end();
}

bool CollisionObject::isCollidingWithType(int collisionType)
{
	for (CollisionObject* next : *this->currentCollisions)
	{
		if (next->hasCollisionType(CollisionType(collisionType)))
		{
			return true;
		}
	}

	return false;
}

bool CollisionObject::wasCollidingWithType(int collisionType)
{
	for (CollisionObject* next : *this->previousCollisions)
	{
		if (next->hasCollisionType(CollisionType(collisionType)))
		{
			return true;
		}
	}

	return false;
}

void CollisionObject::warpTo(cocos2d::Vec3 location)
{
	this->setThisOrBindPosition(location);
}

void CollisionObject::bindTo(GameObject* bindTarget)
{
	this->bindTarget = bindTarget;
}

void CollisionObject::unbind()
{
	this->bindTarget = nullptr;
}

void CollisionObject::whenCollidesWith(std::vector<CollisionType> collisionTypes, std::function<CollisionResult(CollisionData)> onCollision)
{
	for (auto collisionType : collisionTypes)
	{
		this->collidesWithTypes.insert(collisionType);
		this->collisionStartEvents[collisionType].push_back(CollisionEvent(onCollision));
	}
}

void CollisionObject::whileCollidesWith(std::vector<CollisionType> collisionTypes, std::function<CollisionResult(CollisionData)> onCollision)
{
	for (auto collisionType : collisionTypes)
	{
		this->collidesWithTypes.insert(collisionType);
		this->collisionSustainEvents[collisionType].push_back(CollisionEvent(onCollision));
	}
}

void CollisionObject::ifCollidesWith(std::vector<CollisionType> collisionTypes, std::function<CollisionResult(CollisionData)> onCollision)
{
	for (auto collisionType : collisionTypes)
	{
		this->collidesWithTypes.insert(collisionType);
		this->collisionStartEvents[collisionType].push_back(CollisionEvent(onCollision));
		this->collisionSustainEvents[collisionType].push_back(CollisionEvent(onCollision));
	}
}

void CollisionObject::whenStopsCollidingWith(std::vector<CollisionType> collisionTypes, std::function<CollisionResult(CollisionData)> onCollisionEnd)
{
	for (auto collisionType : collisionTypes)
	{
		this->collidesWithTypes.insert(collisionType);
		this->collisionEndEvents[collisionType].push_back(CollisionEvent(onCollisionEnd));
	}
}

unsigned int CollisionObject::getUniverseId()
{
	return this->universeId;
}

std::vector<Vec2> CollisionObject::createCircle(float radius, int segments)
{
	std::vector<Vec2> points = std::vector<Vec2>();

	for (int index = 0; index < segments; index++)
	{
		float progress = float(index) / float(segments);

		points.push_back(Vec2(
			std::cos(2.0f * float(M_PI) * progress) * radius,
			std::sin(2.0f * float(M_PI) * progress) * radius
		));
	}

	return points;
}

std::vector<Vec2> CollisionObject::createBox(CSize size)
{
	return std::vector<Vec2>({
		Vec2(-size.width / 2.0f, -size.height / 2.0f),
		Vec2(-size.width / 2.0f, size.height / 2.0f),
		Vec2(size.width / 2.0f, size.height / 2.0f),
		Vec2(size.width / 2.0f, -size.height / 2.0f)
	});
}

std::vector<Vec2> CollisionObject::createCapsulePolygon(CSize size, float capsuleRadius)
{
	size.height = std::max(0.0f, size.height - capsuleRadius * 2.0f);

	std::vector<Vec2> points = std::vector<Vec2>();

	// Right side
	points.push_back(Vec2(size.width / 2.0f, size.height / 2.0f));
	points.push_back(Vec2(size.width / 2.0f, -size.height / 2.0f));

	// Bottom capsule
	points.push_back(Vec2(0.0f, -size.height / 2.0f - capsuleRadius));

	// Left side
	points.push_back(Vec2(-size.width / 2.0f, -size.height / 2.0f));
	points.push_back(Vec2(-size.width / 2.0f, size.height / 2.0f));

	// Top capsule
	points.push_back(Vec2(0.0f, size.height / 2.0f + capsuleRadius));

	return points; // PhysicsBody::createPolygon(points.data(), points.size(), PhysicsMaterial(0.5f, 0.0f, friction));
}

Vec3 CollisionObject::getThisOrBindPosition()
{
	if (this->bindTarget == nullptr)
	{
		return GameUtils::getWorldCoords3D(this);
	}
	else
	{
		return GameUtils::getWorldCoords3D(this->bindTarget);
	}
}

void CollisionObject::setThisOrBindPosition(Vec3 position)
{
	if (this->bindTarget == nullptr)
	{
		GameUtils::setWorldCoords3D(this, position);
	}
	else
	{
		GameUtils::setWorldCoords3D(this->bindTarget, position);
	}

	this->computeWorldCoords(true);
}

CollisionShape CollisionObject::determineShape()
{
	if (this->points.size() == 2)
	{
		return CollisionShape::Segment;
	}
	else if (this->points.size() == 4)
	{
		int sameX = 0;
		int sameY = 0;
		Vec2 prior = this->points.back();

		for (auto point : this->points)
		{
			sameX += (point.x == prior.x ? 1 : 0);
			sameY += (point.y == prior.y ? 1 : 0);

			prior = point;
		}

		return (sameX == 2 && sameY == 2) ? CollisionShape::Rectangle : CollisionShape::Quad;
	}

	return CollisionShape::Polygon;
}

void CollisionObject::computeDepth(bool force)
{
	if (this->cachedTickDepth != SmartScene::GlobalTick || force)
	{
		this->cachedTickDepth = SmartScene::GlobalTick;
		this->cachedWorldCoords3D.z = GameUtils::getDepth(this);
	}
}

void CollisionObject::computeWorldCoords(bool force)
{
	if (this->cachedTick != SmartScene::GlobalTick || force)
	{
		this->cachedTick = SmartScene::GlobalTick;
		this->cachedTickDepth = SmartScene::GlobalTick;
		this->cachedWorldCoords3D = GameUtils::getWorldCoords3D(this, false);
		this->cachedWorldCoords.x = this->cachedWorldCoords3D.x;
		this->cachedWorldCoords.y = this->cachedWorldCoords3D.y;
	}
}

void CollisionObject::propagateRotation(bool force)
{
	float rotation = GameUtils::getRotation(this);

	if (!force && rotation == this->cachedRotation)
	{
		return;
	}

	this->cachedRotation = rotation;

	const float rotationInRad = -this->cachedRotation * float(M_PI) / 180.0f;
	const float cosTheta = std::cos(rotationInRad);
	const float sinTheta = std::sin(rotationInRad);

	for (int index = 0; index < int(this->points.size()); index++)
	{
		this->pointsRotated[index].x = this->points[index].x * cosTheta - this->points[index].y * sinTheta;
		this->pointsRotated[index].y = this->points[index].x * sinTheta + this->points[index].y * cosTheta;
	}

	this->segmentsRotated = AlgoUtils::buildSegmentsFromPoints(this->pointsRotated);
}

void CollisionObject::drawDebugShapes()
{
	if (this->debugDrawNode != nullptr)
	{
		this->debugDrawNode->setVisible(true);
	}

	if (this->debugInfoSpawned || DeveloperModeController::getDebugLevel() <= 0)
	{
		return;
	}

	if (this->debugDrawNode == nullptr)
	{
		this->debugDrawNode = Node::create();

		this->addChild(this->debugDrawNode);
	}

	this->debugDrawNode->removeAllChildren();

	DrawNode* collisionBack = DrawNode::create();
	this->debugDrawNodeConnectors = DrawNode::create();
	DrawNode* collisionFront = DrawNode::create();

	if (!this->points.empty())
	{
		if (this->points.size() == 2)
		{
			// collisionBack->drawSegment(this->points.front(), this->points.back(), 1.0f, this->debugColor);
			collisionFront->drawSegment(this->points.front(), this->points.back(), 1.0f, this->debugColor);
		}
		else
		{
			// collisionBack->drawPolygon(this->points.data(), this->points.size(), Color4F(this->debugColor.r, this->debugColor.g, this->debugColor.b, 0.4f * this->debugColor.a), 1.0f, this->debugColor);
			collisionFront->drawPolygon(this->points.data(), this->points.size(), Color4F(this->debugColor.r, this->debugColor.g, this->debugColor.b, 0.4f * this->debugColor.a), 1.0f, this->debugColor);
		}
	}

	// collisionBack->setPositionZ(-this->collisionDepth);
	// collisionFront->setPositionZ(this->collisionDepth);

	this->debugDrawNode->addChild(collisionBack);
	this->debugDrawNode->addChild(this->debugDrawNodeConnectors);
	this->debugDrawNode->addChild(collisionFront);

	this->debugInfoSpawned = true;
}

void CollisionObject::drawDebugConnectors()
{
	// This function isn't quite right, so I'm just leaving it disabled. It seems to be 50/50 in drawing correctly, or dead wrong.
	return;

	if (this->debugDrawNodeConnectors == nullptr || DeveloperModeController::getDebugLevel() <= 0)
	{
		return;
	}

	this->debugDrawNodeConnectors->clear();

	const Vec2 projectedCoords = GameUtils::getScreenBounds(this).origin;
	static const Vec2 HalfScreenSize = Vec2(Director::getInstance()->getVisibleSize()) / 2.0f;
	const Vec2 cameraPos = GameCamera::getInstance()->getCameraPosition();
	const Vec2 offset = cameraPos - HalfScreenSize;

	for (auto next : this->pointsRotated)
	{
		// Compute edge verticies in world coords
		const Vec3 sourcePoint = this->cachedWorldCoords3D + Vec3(next.x, next.y, -this->collisionDepth);
		const Vec3 destPoint = this->cachedWorldCoords3D + Vec3(next.x, next.y, this->collisionDepth);

		// Project into screen coords
		const Vec2 projectedSource = GameUtils::getScreenCoords(sourcePoint);
		const Vec2 projectedDest = GameUtils::getScreenCoords(destPoint);
		const Vec2 normalizedLine = projectedDest - projectedSource;

		/*
		// Convert into world coords
		const Vec2 worldSource = projectedSource + offset;
		const Vec2 worldDest = projectedDest + offset;

		// Convert into local coords
		const Vec2 localSource = worldSource - this->cachedWorldCoords;
		const Vec2 localDest = worldDest - this->cachedWorldCoords;
		*/

		const Vec2 localSource = next - normalizedLine / 2.0f;
		const Vec2 localDest = next + normalizedLine / 2.0f;

		this->debugDrawNodeConnectors->drawSegment(localSource, localDest, 1.0f, this->debugColor);
	}

}

void CollisionObject::ClearCollisionObjects()
{
	CollisionObject::CollisionObjects.clear();
}

void CollisionObject::RegisterCollisionObject(CollisionObject* collisionObject)
{
	if (collisionObject == nullptr)
	{
		return;
	}

	for (auto next : collisionObject->collisionTypes)
	{
		CollisionObject::CollisionObjects[next].push_back(collisionObject);
	}
}

void CollisionObject::UnregisterCollisionObject(CollisionObject* collisionObject)
{
	if (collisionObject == nullptr)
	{
		return;
	}

	for (auto next : collisionObject->collisionTypes)
	{
		CollisionObject::CollisionObjects[next].erase(
			std::remove(
				CollisionObject::CollisionObjects[next].begin(),
				CollisionObject::CollisionObjects[next].end(),
				collisionObject
			), CollisionObject::CollisionObjects[next].end()
		);
	}
}
