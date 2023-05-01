#include "GoblinWarriorPig.h"

#include "cocos/math/CCGeometry.h"

#include "Resources/EntityResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

const std::string GoblinWarriorPig::MapKey = "goblin-warrior-pig";

GoblinWarriorPig* GoblinWarriorPig::deserialize(ValueMap& properties)
{
	GoblinWarriorPig* instance = new GoblinWarriorPig(properties);

	instance->autorelease();

	return instance;
}

GoblinWarriorPig::GoblinWarriorPig(ValueMap& properties) : super(properties,
	GoblinWarriorPig::MapKey,
	EntityResources::Enemies_EndianForest_GoblinWarriorPig_Animations,
	EntityResources::Enemies_EndianForest_GoblinWarriorPig_Emblem,
	CSize(128.0f, 296.0f),
	0.85f)
{
}

GoblinWarriorPig::~GoblinWarriorPig()
{
}

Vec2 GoblinWarriorPig::getDialogueOffset()
{
	return Vec2(0.0f, -32.0f);
}

LocalizedString* GoblinWarriorPig::getEntityName()
{
	return Strings::Platformer_Entities_Names_Enemies_EndianForest_GoblinWarriorPig::create();
}
