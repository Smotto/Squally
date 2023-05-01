#include "EntityHoverCollisionBehavior.h"

#include "Engine/Animations/SmartAnimationNode.h"
#include "Engine/Physics/CollisionObject.h"
#include "Engine/Utils/MathUtils.h"
#include "Entities/Platformer/PlatformerEntity.h"
#include "Scenes/Platformer/Components/Entities/Collision/EntityCollisionBehaviorBase.h"
#include "Scenes/Platformer/Components/Entities/Collision/EntityGroundCollisionBehavior.h"
#include "Scenes/Platformer/Components/Entities/Collision/EntityJumpCollisionBehavior.h"
#include "Scenes/Platformer/Components/Entities/Movement/EntityMountBehavior.h"
#include "Scenes/Platformer/Level/Physics/PlatformerPhysicsTypes.h"
#include "Scenes/Platformer/State/StateKeys.h"

#include "Resources/EntityResources.h"

using namespace cocos2d;

const std::string EntityHoverCollisionBehavior::MapKey = "entity-movement-collisions";

EntityHoverCollisionBehavior* EntityHoverCollisionBehavior::create(GameObject* owner)
{
	EntityHoverCollisionBehavior* instance = new EntityHoverCollisionBehavior(owner);

	instance->autorelease();

	return instance;
}

EntityHoverCollisionBehavior::EntityHoverCollisionBehavior(GameObject* owner) : super(owner)
{
	this->entity = dynamic_cast<PlatformerEntity*>(owner);

	if (this->entity == nullptr)
	{
		this->invalidate();
	}
}

EntityHoverCollisionBehavior::~EntityHoverCollisionBehavior()
{
	if (this->hoverCollision != nullptr && this->hoverCollision->getParent() != nullptr && this->hoverCollision->getParent() != this)
	{
		this->hoverCollision->getParent()->removeChild(this->hoverCollision);
	}
}

void EntityHoverCollisionBehavior::onLoad()
{
	this->entity->watchForComponent<EntityCollisionBehaviorBase>([=](EntityCollisionBehaviorBase* entityCollision)
	{
		this->entityCollision = entityCollision;
	});

	this->entity->watchForComponent<EntityGroundCollisionBehavior>([=](EntityGroundCollisionBehavior* groundCollision)
	{
		this->groundCollision = groundCollision;
	});

	this->entity->watchForComponent<EntityJumpCollisionBehavior>([=](EntityJumpCollisionBehavior* jumpCollision)
	{
		this->jumpCollision = jumpCollision;
	});
	
	this->defer([=]()
	{
		this->buildHoverAntiGravityCollision();
		this->buildHoverCollision();
		this->positionHoverCollision();
		this->toggleQueryable(true);
		
		// Jump collision detector can be incorrectly initialized if the entity is spawned dynamically (ie Squally in the intro cutscene).
		// Hack fix because I don't wan't to do root cause analysis.
		this->defer([=]()
		{
			this->positionHoverCollision();
		});
	});

	this->scheduleUpdate();
}

void EntityHoverCollisionBehavior::onDisable()
{
	super::onDisable();
	
	if (this->hoverCollision != nullptr)
	{
		this->hoverCollision->setPhysicsFlagEnabled(false);
	}
}

void EntityHoverCollisionBehavior::update(float dt)
{
	super::update(dt);

	bool autoCrouch = false;
	this->entity->getComponent<EntityMountBehavior>([&](EntityMountBehavior* entityMountBehavior)
	{
		if (entityMountBehavior->isMounted())
		{
			autoCrouch = true;
		}
	});

	if (autoCrouch || this->entity->getRuntimeStateOrDefaultFloat(StateKeys::MovementY, 0.0f) < 0.0f)
	{
		this->crouch(dt);
	}
	else
	{
		this->uncrouch(dt);
	}
	
	if (this->entityCollision != nullptr && this->entityCollision->movementCollision != nullptr)
	{
		this->entityCollision->movementCollision->setGravityFlagEnabled(true, int(GravityFlags::Hover));
		
		if (this->hoverAntiGravityCollisionDetector != nullptr && this->hoverAntiGravityTopCollisionDetector != nullptr)
		{
			// Update anti-gravity to avoid jitter
			if (this->hoverAntiGravityCollisionDetector->hasCollisions() && !this->hoverAntiGravityTopCollisionDetector->hasCollisions())
			{
				this->entityCollision->movementCollision->setGravityFlagEnabled(false, int(GravityFlags::Hover));

				if (this->entityCollision->movementCollision->getVelocity().y < 0.0f)
				{
					this->entityCollision->movementCollision->setVelocityY(0.0f);
				}
			}
		}
	}
}

void EntityHoverCollisionBehavior::crouch(float dt)
{
	static const float TimeToCrouch = 0.25f;

	if (this->crouchProgress >= 1.0f)
	{
		return;
	}

	this->crouchProgress = MathUtils::clamp(this->crouchProgress + dt * (1.0f / TimeToCrouch), 0.0f, 1.0f);

	this->rebuildHoverCrouchCollision();
}

