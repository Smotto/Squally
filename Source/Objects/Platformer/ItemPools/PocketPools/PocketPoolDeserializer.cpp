#include "PocketPoolDeserializer.h"

#include "cocos/base/CCValue.h"

#include "Engine/Maps/GameObject.h"
#include "Objects/Platformer/ItemPools/PocketPools/PocketPools.h"

using namespace cocos2d;

const std::string PocketPoolDeserializer::MapKeyTypePocketPool = "pocket-pool";

PocketPoolDeserializer* PocketPoolDeserializer::create()
{
	PocketPoolDeserializer* instance = new PocketPoolDeserializer();

	instance->autorelease();

	return instance;
}

PocketPoolDeserializer::PocketPoolDeserializer() : super(PocketPoolDeserializer::MapKeyTypePocketPool)
{
	this->deserializers = std::map<std::string, std::function<GameObject*(ValueMap)>>();

	// EF
	this->deserializers[PocketPoolEFGeneric::PoolName] = [=](ValueMap properties) { return (GameObject*)PocketPoolEFGeneric::create(properties); };
	this->deserializers[PocketPoolTownGuard::PoolName] = [=](ValueMap properties) { return (GameObject*)PocketPoolTownGuard::create(properties); };

	// UR
	this->deserializers[PocketPoolURGeneric::PoolName] = [=](ValueMap properties) { return (GameObject*)PocketPoolURGeneric::create(properties); };

	// DM
	this->deserializers[PocketPoolDMGeneric::PoolName] = [=](ValueMap properties) { return (GameObject*)PocketPoolDMGeneric::create(properties); };
}

PocketPoolDeserializer::~PocketPoolDeserializer()
{
}
