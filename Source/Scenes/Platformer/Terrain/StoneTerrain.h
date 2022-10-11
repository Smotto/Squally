#pragma once

#include "Engine/Terrain/TerrainObject.h"

class StoneTerrain : public TerrainObject
{
public:
	static StoneTerrain* create(cocos2d::ValueMap& properties);

	static const std::string MapKey;
	static const cocos2d::Color4B FillColor;
	static const cocos2d::Color4B MiniMapColor;

protected:
	StoneTerrain(cocos2d::ValueMap& properties);
	virtual ~StoneTerrain();

private:
	typedef TerrainObject super;
};
