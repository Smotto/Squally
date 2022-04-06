#pragma once
#include <string>

#include "Entities/Platformer/PlatformerFriendly.h"

class LocalizedString;

class Necron : public PlatformerFriendly
{
public:
	static Necron* deserialize(cocos2d::ValueMap& properties);

	cocos2d::Vec2 getDialogueOffset() override;
	LocalizedString* getEntityName() override;

	static const std::string MapKey;

protected:
	Necron(cocos2d::ValueMap& properties);
	virtual ~Necron();

private:
	typedef PlatformerFriendly super;
};
