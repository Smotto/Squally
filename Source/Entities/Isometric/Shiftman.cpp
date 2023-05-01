#include "Shiftman.h"

#include "Engine/Animations/SmartAnimationNode.h"

#include "Resources/EntityResources.h"

using namespace cocos2d;

const std::string Shiftman::MapKey = "shiftman";

Shiftman* Shiftman::deserialize(ValueMap& properties)
{
	Shiftman* instance = new Shiftman(properties);

	instance->autorelease();

	return instance;
}

Shiftman::Shiftman(ValueMap& properties) : super::IsometricEntity(properties,
	EntityResources::Squally_Animations,
	1.0f)
{
	this->registerHackables();

	//// this->animationNode->playAnimation("Idle_SE", 0.5f);
}

Shiftman::~Shiftman()
{
}

void Shiftman::onEnter()
{
	super::onEnter();
}

Vec2 Shiftman::getButtonOffset()
{
	return Vec2(0, 72.0f);
}

void Shiftman::update(float dt)
{
	super::update(dt);

	this->movement.x = 0.0f;
	this->movement.y = 0.0f;
}
