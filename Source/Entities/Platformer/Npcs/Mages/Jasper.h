#pragma once
#include <string>

#include "Entities/Platformer/PlatformerFriendly.h"

class LocalizedString;

class Jasper : public PlatformerFriendly
{
public:
	static Jasper* deserialize(cocos2d::ValueMap& properties);

	cocos2d::Vec2 getDialogueOffset() override;
	LocalizedString* getEntityName() override;

	static const std::string MapKey;

protected:
	Jasper(cocos2d::ValueMap& properties);
	virtual ~Jasper();

private:
	typedef PlatformerFriendly super;
};
