#include "WarpGateDH.h"

#include "Scenes/Platformer/Level/PreviewMap.h"

#include "Resources/MapResources.h"
#include "Resources/UIResources.h"

using namespace cocos2d;

const std::string WarpGateDH::MapKey = "warp-gate-dh";

WarpGateDH* WarpGateDH::create(ValueMap& properties)
{
	WarpGateDH* instance = new WarpGateDH(properties);

	instance->autorelease();

	return instance;
}

WarpGateDH::WarpGateDH(ValueMap& properties) : super(properties)
{
	PreviewMap* map = PreviewMap::create(MapResources::WarpGatePreviewMaps_EndianForest);

	map->setPositionX(-map->getMapSize().width / 2.0f);
	map->setPositionY(-map->getMapSize().height / 2.0f);
	map->setPositionZ(-320.0f);

	this->mapNode->addChild(map);
}

WarpGateDH::~WarpGateDH()
{
}
