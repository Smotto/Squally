#include "IsometricSqually.h"

#include "Engine/Input/Input.h"

#include "Resources/IsometricEntityResources.h"

using namespace cocos2d;

const std::string IsometricSqually::MapKey = "squally";

IsometricSqually* IsometricSqually::deserialize(ValueMap& properties)
{
	return IsometricSqually::create(properties);
}

IsometricSqually* IsometricSqually::create(cocos2d::ValueMap& properties)
{
	IsometricSqually* instance = new IsometricSqually(properties);

	instance->autorelease();

	return instance;
}

IsometricSqually::IsometricSqually(ValueMap& properties) : super::IsometricEntity(properties,
	IsometricEntityResources::Squally_Animations,
	1.0f,
	Vec2(-128.0f, 0.0f))
{
}

IsometricSqually::~IsometricSqually()
{
}

void IsometricSqually::onEnter()
{
	super::onEnter();
}

Vec2 IsometricSqually::getButtonOffset()
{
	return Vec2(0, 72.0f);
}

void IsometricSqually::update(float dt)
{
	super::update(dt);

	this->movement.x = 0.0f;
	this->movement.y = 0.0f;

	if (Input::IsPressed(InputEvents::KeyCode::KEY_LEFT_ARROW) || Input::IsPressed(InputEvents::KeyCode::KEY_A))
	{
		this->movement.x -= 1.0f;
	}

	if (Input::IsPressed(InputEvents::KeyCode::KEY_RIGHT_ARROW) || Input::IsPressed(InputEvents::KeyCode::KEY_D))
	{
		this->movement.x += 1.0f;
	}

	if (Input::IsPressed(InputEvents::KeyCode::KEY_UP_ARROW) || Input::IsPressed(InputEvents::KeyCode::KEY_W) || Input::IsPressed(InputEvents::KeyCode::KEY_SPACE))
	{
		this->movement.y += 1.0f;
	}

	if (Input::IsPressed(InputEvents::KeyCode::KEY_DOWN_ARROW) || Input::IsPressed(InputEvents::KeyCode::KEY_S))
	{
		this->movement.y -= 1.0f;
	}
}
