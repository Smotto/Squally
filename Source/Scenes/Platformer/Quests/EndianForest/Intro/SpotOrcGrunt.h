#pragma once

#include "Engine/Quests/QuestTask.h"

class Scrappy;
class QuestLine;

class SpotOrcGrunt : public QuestTask
{
public:
	static SpotOrcGrunt* create(GameObject* owner, QuestLine* questLine);

	static const std::string MapKeyQuest;

protected:
	SpotOrcGrunt(GameObject* owner, QuestLine* questLine);
	virtual ~SpotOrcGrunt();

	void onLoad(QuestState questState) override;
	void onActivate(bool isActiveThroughSkippable, bool isInitialActivation) override;
	void onComplete() override;
	void onSkipped() override;

private:
	typedef QuestTask super;

	void runCinematicSequence();

	Scrappy* scrappy = nullptr;
};
