#pragma once
#include <string>

#include "Entities/Platformer/PlatformerEnemy.h"

class LocalizedString;

class DarkTiger : public PlatformerEnemy
{
public:
	static DarkTiger* deserialize(cocos2d::ValueMap& properties);

	cocos2d::Vec2 getDialogueOffset() override;
	LocalizedString* getEntityName() override;

	static const std::string MapKey;
	
protected:
	DarkTiger(cocos2d::ValueMap& properties);
	virtual ~DarkTiger();

private:
	typedef PlatformerEnemy super;
};
