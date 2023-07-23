#include "Zombie.h"

#include "cocos/math/CCGeometry.h"

#include "Resources/EntityResources.h"
#include "Resources/UIResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

const std::string Zombie::MapKey = "zombie";

Zombie* Zombie::deserialize(ValueMap& properties)
{
	Zombie* instance = new Zombie(properties);

	instance->autorelease();

	return instance;
}

Zombie::Zombie(ValueMap& properties) : super(properties,
	Zombie::MapKey,
	EntityResources::Enemies_LambdaCrypts_Zombie_Animations,
	EntityResources::Enemies_LambdaCrypts_Zombie_Emblem,
	CSize(146.0f, 248.0f),
	0.9f)
{
}

Zombie::~Zombie()
{
}

Vec2 Zombie::getDialogueOffset()
{
	return Vec2(0.0f, 0.0f);
}

LocalizedString* Zombie::getEntityName()
{
	return Strings::Platformer_Entities_Names_Enemies_LambdaCrypts_Zombie::create();
}
