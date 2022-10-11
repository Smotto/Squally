#pragma once

#include "Engine/Hackables/Menus/HackablePreview.h"

class LocalizedLabel;
class SmartAnimationSequenceNode;

class FireballSpeedPreview : public HackablePreview
{
public:
	static FireballSpeedPreview* create();

	HackablePreview* clone() override;

protected:
	FireballSpeedPreview();
	virtual ~FireballSpeedPreview();

	void onEnter() override;
	void initializePositions() override;

private:
	typedef HackablePreview super;

	SmartAnimationSequenceNode* fireball = nullptr;
	LocalizedLabel* xmm1Left = nullptr;
	LocalizedLabel* xmm1Right = nullptr;
};
