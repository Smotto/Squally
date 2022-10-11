 #pragma once

#include "Engine/Quests/QuestTask.h"

class Guano;
class QuestLine;
class Squally;

class RescueGuano : public QuestTask
{
public:
	static RescueGuano* create(GameObject* owner, QuestLine* questLine);

	static const std::string MapKeyQuest;

protected:
	RescueGuano(GameObject* owner, QuestLine* questLine);
	virtual ~RescueGuano();

	void onLoad(QuestState questState) override;
	void onActivate(bool isActiveThroughSkippable) override;
	void onComplete() override;
	void onSkipped() override;

private:
	typedef QuestTask super;

	void runRescueSequence(); 
	void runRescueSequencePt2();
	void runRescueSequencePt3();

	Guano* guano = nullptr;
	Squally* squally = nullptr;

	static const std::string EventMulDoorUnlocked;
	static const std::string TagPrisonDoor;
};
