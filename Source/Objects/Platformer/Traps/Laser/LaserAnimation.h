#pragma once

#include "Engine/SmartNode.h"

namespace cocos2d
{
	class Sprite;
}

class LaserAnimation : public SmartNode
{
public:
	static LaserAnimation* create(float height);

	void runAnimation(std::function<void()> onBeamActiveCallback = nullptr, std::function<void()> onBeamDeactiveCallback = nullptr);

protected:
	LaserAnimation(float height);
	virtual	~LaserAnimation();

	void onEnter() override;
	void initializePositions() override;

private:
	typedef SmartNode super;

	cocos2d::Sprite* laserHeadTop = nullptr;
	cocos2d::Sprite* laserHeadBottom = nullptr;
	cocos2d::Sprite* laserWeak = nullptr;
	cocos2d::Sprite* laserStrong = nullptr;

	float height = 0.0f;
};
