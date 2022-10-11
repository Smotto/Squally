#pragma once

#include "Entities/Platformer/PlatformerEntity.h"

// Base class for player + npcs
class PlatformerFriendly : public PlatformerEntity
{
public:
	static const std::string PlatformerFriendlyTag;

protected:
	PlatformerFriendly(cocos2d::ValueMap& properties,
		std::string entityName,
		std::string scmlResource,
		std::string emblemResource,
		cocos2d::CSize size,
		float scale,
		cocos2d::Vec2 collisionOffset,
		float hoverHeight = 0.0f);
	virtual ~PlatformerFriendly();

private:
	typedef PlatformerEntity super;
};
