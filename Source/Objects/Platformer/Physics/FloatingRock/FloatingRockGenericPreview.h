#pragma once

#include "Engine/Hackables/Menus/HackablePreview.h"

namespace cocos2d
{
	class Sprite;
}

class FloatingRockGenericPreview : public HackablePreview
{
public:
	static FloatingRockGenericPreview* create();

	HackablePreview* clone() override;

protected:
	FloatingRockGenericPreview();
	virtual ~FloatingRockGenericPreview();

	void onEnter() override;
	void initializePositions() override;

private:
	typedef HackablePreview super;

	cocos2d::Sprite* previewAsteroid;
};
