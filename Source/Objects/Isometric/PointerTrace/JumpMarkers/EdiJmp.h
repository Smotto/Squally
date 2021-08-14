#pragma once

#include "Objects/Isometric/PointerTrace/JumpMarkers/JmpMarker.h"

namespace cocos2d
{
	class Sprite;
}

class EdiJmp : public JmpMarker
{
public:
	static EdiJmp* create(cocos2d::ValueMap& properties);

	static const std::string MapKey;

protected:
	EdiJmp(cocos2d::ValueMap& properties);
	virtual ~EdiJmp();

	void onEnter() override;
	void initializePositions() override;
	int getJumpDestination() override;

private:
	typedef JmpMarker super;

	cocos2d::Sprite* marker = nullptr;
};