void EntityHoverCollisionBehavior::uncrouch(float dt)
{
	static const float TimeToUncrouch = 0.25f;

	if (this->crouchProgress <= 0.0f)
	{
		return;
	}

	this->crouchProgress = MathUtils::clamp(this->crouchProgress - dt * (1.0f / TimeToUncrouch), 0.0f, 1.0f);

	this->rebuildHoverCrouchCollision();
}

void EntityHoverCollisionBehavior::rebuildHoverCrouchCollision()
{
	if (this->hoverCollision == nullptr)
	{
		return;
	}
	
	static const float MinCrouch = 0.85f;
	static const float MaxCrouch = 1.0f;

	float inverseProgress = MathUtils::clamp(1.0f - this->crouchProgress, 0.0f, 1.0f);
	CSize hoverSize = this->getHoverSize(inverseProgress);

	this->hoverCollision->setPoints(CollisionObject::createBox(hoverSize));
	this->positionHoverCollision(inverseProgress);
}

CSize EntityHoverCollisionBehavior::getHoverSize(float progress)
{
	static const float UpwardsPadding = 40.0f;
	static const float CrouchLowPoint = this->entity->getHoverHeight() / 3.0f;
	static const float MinCrouch =  CrouchLowPoint;
	static const float MaxCrouch =  this->entity->getHoverHeight();

	float crouchHeight = MinCrouch + (MaxCrouch - MinCrouch) * progress;

	return CSize(
		std::max(this->entity->getEntitySize().width - 8.0f, 16.0f),
		crouchHeight + UpwardsPadding
	);
}

void EntityHoverCollisionBehavior::positionHoverCollision(float progress)
{
	if (this->hoverCollision == nullptr)
	{
		return;
	}

	// Upwards padding exists to help in collisions, as it will result in the "closest" points of a collision being near the bottom of the hover
	static const float UpwardsPadding = 40.0f;
	static const float CrouchLowPoint = this->entity->getHoverHeight() / 3.0f;
	static const float MinCrouchGround =  -CrouchLowPoint + EntityGroundCollisionBehavior::GroundCollisionOffset - EntityGroundCollisionBehavior::GroundCollisionHeight / 2.0f;
	static const float MaxCrouchGround =  -this->entity->getHoverHeight() + EntityGroundCollisionBehavior::GroundCollisionOffset - EntityGroundCollisionBehavior::GroundCollisionHeight / 2.0f;
	static const float MinCrouchJump =  -CrouchLowPoint + EntityJumpCollisionBehavior::JumpCollisionOffset - EntityJumpCollisionBehavior::JumpCollisionHeight / 2.0f;
	static const float MaxCrouchJump =  -this->entity->getHoverHeight() + EntityJumpCollisionBehavior::JumpCollisionOffset - EntityJumpCollisionBehavior::JumpCollisionHeight / 2.0f;

	static const float OriginalHoverHeight = this->entity->getHoverHeight();
	static const Vec2 Offset = Vec2(0.0f, -OriginalHoverHeight / 2.0f + UpwardsPadding);

	const CSize hoverSize = this->getHoverSize(progress);
	const float hoverDelta = OriginalHoverHeight - hoverSize.height;

	if (this->hoverCollision != nullptr)
	{
		this->hoverCollision->setPosition(Offset + Vec2(0.0f, hoverDelta / 2.0f));
	}

	// Reposition ground/jump collision to the bottom of the hover
	const float crouchAdjustGroundY = MinCrouchGround + (MaxCrouchGround - MinCrouchGround) * progress;
	const float crouchAdjustJumpY = MinCrouchJump + (MaxCrouchJump - MinCrouchJump) * progress;

	if (this->groundCollision != nullptr && this->groundCollision->getGroundCollision() != nullptr)
	{
		this->groundCollision->getGroundCollision()->setPosition(Vec2(0.0f, crouchAdjustGroundY));
	}

	if (this->jumpCollision != nullptr && this->jumpCollision->getJumpCollision() != nullptr)
	{
		this->jumpCollision->getJumpCollision()->setPosition(Vec2(0.0f, crouchAdjustJumpY));
	}
	
	// Reposition anti-gravity detectors
	static const float AntiGravityDetectorSize = 32.0f;

	if (this->hoverAntiGravityCollisionDetector != nullptr)
	{
		this->hoverAntiGravityCollisionDetector->setPosition(Vec2(0.0f,  crouchAdjustGroundY));
	}

	if (this->hoverAntiGravityCollisionDetector != nullptr)
	{
		this->hoverAntiGravityTopCollisionDetector->setPosition(Vec2(0.0f,  crouchAdjustGroundY) + Vec2(0.0f, AntiGravityDetectorSize));
	}
}

