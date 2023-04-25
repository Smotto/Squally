#pragma once

#include "Engine/Quests/QuestTask.h"

class Sarude;
class Scrappy;
class QuestLine;
class Squally;

class TalkToMages : public QuestTask
{
public:
	static TalkToMages* create(GameObject* owner, QuestLine* questLine);

	static const std::string MapKeyQuest;

protected:
	TalkToMages(GameObject* owner, QuestLine* questLine);
	virtual ~TalkToMages();

	void onLoad(QuestState questState) override;
	void onActivate(bool isActiveThroughSkippable, bool isInitialActivation) override;
	void onComplete() override;
	void onSkipped() override;

private:
	typedef QuestTask super;

	void runCinematicSequencePart1();
	void runCinematicSequencePart2();
	void runCinematicSequencePart3();
	void runCinematicSequencePart4();
	void setPostText();

	Sarude* sarude = nullptr;
	Scrappy* scrappy = nullptr;
	Squally* squally = nullptr;
};
