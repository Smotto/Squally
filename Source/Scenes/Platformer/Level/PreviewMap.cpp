#include "PreviewMap.h"

#include "Deserializers/Platformer/PlatformerComponentDeserializer.h"
#include "Deserializers/Platformer/PlatformerBannerDeserializer.h"
#include "Deserializers/Platformer/PlatformerCrackDeserializer.h"
#include "Deserializers/Platformer/PlatformerDecorDeserializer.h"
#include "Deserializers/Platformer/PlatformerEntityDeserializer.h"
#include "Deserializers/Platformer/PlatformerHideMiniMapDeserializer.h"
#include "Deserializers/Platformer/PlatformerMiniMapRequiredItemDeserializer.h"
#include "Deserializers/Platformer/PlatformerObjectDeserializer.h"
#include "Deserializers/Platformer/PlatformerQuestDeserializer.h"
#include "Deserializers/Platformer/PlatformerTerrainDeserializer.h"
#include "Deserializers/Platformer/PlatformerTextureDeserializer.h"
#include "Deserializers/WeatherDeserializer.h"
#include "Engine/Deserializers/Meta/BackgroundDeserializer.h"
#include "Engine/Deserializers/Meta/MetaLayerDeserializer.h"
#include "Engine/Deserializers/Meta/MusicDeserializer.h"
#include "Engine/Deserializers/Meta/PhysicsDeserializer.h"
#include "Engine/Deserializers/Objects/CollisionDeserializer.h"
#include "Engine/Deserializers/Objects/ObjectLayerDeserializer.h"
#include "Engine/GlobalDirector.h"
#include "Engine/Maps/GameMap.h"
#include "Engine/Physics/CollisionObject.h"
#include "Engine/Utils/GameUtils.h"
#include "Engine/Utils/StrUtils.h"

#include "Resources/MapResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

PreviewMap* PreviewMap::create(std::string mapResource)
{
	PreviewMap* instance = new PreviewMap();

	instance->autorelease();

	instance->loadMap(mapResource);

	return instance;
}

PreviewMap::PreviewMap() : super()
{
	if (!PreviewMap::init())
	{
		return;
		// throw std::uncaught_exceptions();
	}
	
	this->layerDeserializers = std::vector<LayerDeserializer*>();

	this->map = nullptr;
	this->mapNode = Node::create();

	this->addLayerDeserializers({
			MetaLayerDeserializer::create(
			{
				BackgroundDeserializer::create(),
				PlatformerCrackDeserializer::create(),
				PhysicsDeserializer::create(),
			}),
			ObjectLayerDeserializer::create({
				{ CollisionDeserializer::MapKeyTypeCollision, CollisionDeserializer::create({ (PropertyDeserializer*)PlatformerComponentDeserializer::create(), (PropertyDeserializer*)PlatformerQuestDeserializer::create() }) },
				{ PlatformerDecorDeserializer::MapKeyTypeDecor, PlatformerDecorDeserializer::create() },
				{ PlatformerEntityDeserializer::MapKeyTypeEntity, PlatformerEntityDeserializer::create() },
				{ PlatformerObjectDeserializer::MapKeyTypeObject, PlatformerObjectDeserializer::create() },
				{ PlatformerTerrainDeserializer::MapKeyTypeTerrain, PlatformerTerrainDeserializer::create() },
				{ PlatformerTextureDeserializer::MapKeyTypeTexture, PlatformerTextureDeserializer::create() },
			}),
			WeatherDeserializer::create()
		}
	);

	this->addChild(this->mapNode);
}

PreviewMap::~PreviewMap()
{
}

CSize PreviewMap::getMapSize()
{
	if (this->map == nullptr)
	{
		return CSize::ZERO;
	}

	return this->map->getMapSize();
}

void PreviewMap::addLayerDeserializer(LayerDeserializer* layerDeserializer)
{
	this->addChild(layerDeserializer);
	this->layerDeserializers.push_back(layerDeserializer);
}

void PreviewMap::addLayerDeserializers(std::vector<LayerDeserializer*> layerDeserializers)
{
	for (auto next : layerDeserializers)
	{
		this->addChild(next);
		this->layerDeserializers.push_back(next);
	}
}

bool PreviewMap::loadMap(std::string mapResource)
{
	// Previews need to exist in their own universes, to prevent collision between universes
	unsigned int previousUniverseId = CollisionObject::UniverseId;
	CollisionObject::AlternateUniverseCounter++;
	CollisionObject::UniverseId = CollisionObject::AlternateUniverseCounter;

	if (this->map != nullptr)
	{
		this->mapNode->removeChild(this->map);
	}

	this->mapResource = mapResource;
	this->map = GameMap::deserialize(this->mapResource, GameMap::parse(this->mapResource), this->layerDeserializers, true);

	CollisionObject::UniverseId = previousUniverseId;
	
	if (this->map != nullptr)
	{
		this->mapNode->addChild(this->map);

		return true;
	}

	return false;
}
