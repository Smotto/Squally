#include "TempleTerrain.h"

#include "cocos/base/CCValue.h"

#include "Resources/TerrainResources.h"
#include "Resources/TextureResources.h"

using namespace cocos2d;

const std::string TempleTerrain::MapKeyTerrainType = "temple";

TempleTerrain* TempleTerrain::create(ValueMap& properties)
{
	TempleTerrain* instance = new TempleTerrain(properties);

	instance->autorelease();

	return instance;
}

TempleTerrain::TempleTerrain(ValueMap& properties) : super(
	properties,
	TerrainObject::TerrainData(
		0.5f,
		TempleTerrain::MapKeyTerrainType,
		TextureResources::TempleTexture,
		TerrainResources::Temple_Top,
		TerrainResources::Temple_TopLeft,
		TerrainResources::Temple_TopRight,
		TerrainResources::Temple_TopConnector,
		TerrainResources::BlueGrass_TopConnectorConcave,
		TerrainResources::BlueGrass_TopConnectorConcaveDeep,
		TerrainResources::BlueGrass_TopConnectorConvex,
		TerrainResources::BlueGrass_TopConnectorConvexDeep,
		TerrainResources::Temple_Bottom,
		TerrainResources::Temple_BottomLeft,
		TerrainResources::Temple_BottomRight,
		TerrainResources::Temple_BottomConnector,
		TerrainResources::Temple_Left,
		TerrainResources::Temple_LeftConnector,
		TerrainResources::Temple_Right,
		TerrainResources::Temple_RightConnector,
		// Top
		Vec2::ZERO,
		// Bottom
		Vec2::ZERO,
		// Left
		Vec2::ZERO,
		// Right
		Vec2::ZERO,
		// TopLeft
		Vec2::ZERO,
		// TopRight
		Vec2::ZERO,
		// BottomLeft
		Vec2::ZERO,
		// BottomRight
		Vec2::ZERO,
		// TopConnector
		Vec2::ZERO,
		// TopConnectorConcave
		Vec2::ZERO,
		// TopConnectorConcaveDeep
		Vec2::ZERO,
		// TopConnectorConvex
		Vec2::ZERO,
		// TopConnectorConvexDeep
		Vec2::ZERO,
		// BottomConnector
		Vec2::ZERO,
		// FillColor
		Color4B(11, 30, 39, 255)))
{
}

TempleTerrain::~TempleTerrain()
{
}