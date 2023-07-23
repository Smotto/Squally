#pragma once
#include <string>

#include "Entities/Platformer/PlatformerEnemy.h"

class LocalizedString;

class DemonSwordsman : public PlatformerEnemy
{
public:
	static DemonSwordsman* deserialize(cocos2d::ValueMap& properties);

	cocos2d::Vec2 getDialogueOffset() override;
	LocalizedString* getEntityName() override;

	static const std::string MapKey;
	
protected:
	DemonSwordsman(cocos2d::ValueMap& properties);
	virtual ~DemonSwordsman();

private:
	typedef PlatformerEnemy super;
};
