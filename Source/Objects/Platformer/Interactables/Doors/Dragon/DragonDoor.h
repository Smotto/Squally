#pragma once

#include "Objects/Platformer/Interactables/Doors/Portal.h"

namespace cocos2d
{
	class Sprite;
}

class CollisionObject;
class WorldSound;

class DragonDoor : public Portal
{
public:
	static DragonDoor* create(cocos2d::ValueMap& properties);

	void lock(bool animate = true) override;
	void unlock(bool animate = true) override;

	static const std::string MapKey;

protected:
	DragonDoor(cocos2d::ValueMap& properties);
	virtual ~DragonDoor();
	
	void onEnter() override;
	void initializePositions() override;

private:
	typedef Portal super;

	cocos2d::Sprite* base = nullptr;
	cocos2d::Sprite* bars = nullptr;
	cocos2d::Sprite* top = nullptr;
	cocos2d::Sprite* teeth = nullptr;

	WorldSound* doorOpenSound = nullptr;

	static const float DoorOpenDelta;
	static const std::string UnlockedSaveKey;
	static const cocos2d::Vec2 TeethPositionBottom;
	static const cocos2d::Vec2 BarPositionBottom;
};
