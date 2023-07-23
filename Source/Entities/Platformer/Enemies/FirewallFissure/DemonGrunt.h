#pragma once
#include <string>

#include "Entities/Platformer/PlatformerEnemy.h"

class LocalizedString;

class DemonGrunt : public PlatformerEnemy
{
public:
	static DemonGrunt* deserialize(cocos2d::ValueMap& properties);

	cocos2d::Vec2 getDialogueOffset() override;
	LocalizedString* getEntityName() override;

	static const std::string MapKey;
	
protected:
	DemonGrunt(cocos2d::ValueMap& properties);
	virtual ~DemonGrunt();

private:
	typedef PlatformerEnemy super;
};
