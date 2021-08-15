#pragma once

#include "Engine/Hackables/Menus/HackablePreview.h"

namespace cocos2d
{
	class Sprite;
}

class ConstantString;
class LocalizedLabel;
class SmartAnimationSequenceNode;

class SpikedLogSetRotationPreview : public HackablePreview
{
public:
	static SpikedLogSetRotationPreview* create();

	HackablePreview* clone() override;

protected:
	SpikedLogSetRotationPreview();
	virtual ~SpikedLogSetRotationPreview();
	
	void onEnter() override;
	void initializePositions() override;

private:
	typedef HackablePreview super;

	void onFrameComplete();

	SmartAnimationSequenceNode* previewSpikedLog = nullptr;
	LocalizedLabel* ecxAnimationCount = nullptr;
	ConstantString* countString = nullptr;
	int currentAnimationIndex = -1;
	int animationLength = 0;
};
