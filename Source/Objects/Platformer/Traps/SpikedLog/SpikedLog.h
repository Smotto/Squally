#pragma once

#include "Engine/Hackables/HackableObject.h"

namespace cocos2d
{
	class Sprite;
}

class CollisionObject;
class HackableData;
class SmartAnimationSequenceNode;

class SpikedLog : public HackableObject
{
public:
	static SpikedLog* create(cocos2d::ValueMap& properties);

	static const std::string MapKey;

protected:
	SpikedLog(cocos2d::ValueMap& properties);
	virtual ~SpikedLog();

	void onEnter() override;
	void initializePositions() override;
	cocos2d::Vec2 getButtonOffset() override;
	void registerHackables() override;
	HackablePreview* createDefaultPreview() override;

private:
	typedef HackableObject super;

	void onFrameComplete();
	int incrementSpikedLogAnimation(int count, int max);

	int animationLength = 0;
	int currentAnimationIndex = 0;
	cocos2d::Sprite* beam = nullptr;
	SmartAnimationSequenceNode* spikedLog = nullptr;
	CollisionObject* spikeCollision = nullptr;
	CollisionObject* logCollision = nullptr;
};
