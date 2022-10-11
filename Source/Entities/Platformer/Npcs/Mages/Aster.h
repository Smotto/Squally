#pragma once
#include <string>

#include "Entities/Platformer/PlatformerFriendly.h"

class LocalizedString;

class Aster : public PlatformerFriendly
{
public:
	static Aster* deserialize(cocos2d::ValueMap& properties);

	cocos2d::Vec2 getDialogueOffset() override;
	LocalizedString* getEntityName() override;

	static const std::string MapKey;

protected:
	Aster(cocos2d::ValueMap& properties);
	virtual ~Aster();

private:
	typedef PlatformerFriendly super;
};
