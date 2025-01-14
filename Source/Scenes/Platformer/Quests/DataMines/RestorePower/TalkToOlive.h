#pragma once

#include "Engine/Quests/QuestTask.h"

class Guano;
class Olive;
class QuestLine;
class Scrappy;
class Squally;

class TalkToOlive : public QuestTask
{
public:
	static TalkToOlive* create(GameObject* owner, QuestLine* questLine);

	static const std::string MapKeyQuest;

protected:
	TalkToOlive(GameObject* owner, QuestLine* questLine);
	virtual ~TalkToOlive();

	void onLoad(QuestState questState) override;
	void onActivate(bool isActiveThroughSkippable, bool isInitialActivation) override;
	void onComplete() override;
	void onSkipped() override;

private:
	typedef QuestTask super;

	void runCinematicSequencePt1();
	void runCinematicSequencePt2();
	void runCinematicSequencePt3();
	void runCinematicSequencePt4();
	void runCinematicSequencePt5();
	void runCinematicSequencePt6();
	void runCinematicSequencePt7();
	void runCinematicSequencePt8();
	void runCinematicSequencePt9();
	void runCinematicSequencePt10();
	void runCinematicSequencePt11();

	Guano* guano = nullptr;
	Scrappy* scrappy = nullptr;
	Squally* squally = nullptr;
	Olive* olive = nullptr;

	static const std::string MapEventTalkToOlive;
};
