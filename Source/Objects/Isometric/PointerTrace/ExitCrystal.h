#pragma once

#include "Objects/Isometric/PointerTrace/GridObject.h"

namespace cocos2d
{
	class Sprite;
}

class SmartAnimationSequenceNode;

class ExitCrystal : public GridObject
{
public:
	static ExitCrystal* create(cocos2d::ValueMap& properties);

	int getValue();
	int getOffset();

	static const std::string MapKey;

protected:
	ExitCrystal(cocos2d::ValueMap& properties);
	virtual ~ExitCrystal();

	void onEnter() override;
	void initializePositions() override;
	void initializeListeners() override;


private:
	typedef GridObject super;

	cocos2d::Sprite* crystal = nullptr;
	cocos2d::Sprite* shadow = nullptr;
	SmartAnimationSequenceNode* shineFx = nullptr;
};
