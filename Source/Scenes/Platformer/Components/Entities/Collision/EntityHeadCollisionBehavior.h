#pragma once

#include "Engine/Components/GameComponent.h"

class CollisionObject;
class PlatformerEntity;

class EntityHeadCollisionBehavior : public GameComponent
{
public:
	static EntityHeadCollisionBehavior* create(GameObject* owner);

	bool hasHeadCollisionWith(CollisionObject* collisonObject);
	
	CollisionObject* headCollision;

	static const std::string MapKey;

protected:
	EntityHeadCollisionBehavior(GameObject* owner);
	virtual ~EntityHeadCollisionBehavior();

	void onLoad() override;
	void onDisable() override;

private:
	typedef GameComponent super;

	void buildHeadCollisionDetector();

	PlatformerEntity* entity;

	static const float HeadCollisionPadding;
	static const float HeadCollisionSize;
};
