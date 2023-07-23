#include "OrcWarrior.h"

#include "cocos/math/CCGeometry.h"

#include "Resources/EntityResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

const std::string OrcWarrior::MapKey = "orc-warrior";

OrcWarrior* OrcWarrior::deserialize(ValueMap& properties)
{
	OrcWarrior* instance = new OrcWarrior(properties);

	instance->autorelease();

	return instance;
}

OrcWarrior::OrcWarrior(ValueMap& properties) : super(properties,
	OrcWarrior::MapKey,
	EntityResources::Enemies_EndianForest_OrcWarrior_Animations,
	EntityResources::Enemies_EndianForest_OrcWarrior_Emblem,
	CSize(256.0f, 228.0f),
	1.0f)
{
}

OrcWarrior::~OrcWarrior()
{
}

Vec2 OrcWarrior::getDialogueOffset()
{
	return Vec2(0.0f, 0.0f);
}

LocalizedString* OrcWarrior::getEntityName()
{
	return Strings::Platformer_Entities_Names_Enemies_EndianForest_OrcWarrior::create();
}
