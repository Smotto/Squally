#pragma once

#include "Engine/Quests/QuestTask.h"

class Guano;
class PrincessDawn;
class QuestLine;
class Scrappy;
class Squally;

class RestorePower : public QuestTask
{
public:
	static RestorePower* create(GameObject* owner, QuestLine* questLine);

	static const std::string MapKeyQuest;

protected:
	RestorePower(GameObject* owner, QuestLine* questLine);
	virtual ~RestorePower();

	void onLoad(QuestState questState) override;
	void onActivate(bool isActiveThroughSkippable) override;
	void onComplete() override;
	void onSkipped() override;

private:
	typedef QuestTask super;

	void runCinematicSequencePt1();

	Guano* guano = nullptr;
	Scrappy* scrappy = nullptr;
	Squally* squally = nullptr;
	PrincessDawn* princessDawn = nullptr;
};