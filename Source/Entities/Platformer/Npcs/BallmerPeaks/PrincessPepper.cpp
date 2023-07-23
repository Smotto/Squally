#include "PrincessPepper.h"

#include "cocos/math/CCGeometry.h"

#include "Resources/EntityResources.h"
#include "Resources/UIResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

const std::string PrincessPepper::MapKey = "princess-pepper";

PrincessPepper* PrincessPepper::deserialize(ValueMap& properties)
{
	PrincessPepper* instance = new PrincessPepper(properties);

	instance->autorelease();

	return instance;
}

PrincessPepper::PrincessPepper(ValueMap& properties) : super(properties,
	PrincessPepper::MapKey,
	EntityResources::Npcs_BallmerPeaks_PrincessPepper_Animations,
	EntityResources::Npcs_BallmerPeaks_PrincessPepper_Emblem,
	CSize(112.0f, 160.0f),
	0.85f)
{
}

PrincessPepper::~PrincessPepper()
{
}

Vec2 PrincessPepper::getDialogueOffset()
{
	return Vec2(0.0f, 0.0f);
}

LocalizedString* PrincessPepper::getEntityName()
{
	return Strings::Platformer_Entities_Names_Npcs_BallmerPeaks_PrincessPepper::create();
}
