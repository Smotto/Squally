#pragma once
#include <string>

#include "Entities/Platformer/PlatformerFriendly.h"

class LocalizedString;

class Fraya : public PlatformerFriendly
{
public:
	static Fraya* deserialize(cocos2d::ValueMap& properties);

	cocos2d::Vec2 getDialogueOffset() override;
	LocalizedString* getEntityName() override;

	static const std::string MapKey;

protected:
	Fraya(cocos2d::ValueMap& properties);
	virtual ~Fraya();

private:
	typedef PlatformerFriendly super;
};
