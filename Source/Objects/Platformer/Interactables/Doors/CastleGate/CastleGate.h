#pragma once

#include "Engine/Hackables/HackableObject.h"

namespace cocos2d
{
	class Sprite;
}

class CollisionObject;
class HackableData;
class SmartClippingNode;

class CastleGate : public HackableObject
{
public:
	static CastleGate* create(cocos2d::ValueMap& properties);

	static const std::string MapKey;

protected:
	CastleGate(cocos2d::ValueMap& properties);
	virtual ~CastleGate();

	void onEnter() override;
	void initializeListeners() override;
	void initializePositions() override;
	void update(float) override;
	cocos2d::Vec2 getButtonOffset() override;
	void registerHackables() override;

private:
	typedef HackableObject super;

	void onDoorEventTriggered(cocos2d::ValueMap args);

	bool isFlipped = false;
	float doorClosedPosition = 0.0f;
	cocos2d::Sprite* castleBack = nullptr;
	SmartClippingNode* doorClip = nullptr;
	cocos2d::Sprite* door = nullptr;
	cocos2d::Sprite* bridge1 = nullptr;
	cocos2d::Sprite* bridge2 = nullptr;
	CollisionObject* doorCollision = nullptr;
	cocos2d::Sprite* castleDoorFront = nullptr;
	cocos2d::Sprite* castleDoorFront2 = nullptr;

	static const float DoorOpenDelta;
};
