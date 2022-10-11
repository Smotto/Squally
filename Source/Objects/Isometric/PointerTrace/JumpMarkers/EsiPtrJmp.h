#pragma once

#include "Objects/Isometric/PointerTrace/JumpMarkers/JmpMarker.h"

namespace cocos2d
{
	class Sprite;
}

class EsiPtrJmp : public JmpMarker
{
public:
	static EsiPtrJmp* create(cocos2d::ValueMap& properties);

	static const std::string MapKey;

protected:
	EsiPtrJmp(cocos2d::ValueMap& properties);
	virtual ~EsiPtrJmp();

	void onEnter() override;
	void initializePositions() override;
	int getJumpDestination() override;

private:
	typedef JmpMarker super;

	cocos2d::Sprite* marker = nullptr;
};
