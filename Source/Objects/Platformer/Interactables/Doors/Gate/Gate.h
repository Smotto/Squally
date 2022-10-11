#pragma once

#include "Objects/Platformer/Interactables/Doors/Portal.h"

namespace cocos2d
{
	class Sprite;
}

class ClickableNode;

class Gate : public Portal
{
public:
	static Gate* create(cocos2d::ValueMap& properties);

	void lock(bool animate = true) override;
	void unlock(bool animate = true) override;

	static const std::string MapKey;

protected:
	Gate(cocos2d::ValueMap& properties);
	virtual ~Gate();

	void onEnter() override;
	void initializePositions() override;
	void initializeListeners() override;
	cocos2d::Vec2 getButtonOffset() override;
	HackablePreview* createDefaultPreview() override;

private:
	typedef Portal super;

	cocos2d::Sprite* gateRight = nullptr;
	cocos2d::Sprite* gateLeft = nullptr;

	ClickableNode* lockButton = nullptr;
	ClickableNode* unlockButton = nullptr;

	static const cocos2d::Vec2 GateLeftClosedOffset;
	static const cocos2d::Vec2 GateRightClosedOffset;
	static const cocos2d::Vec2 GateLeftOpenedOffset;
	static const cocos2d::Vec2 GateRightOpenedOffset;
};
