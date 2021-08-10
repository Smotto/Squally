#pragma once

#include "Engine/Components/GameComponent.h"

class PlatformerEntity;
class Squally;

class HelperFollowMovementBehavior : public GameComponent
{
public:
	static HelperFollowMovementBehavior* create(GameObject* owner);

	void enableFollow();
	void disableFollow();

	static const std::string MapKey;

protected:
	HelperFollowMovementBehavior(GameObject* owner);
	virtual ~HelperFollowMovementBehavior();

	void onLoad() override;
	void onDisable() override;
	void update(float dt) override;

private:
	typedef GameComponent super;
	
	void warpToSqually();

	PlatformerEntity* entity;
	Squally* squally;
	
	bool followEnabled;

	static const float StopFollowRangeX;
	static const float TryJumpRangeY;
	static const float ResetRangeX;
	static const float ResetRangeY;

	static const std::string MapEventMoveToRest;
	static const std::string MapEventStopRest;
};
