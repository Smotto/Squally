#pragma once

#include "Engine/Quests/QuestTask.h"

class MagePortal;
class PlatformerEntity;
class QuestLine;
class Squally;

class BeatTutorialC : public QuestTask
{
public:
	static BeatTutorialC* create(GameObject* owner, QuestLine* questLine);

	static const std::string MapKeyQuest;

protected:
	BeatTutorialC(GameObject* owner, QuestLine* questLine);
	virtual ~BeatTutorialC();

	void onLoad(QuestState questState) override;
	void onActivate(bool isActiveThroughSkippable, bool isInitialActivation) override;
	void onComplete() override;
	void onSkipped() override;

private:
	typedef QuestTask super;

	MagePortal* portal = nullptr;
	PlatformerEntity* mage = nullptr;
	Squally* squally = nullptr;

	static const std::string QuestPortalTag;
	static const std::string WinLossTrackIdentifier;
};
