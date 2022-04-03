#pragma once

#include "Engine/Quests/QuestTask.h"

class QuestLine;
class Rhinoman;
class Sarcophagus;
class Squally;
class WorldSound;

class DefeatRhinoman : public QuestTask
{
public:
	static DefeatRhinoman* create(GameObject* owner, QuestLine* questLine);

	static const std::string MapKeyQuest;

protected:
	DefeatRhinoman(GameObject* owner, QuestLine* questLine);
	virtual ~DefeatRhinoman();

	void onLoad(QuestState questState) override;
	void onActivate(bool isActiveThroughSkippable) override;
	void onComplete() override;
	void onSkipped() override;

private:
	typedef QuestTask super;

	void runCinematicSequencePt1();
	void runCinematicSequencePt2();
	void runCinematicSequencePt3();

	Rhinoman* rhinoman = nullptr;
	Squally* squally = nullptr;
	Sarcophagus* sarcophagus = nullptr;
	WorldSound* rumbleSound = nullptr;

	static const std::string MapEventAwakenRhinoman;
	static const std::string TagExitPortal;
};