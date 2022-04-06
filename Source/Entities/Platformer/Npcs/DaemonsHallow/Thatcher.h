#pragma once
#include <string>

#include "Entities/Platformer/PlatformerFriendly.h"

class LocalizedString;

class Thatcher : public PlatformerFriendly
{
public:
	static Thatcher* deserialize(cocos2d::ValueMap& properties);

	cocos2d::Vec2 getDialogueOffset() override;
	LocalizedString* getEntityName() override;

	static const std::string MapKey;

protected:
	Thatcher(cocos2d::ValueMap& properties);
	virtual ~Thatcher();

private:
	typedef PlatformerFriendly super;
};
