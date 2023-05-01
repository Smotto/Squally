#include "Jingles.h"

#include "cocos/math/CCGeometry.h"

#include "Resources/EntityResources.h"
#include "Resources/UIResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

const std::string Jingles::MapKey = "jingles";

Jingles* Jingles::deserialize(ValueMap& properties)
{
	Jingles* instance = new Jingles(properties);

	instance->autorelease();

	return instance;
}

Jingles::Jingles(ValueMap& properties) : super(properties,
	Jingles::MapKey,
	EntityResources::Npcs_BallmerPeaks_Jingles_Animations,
	EntityResources::Npcs_BallmerPeaks_Jingles_Emblem,
	CSize(112.0f, 160.0f),
	0.85f)
{
}

Jingles::~Jingles()
{
}

Vec2 Jingles::getDialogueOffset()
{
	return Vec2(0.0f, 0.0f);
}

LocalizedString* Jingles::getEntityName()
{
	return Strings::Platformer_Entities_Names_Npcs_BallmerPeaks_Jingles::create();
}
