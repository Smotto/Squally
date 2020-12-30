#pragma once

#include "Engine/Hackables/HackableObject.h"

class CollisionObject;
class PlatformerObjectDeserializer;
class SpawnPool;

class BreakableBase : public HackableObject
{
public:

protected:
	BreakableBase(cocos2d::ValueMap& properties, cocos2d::Size collisionSize, int requiredHits = 1);
	virtual ~BreakableBase();

	void onEnter() override;
	void initializePositions() override;
	void initializeListeners() override;
	virtual void onBreak();
	SpawnPool* getSpawnPool();

	PlatformerObjectDeserializer* objectDeserializer;
	cocos2d::Node* contentNode;

private:
	typedef HackableObject super;

	CollisionObject* breakableCollision;

	SpawnPool* spawnPool;
	bool hasBroke;
};
