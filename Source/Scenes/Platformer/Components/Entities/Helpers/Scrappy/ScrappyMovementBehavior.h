#pragma once

#include "Engine/Components/GameComponent.h"

class Scrappy;
class Squally;

class ScrappyMovementBehavior : public GameComponent
{
public:
	static ScrappyMovementBehavior* create(GameObject* owner);

	static const std::string MapKey;

protected:
	ScrappyMovementBehavior(GameObject* owner);
	virtual ~ScrappyMovementBehavior();

	void onLoad() override;
	void onDisable() override;
	void update(float dt) override;

private:
	typedef GameComponent super;
	
	void warpToSqually();

	Scrappy* scrappy;
	Squally* squally;

	float elapsed;

	static const float FloatOffsetRadius;
	static const float FloatOffsetAngle;
	static const cocos2d::Vec2 FlySpeedBase;
	static const float FlySpeedDistanceMultiplier;
};
