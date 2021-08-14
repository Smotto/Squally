#pragma once

#include "Scenes/Platformer/Components/Entities/Collision/EntityCollisionBehaviorBase.h"

class PlatformerEntity;

class FriendlyCombatCollisionBehavior : public EntityCollisionBehaviorBase
{
public:
	static FriendlyCombatCollisionBehavior* create(GameObject* owner);

	static const std::string MapKey;

protected:
	FriendlyCombatCollisionBehavior(GameObject* owner);
	virtual ~FriendlyCombatCollisionBehavior();

	void onLoad() override;
	void onDisable() override;
	void onEntityCollisionCreated() override;
	void tryBind() override;

private:
	typedef EntityCollisionBehaviorBase super;

	PlatformerEntity* owner = nullptr;
};