void EntityHoverCollisionBehavior::buildHoverCollision()
{
	if (this->hoverCollision != nullptr || this->entity->getHoverHeight() <= 0.0f)
	{
		return;
	}

	CSize hoverSize = this->getHoverSize();
	CollisionType collisionType = CollisionType(PlatformerCollisionType::Hover);

	this->hoverCollision = CollisionObject::create(
		CollisionObject::createBox(hoverSize),
		// CollisionObject::createCapsulePolygon(hoverSize, 8.0f),
		collisionType,
		CollisionObject::Properties(true, false, 1.0f, 1.0f),
		Color4F::BLUE
	);
	
	this->hoverCollision->setName("entity movement");
	this->hoverCollision->bindTo(this->entity);
	this->hoverCollision->setGravityFlagEnabled(false);

	this->addChild(this->hoverCollision);
	
	this->hoverCollision->whileCollidesWith({ (int)PlatformerCollisionType::Solid, (int)PlatformerCollisionType::PassThrough, (int)PlatformerCollisionType::SolidPlayerOnly }, [=](CollisionData collisionData)
	{
		static const float HoverSpeed = 128.0f;
		static const float MinSpeed = 0.0f;

		if (this->groundCollision == nullptr
			|| this->entityCollision == nullptr
			|| this->entityCollision->movementCollision == nullptr
			|| this->groundCollision->getGroundCollision() == nullptr
			|| this->entity->controlState == PlatformerEntity::ControlState::Swimming)
		{
			return CollisionResult::DoNothing;
		}

		if (this->groundCollision->isStandingOnSomethingOtherThan(collisionData.other))
		{
			return CollisionResult::DoNothing;
		}
		
		// Special case to collide with physics when standing on a single surface.
		// This prevents a bug where the hover collision pushes the player up too slowly, allowing them
		// to clip into the ground due to all of the detectors (ground, jump, etc) being below the surface
		// Edit: This results in pretty trash behavior when hitting square corners at certain angles
		// In retrospect, it doesn't even feel very necessary.
		/*
		if (this->hoverAntiGravityCollisionDetector->isCollidingWithSingleGroup()
			&& this->hoverAntiGravityTopCollisionDetector->isCollidingWithSingleGroup()
			&& this->hoverCollision->isCollidingWithSingleGroup()
			&& !this->entityCollision->movementCollision->hasCollisions()
			&& !this->entityCollision->hasLeftWallCollision()
			&& !this->entityCollision->hasRightWallCollision()
			&& collisionData.other->getParent() == (*this->hoverAntiGravityCollisionDetector->getCurrentCollisions().begin())->getParent()
			&& collisionData.other->getParent() == (*this->hoverAntiGravityTopCollisionDetector->getCurrentCollisions().begin())->getParent())
		{
			return CollisionResult::CollideWithPhysics;
		}*/

		if (this->entityCollision != nullptr)
		{
			Vec2 velocity = entityCollision->movementCollision->getVelocity();
			Vec2 gravity = entityCollision->movementCollision->getGravity();
			float speedBoost = this->hoverAntiGravityCollisionDetector->hasCollisions() ? HoverSpeed : (HoverSpeed * 4.0f);

			velocity.y = velocity.y <= MinSpeed ? MinSpeed : velocity.y;

			// Apply hover speed
			entityCollision->movementCollision->setVelocity(velocity + (Vec2(0.0f, speedBoost) - gravity) * collisionData.dt);
		}

		return CollisionResult::DoNothing;
	});
}

void EntityHoverCollisionBehavior::buildHoverAntiGravityCollision()
{
	static const float AntiGravityDetectorSize = 32.0f;

	if (this->hoverAntiGravityCollisionDetector != nullptr || this->hoverAntiGravityTopCollisionDetector != nullptr || this->entity->getHoverHeight() <= 0.0f)
	{
		return;
	}

	float detectorWidth = std::max((this->entity->getEntitySize()).width + EntityGroundCollisionBehavior::GroundCollisionPadding * 2.0f, 8.0f);

	this->hoverAntiGravityCollisionDetector = CollisionObject::create(
		CollisionObject::createBox(CSize(detectorWidth, AntiGravityDetectorSize)),
		(int)PlatformerCollisionType::GroundDetector,
		CollisionObject::Properties(false, false),
		Color4F::PURPLE
	);

	this->hoverAntiGravityTopCollisionDetector = CollisionObject::create(
		CollisionObject::createBox(CSize(detectorWidth, AntiGravityDetectorSize)),
		(int)PlatformerCollisionType::GroundDetector,
		CollisionObject::Properties(false, false),
		Color4F::GREEN
	);

	this->addChild(this->hoverAntiGravityCollisionDetector);
	this->addChild(this->hoverAntiGravityTopCollisionDetector);

	this->hoverAntiGravityTopCollisionDetector->whenCollidesWith({ (int)PlatformerCollisionType::Solid, (int)PlatformerCollisionType::SolidRoof, (int)PlatformerCollisionType::PassThrough, (int)PlatformerCollisionType::Physics }, [=](CollisionData collisionData)
	{
		return CollisionResult::DoNothing;
	});

	this->hoverAntiGravityCollisionDetector->whenCollidesWith({ (int)PlatformerCollisionType::Solid, (int)PlatformerCollisionType::SolidRoof, (int)PlatformerCollisionType::PassThrough, (int)PlatformerCollisionType::Physics }, [=](CollisionData collisionData)
	{
		// See update(float dt) for anti-gravity logic

		return CollisionResult::DoNothing;
	});
}
