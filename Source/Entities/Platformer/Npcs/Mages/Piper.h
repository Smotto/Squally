#pragma once
#include <string>

#include "Entities/Platformer/PlatformerFriendly.h"

class LocalizedString;

class Piper : public PlatformerFriendly
{
public:
	static Piper* deserialize(cocos2d::ValueMap& properties);

	cocos2d::Vec2 getDialogueOffset() override;
	LocalizedString* getEntityName() override;

	static const std::string MapKey;

protected:
	Piper(cocos2d::ValueMap& properties);
	virtual ~Piper();

private:
	typedef PlatformerFriendly super;
};
