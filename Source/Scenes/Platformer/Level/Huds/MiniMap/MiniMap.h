#pragma once

#include "Engine/UI/HUD/Hud.h"

namespace cocos2d
{
	class DrawNode;

	namespace cocos_experimental
	{
		class TMXTiledMap;
	};
};

class Inventory;
class LayerDeserializer;
class MiniGameMap;
class MiniMapObject;
class MiniMapTerrainObject;
class SmartClippingNode;
class Squally;

class MiniMap : public Hud
{
public:
	static MiniMap* create();

	void show(bool instant = false);
	void hide(bool instant = false);
	void setPositioning(std::string miniMapPositioning = "");
	bool loadMapFromTmx(std::string mapResource, cocos2d::cocos_experimental::TMXTiledMap* mapRaw);

protected:
	MiniMap();
	virtual ~MiniMap();
	
	void onHackerModeEnable() override;
	void onHackerModeDisable() override;
	void initializePositions() override;
	void initializeListeners() override;
	void update(float dt) override;

	void addLayerDeserializer(LayerDeserializer* layerDeserializer);
	void addLayerDeserializers(std::vector<LayerDeserializer*> layerDeserializers);

	cocos2d::Node* rootNode;
	cocos2d::DrawNode* background;
	cocos2d::Node* mapNode;
	MiniGameMap* map;
	cocos2d::DrawNode* squallyMarker;
	SmartClippingNode* mapClip;

	std::string mapResource;

private:
	typedef Hud super;

	bool hasRequiredItem();
	void initializeMapData();
	void positionMiniMap();
	void positionEntityIcons();

	Squally* squally = nullptr;
	Inventory* squallyInventory;
	MiniGameMap* squallyMap;
	cocos2d::Node* contentNode;

	std::vector<LayerDeserializer*> layerDeserializers;
	std::map<MiniMapTerrainObject*, float> miniMapTerrainObjects;
	std::map<MiniMapObject*, float> miniMapObjects;
	
	bool isShown;
	std::string requiredItemKey;

	static const float MiniMapScale;
	static const cocos2d::CSize MiniMapSize;
	static const cocos2d::Vec2 MiniMapMargin;
};
