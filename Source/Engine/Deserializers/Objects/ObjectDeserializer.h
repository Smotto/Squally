#pragma once

#include "cocos/base/CCValue.h"

#include "Engine/SmartNode.h"

class GameObject;
class PropertyDeserializer;

class ObjectDeserializer : public SmartNode
{
public:
	struct ObjectDeserializationArgs
	{
		GameObject* gameObject = nullptr;

		ObjectDeserializationArgs(GameObject* gameObject) : gameObject(gameObject)
		{
		}
	};

	struct ObjectDeserializationRequestArgs
	{
		cocos2d::ValueMap properties;
		std::function<void(ObjectDeserializationArgs)> onDeserializeCallback = nullptr;

		ObjectDeserializationRequestArgs(
			cocos2d::ValueMap properties,
			std::function<void(ObjectDeserializationArgs)> onDeserializeCallback):
			properties(properties), onDeserializeCallback(onDeserializeCallback)
		{
		}
	};

	void deserialize(ObjectDeserializer::ObjectDeserializationRequestArgs args);
	virtual void deserialize(ObjectDeserializer::ObjectDeserializationRequestArgs* args);
	std::string getObjectType();

protected:
	ObjectDeserializer(std::string objectType, std::vector<PropertyDeserializer*> propertyDeserializers = { });
	virtual ~ObjectDeserializer();

	void deserializeProperties(GameObject* object, const cocos2d::ValueMap& properties);

	std::map<std::string, std::function<GameObject*(cocos2d::ValueMap)>> deserializers;
	std::vector<PropertyDeserializer*> propertyDeserializers;

private:
	typedef SmartNode super;

	std::string objectType;
};
