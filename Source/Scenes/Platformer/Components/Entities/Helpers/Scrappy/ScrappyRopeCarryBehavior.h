#pragma once

#include "Engine/Components/GameComponent.h"

namespace cocos2d
{
	class Sprite;
};

class PlatformerEntity;

class ScrappyRopeCarryBehavior : public GameComponent
{
public:
	static ScrappyRopeCarryBehavior* create(GameObject* owner);

	void setCarryTarget(GameObject* target);

	static const std::string MapKey;

protected:
	ScrappyRopeCarryBehavior(GameObject* owner);
	virtual ~ScrappyRopeCarryBehavior();

	void onLoad() override;
	void onDisable() override;
	void update(float dt) override;

private:
	typedef GameComponent super;

	PlatformerEntity* entity;
	GameObject* target;
	cocos2d::Sprite* rope;
	cocos2d::Sprite* ropeWaist;

	float ropeRotation;

	static const float FixedRopeDistance;
};
