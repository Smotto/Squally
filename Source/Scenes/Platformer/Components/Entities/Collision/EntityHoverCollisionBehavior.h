#pragma once

#include "Engine/Components/GameComponent.h"

class CollisionObject;
class EntityCollisionBehaviorBase;
class EntityGroundCollisionBehavior;
class EntityJumpCollisionBehavior;
class PlatformerEntity;

class EntityHoverCollisionBehavior : public GameComponent
{
public:
	static EntityHoverCollisionBehavior* create(GameObject* owner);

	static const std::string MapKey;

protected:
	EntityHoverCollisionBehavior(GameObject* owner);
	virtual ~EntityHoverCollisionBehavior();

	void onLoad() override;
	void onDisable() override;
	void update(float dt) override;

private:
	typedef GameComponent super;

	void crouch(float dt);
	void uncrouch(float dt);
	void rebuildHoverCrouchCollision();
	cocos2d::CSize getHoverSize(float progress = 1.0f);
	void positionHoverCollision(float progress = 1.0f);

	float crouchProgress;

	PlatformerEntity* entity;
	CollisionObject* hoverCollision;
	CollisionObject* hoverAntiGravityCollisionDetector;
	CollisionObject* hoverAntiGravityTopCollisionDetector;

	EntityCollisionBehaviorBase* entityCollision;
	EntityGroundCollisionBehavior* groundCollision;
	EntityJumpCollisionBehavior* jumpCollision;

	void buildHoverAntiGravityCollision();
	void buildHoverCollision();
};
