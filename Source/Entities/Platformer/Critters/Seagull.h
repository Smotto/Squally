#pragma once
#include <string>

#include "Entities/Platformer/PlatformerFriendly.h"

class LocalizedString;

class Seagull : public PlatformerFriendly
{
public:
	static Seagull* deserialize(cocos2d::ValueMap& properties);

	cocos2d::Vec2 getDialogueOffset() override;
	LocalizedString* getEntityName() override;

	static const std::string MapKey;
	
protected:
	Seagull(cocos2d::ValueMap& properties);
	virtual ~Seagull();

private:
	typedef PlatformerFriendly super;
};
