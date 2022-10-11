#pragma once
#include <string>

#include "Entities/Platformer/PlatformerHelper.h"

class LocalizedString;

class Ghost : public PlatformerHelper
{
public:
	static Ghost* deserialize(cocos2d::ValueMap& properties);

	cocos2d::Vec2 getDialogueOffset() override;
	LocalizedString* getEntityName() override;

	static const std::string MapKey;
	
protected:
	Ghost(cocos2d::ValueMap& properties);
	virtual ~Ghost();

private:
	typedef PlatformerHelper super;
};
