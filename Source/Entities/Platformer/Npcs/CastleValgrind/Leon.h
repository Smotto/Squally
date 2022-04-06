#pragma once
#include <string>

#include "Entities/Platformer/PlatformerFriendly.h"

class LocalizedString;

class Leon : public PlatformerFriendly
{
public:
	static Leon* deserialize(cocos2d::ValueMap& properties);

	cocos2d::Vec2 getDialogueOffset() override;
	LocalizedString* getEntityName() override;

	static const std::string MapKey;

protected:
	Leon(cocos2d::ValueMap& properties);
	virtual ~Leon();

private:
	typedef PlatformerFriendly super;
};
