#pragma once

#include "Engine/Components/GameComponent.h"


class EntityCollisionBehaviorBase;
class EntityGroundCollisionBehavior;
class PlatformerEntity;
class WorldSound;

class EntityMovementBehavior : public GameComponent
{
public:
	static EntityMovementBehavior* create(GameObject* owner);

	cocos2d::Vec2 getVelocity() const;

	void setMoveAcceleration(float moveAcceleration);
	void setSwimAcceleration(cocos2d::Vec2 swimAcceleration);
	void setJumpVelocity(float jumpVelocity);

	void cancelWaterSfx();

	static const std::string MapKey;
	static const float DefaultWalkAcceleration;
	static const float DefaultRunAcceleration;
	static const float DefaultSprintAcceleration;
	static const cocos2d::Vec2 DefaultSwimAcceleration;
	static const float DefaultJumpVelocity;

protected:
	EntityMovementBehavior(GameObject* owner);
	virtual ~EntityMovementBehavior();

	void onLoad() override;
	void onDisable() override;
	void update(float dt) override;

private:
	typedef GameComponent super;

	void applyCinematicMovement(cocos2d::Vec2* movement);
	void applyPatrolMovement(cocos2d::Vec2* movement);
	void checkCinematicMovementComplete();
	void checkPatrolMovementComplete();

	float onGroundDuration = 0.0f;
	float moveAcceleration = 0.0f;
	cocos2d::Vec2 swimAcceleration;
	float jumpVelocity = 0.0f;

	PlatformerEntity* entity = nullptr;

	WorldSound* jumpSound = nullptr;
	std::vector<WorldSound*> swimSounds;
	std::vector<WorldSound*> walkSounds;
	
	EntityCollisionBehaviorBase* entityCollision = nullptr;
	EntityGroundCollisionBehavior* groundCollision = nullptr;

	int swimSoundIndex = 0;
	int walkSoundIndex = 0;
};
