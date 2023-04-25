#pragma once

#include "Engine/Quests/QuestTask.h"

class Blackbeard;
class Portal;
class QuestLine;
class Squally;

class SailForRuins : public QuestTask
{
public:
	static SailForRuins* create(GameObject* owner, QuestLine* questLine);

	static const std::string MapKeyQuest;

protected:
	SailForRuins(GameObject* owner, QuestLine* questLine);
	virtual ~SailForRuins();

	void onLoad(QuestState questState) override;
	void onActivate(bool isActiveThroughSkippable, bool isInitialActivation) override;
	void onComplete() override;
	void onSkipped() override;

private:
	typedef QuestTask super;

	void runNoSequence();
	void runYesSequence();

	Blackbeard* blackbeard = nullptr;
	Squally* squally = nullptr;
	Portal* portal = nullptr;

	static const std::string QuestTagShipPortal;
};
