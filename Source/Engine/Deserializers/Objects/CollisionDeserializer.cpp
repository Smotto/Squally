#include "CollisionDeserializer.h"

#include "Engine/Deserializers/Properties/PropertyDeserializer.h"
#include "Engine/Physics/CollisionObject.h"
#include "Engine/Utils/GameUtils.h"

using namespace cocos2d;

const std::string CollisionDeserializer::MapKeyTypeCollision = "collision";

CollisionDeserializer* CollisionDeserializer::create(std::vector<PropertyDeserializer*> propertyDeserializers)
{
	CollisionDeserializer* instance = new CollisionDeserializer(propertyDeserializers);

	instance->autorelease();

	return instance;
}

CollisionDeserializer::CollisionDeserializer(std::vector<PropertyDeserializer*> propertyDeserializers) : super(CollisionObject::MapKeyTypeCollision, propertyDeserializers)
{
}

CollisionDeserializer::~CollisionDeserializer()
{
}

void CollisionDeserializer::deserialize(ObjectDeserializer::ObjectDeserializationRequestArgs* args)
{
	ValueMap properties = args->properties;
	std::string name = GameUtils::getKeyOrDefault(args->properties, GameObject::MapKeyName, Value("")).asString();

	float width = GameUtils::getKeyOrDefault(args->properties, GameObject::MapKeyWidth, Value(1.0f)).asFloat();
	float height = GameUtils::getKeyOrDefault(args->properties, GameObject::MapKeyHeight, Value(1.0f)).asFloat();
	float x = GameUtils::getKeyOrDefault(args->properties, GameObject::MapKeyXPosition, Value(0.0f)).asFloat() + width / 2.0f;
	float y = GameUtils::getKeyOrDefault(args->properties, GameObject::MapKeyYPosition, Value(0.0f)).asFloat() + height / 2.0f;
	float friction = GameUtils::getKeyOrDefault(args->properties, CollisionObject::MapKeyFriction, Value(1.0f)).asFloat();
	std::vector<Vec2> shape = std::vector<Vec2>();

	if (GameUtils::keyExists(properties, GameObject::MapKeyPoints))
	{
		ValueVector polygonPoints = (properties.at(GameObject::MapKeyPoints).asValueVector());

		int index = 0;

		for (auto next : polygonPoints)
		{
			auto point = next.asValueMap();

			float deltaX = GameUtils::getKeyOrDefault(point, GameObject::MapKeyXPosition, Value(0.0f)).asFloat();
			float deltaY = GameUtils::getKeyOrDefault(point, GameObject::MapKeyYPosition, Value(0.0f)).asFloat();

			// Negate the Y since we're operating in a different coordinate system
			shape.push_back(Vec2(deltaX, -deltaY));
		}
	}
	else
	{
		shape = CollisionObject::createBox(CSize(width, height));
	}

	// Fire event, allowing for the game to map the deserialized collision string type (ie 'solid') to a unique integer identifier for CollisionType
	CollisionMappingEvents::requestCollisionMapKeyMapping(CollisionMappingEvents::CollisionMapRequestArgs(name, [&](CollisionType collisionType)
	{
		CollisionObject* collisionObject = CollisionObject::create(properties, shape, collisionType, CollisionObject::Properties(false, false));

		for (auto next : this->propertyDeserializers)
		{
			std::string key = GameUtils::getKeyOrDefault(properties, next->getPropertyDeserializerKey(), Value("")).asString();

			if (!key.empty())
			{
				next->deserializeProperties(collisionObject, properties);
			}
		}

		// Fire an event indicating successful deserialization
		args->onDeserializeCallback(ObjectDeserializer::ObjectDeserializationArgs(collisionObject));
	}));
}
