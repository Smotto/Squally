#pragma once

#include "Objects/Isometric/PointerTrace/JumpMarkers/JmpMarker.h"

namespace cocos2d
{
	class Sprite;
}

class EbpPtrJmp : public JmpMarker
{
public:
	static EbpPtrJmp* create(cocos2d::ValueMap& properties);

	static const std::string MapKey;

protected:
	EbpPtrJmp(cocos2d::ValueMap& properties);
	virtual ~EbpPtrJmp();

	void onEnter() override;
	void initializePositions() override;
	int getJumpDestination() override;

private:
	typedef JmpMarker super;

	cocos2d::Sprite* marker = nullptr;
};
