#pragma once

#include "Engine/Deserializers/Properties/PropertyDeserializer.h"

class GameObject;
class QuestLine;
class QuestTask;

class QuestDeserializer : public PropertyDeserializer
{
public:
	struct QuestLineDeserializationRequestArgs
	{
		std::string questLine;

		QuestLineDeserializationRequestArgs(std::string questLine) : questLine(questLine)
		{
		}
	};
	
	struct QuestDeserializationRequestArgs
	{
		GameObject* owner = nullptr;
		std::string questLine;
		std::string questTask;

		QuestDeserializationRequestArgs(GameObject* owner, std::string questLine, std::string questTask) : owner(owner), questLine(questLine), questTask(questTask)
		{
		}
	};

	QuestLine* deserialize(QuestDeserializer::QuestLineDeserializationRequestArgs args);
	QuestTask* deserialize(QuestDeserializer::QuestDeserializationRequestArgs args);

protected:
	QuestDeserializer();
	virtual ~QuestDeserializer();

	std::map<std::string, std::function<QuestLine*()>> lineDeserializers;

private:
	typedef PropertyDeserializer super;
};
