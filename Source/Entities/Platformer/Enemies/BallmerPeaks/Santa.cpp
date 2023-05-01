#include "Santa.h"

#include "cocos/math/CCGeometry.h"

#include "Resources/EntityResources.h"
#include "Resources/UIResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

const std::string Santa::MapKey = "santa";

Santa* Santa::deserialize(ValueMap& properties)
{
	Santa* instance = new Santa(properties);

	instance->autorelease();

	return instance;
}

Santa::Santa(ValueMap& properties) : super(properties,
	Santa::MapKey,
	EntityResources::Enemies_BallmerPeaks_Santa_Animations,
	EntityResources::Enemies_BallmerPeaks_Santa_Emblem,
	CSize(420.0f, 452.0f),
	0.9f)
{
}

Santa::~Santa()
{
}

Vec2 Santa::getDialogueOffset()
{
	return Vec2(-32.0f, -248.0f);
}

LocalizedString* Santa::getEntityName()
{
	return Strings::Platformer_Entities_Names_Enemies_BallmerPeaks_Santa::create();
}
