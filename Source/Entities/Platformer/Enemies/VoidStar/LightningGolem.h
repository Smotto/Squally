#pragma once
#include <string>

#include "Entities/Platformer/PlatformerEnemy.h"

class LocalizedString;

class LightningGolem : public PlatformerEnemy
{
public:
	static LightningGolem* deserialize(cocos2d::ValueMap& properties);

	cocos2d::Vec2 getDialogueOffset() override;
	LocalizedString* getEntityName() override;

	static const std::string MapKey;
	
protected:
	LightningGolem(cocos2d::ValueMap& properties);
	virtual ~LightningGolem();

private:
	typedef PlatformerEnemy super;
};
