#pragma once

#include "Entities/Platformer/PlatformerFriendly.h"

// Base class for player + npcs
class PlatformerHelper : public PlatformerFriendly
{
public:
	static const std::string PlatformerHelperTag;

protected:
	PlatformerHelper(cocos2d::ValueMap& properties,
		std::string entityName,
		std::string scmlResource,
		std::string emblemResource,
		cocos2d::CSize size,
		float scale,
		cocos2d::Vec2 collisionOffset,
		float hoverHeight = 0.0f);
	virtual ~PlatformerHelper();

private:
	typedef PlatformerFriendly super;
};
