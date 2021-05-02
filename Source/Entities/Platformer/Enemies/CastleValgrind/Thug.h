#pragma once
#include <string>

#include "Entities/Platformer/PlatformerEnemy.h"

class LocalizedString;

class Thug : public PlatformerEnemy
{
public:
	static Thug* deserialize(cocos2d::ValueMap& properties);

	cocos2d::Vec2 getDialogueOffset() override;
	LocalizedString* getEntityName() override;

	static const std::string MapKey;
	
protected:
	Thug(cocos2d::ValueMap& properties);
	virtual ~Thug();

private:
	typedef PlatformerEnemy super;
};
