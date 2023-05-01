#include "PenguinWarrior.h"

#include "cocos/math/CCGeometry.h"

#include "Resources/EntityResources.h"
#include "Resources/UIResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

const std::string PenguinWarrior::MapKey = "penguin-warrior";

PenguinWarrior* PenguinWarrior::deserialize(ValueMap& properties)
{
	PenguinWarrior* instance = new PenguinWarrior(properties);

	instance->autorelease();

	return instance;
}

PenguinWarrior::PenguinWarrior(ValueMap& properties) : super(properties,
	PenguinWarrior::MapKey,
	EntityResources::Enemies_BallmerPeaks_PenguinWarrior_Animations,
	EntityResources::Enemies_BallmerPeaks_PenguinWarrior_Emblem,
	CSize(212.0f, 296.0f),
	1.0f)
{
}

PenguinWarrior::~PenguinWarrior()
{
}

Vec2 PenguinWarrior::getDialogueOffset()
{
	return Vec2(-32.0f, -128.0f);
}

LocalizedString* PenguinWarrior::getEntityName()
{
	return Strings::Platformer_Entities_Names_Enemies_BallmerPeaks_PenguinWarrior::create();
}
