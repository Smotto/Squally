#pragma once

#include "Engine/AttachedBehavior/AttachedBehavior.h"

namespace cocos2d
{
	class DrawNode;
}

class PlatformerEntity;

class EntityProjectileTargetBehavior : public AttachedBehavior
{
public:
	static EntityProjectileTargetBehavior* create(GameObject* owner);

	cocos2d::Node* getTarget();

	static const std::string MapKey;

protected:
	EntityProjectileTargetBehavior(GameObject* owner);
	virtual ~EntityProjectileTargetBehavior();

	void onDeveloperModeEnable(int debugLevel) override;
	void onDeveloperModeDisable() override;
	void onLoad() override;
	void onDisable() override;
	void initializePositions() override;

private:
	typedef AttachedBehavior super;

	PlatformerEntity* entity;

	cocos2d::Node* projectileTarget;
};
