#pragma once

#include "Engine/Quests/QuestTask.h"

class Elriel;
class Portal;
class QuestLine;
class Scrappy;
class Squally;

class TalkToElriel : public QuestTask
{
public:
	static TalkToElriel* create(GameObject* owner, QuestLine* questLine);

	static const std::string MapKeyQuest;

protected:
	TalkToElriel(GameObject* owner, QuestLine* questLine);
	virtual ~TalkToElriel();

	void onLoad(QuestState questState) override;
	void onActivate(bool isActiveThroughSkippable) override;
	void onComplete() override;
	void onSkipped() override;

private:
	typedef QuestTask super;

	void runCinematicSequencePart1();
	void runCinematicSequencePart2();
	void runCinematicSequencePart3();
	void runCinematicSequencePart4();

	Elriel* elriel;
	Scrappy* scrappy = nullptr;
	Squally* squally = nullptr;
	Portal* cutscenePortal;

	static const std::string TagElrielExit;
	static const std::string TagCutscenePortal;
};
