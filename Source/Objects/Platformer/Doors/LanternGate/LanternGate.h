#pragma once

#include "Objects/Platformer/Doors/Portal.h"

namespace cocos2d
{
	class Sprite;
}

class ClickableNode;
class CollisionObject;
class ConstantString;
class InteractMenu;
class LocalizedLabel;
class SmartAnimationSequenceNode;
class SmartClippingNode;
class Sound;

class LanternGate : public Portal
{
public:
	static LanternGate* create(cocos2d::ValueMap& initProperties);

	void lock(bool animate = true) override;
	void unlock(bool animate = true) override;

	static const std::string MapKeyLanternGate;

protected:
	LanternGate(cocos2d::ValueMap& initProperties);
	~LanternGate();

	void onEnter() override;
	void initializePositions() override;
	void initializeListeners() override;
	cocos2d::Vec2 getButtonOffset() override;
	HackablePreview* createDefaultPreview() override;

private:
	typedef Portal super;

	cocos2d::Sprite* gateRight;
	cocos2d::Sprite* gateLeft;

	ClickableNode* lockButton;
	ClickableNode* unlockButton;
};