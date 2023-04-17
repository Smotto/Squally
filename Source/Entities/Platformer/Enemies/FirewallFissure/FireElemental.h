#pragma once
#include <string>

#include "Entities/Platformer/PlatformerEnemy.h"

class LocalizedString;

class FireElemental : public PlatformerEnemy
{
public:
	static FireElemental* deserialize(cocos2d::ValueMap& properties);

	cocos2d::Vec2 getDialogueOffset() override;
	LocalizedString* getEntityName() override;

	static const std::string MapKey;
	
protected:
	FireElemental(cocos2d::ValueMap& properties);
	virtual ~FireElemental();

private:
	typedef PlatformerEnemy super;
};
