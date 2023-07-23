#pragma once
#include <string>

#include "Entities/Platformer/PlatformerFriendly.h"

class LocalizedString;

class Cindra : public PlatformerFriendly
{
public:
	static Cindra* deserialize(cocos2d::ValueMap& properties);

	cocos2d::Vec2 getDialogueOffset() override;
	LocalizedString* getEntityName() override;

	static const std::string MapKey;

protected:
	Cindra(cocos2d::ValueMap& properties);
	virtual ~Cindra();

private:
	typedef PlatformerFriendly super;
};
