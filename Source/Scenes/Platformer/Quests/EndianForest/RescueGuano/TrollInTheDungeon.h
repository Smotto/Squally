#pragma once

#include "Engine/Quests/QuestTask.h"

class QuestLine;
class PlatformerEntity;
class Portal;
class Squally;

class TrollInTheDungeon : public QuestTask
{
public:
	static TrollInTheDungeon* create(GameObject* owner, QuestLine* questLine);

	static const std::string MapKeyQuest;

protected:
	TrollInTheDungeon(GameObject* owner, QuestLine* questLine);
	virtual ~TrollInTheDungeon();

	void onLoad(QuestState questState) override;
	void onActivate(bool isActiveThroughSkippable) override;
	void onComplete() override;
	void onSkipped() override;

private:
	typedef QuestTask super;

	void runChatSequence();
	void runChatSequencePt2();

	PlatformerEntity* mage = nullptr;
	Squally* squally = nullptr;
	Portal* exitPortal;
	
	static const std::string TagDialoguePause;
	static const std::string TagExit;
	static const std::string TagExitPortal;
};
