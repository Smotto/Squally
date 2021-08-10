#pragma once

#include "Engine/Quests/QuestTask.h"

class AnimationPart;
class InteractObject;
class Inventory;
class QuestLine;
class Ram;
class Scrappy;
class Squally;
class Sound;
class WorldSound;

class RepairRam : public QuestTask
{
public:
	static RepairRam* create(GameObject* owner, QuestLine* questLine);

	static const std::string MapKeyQuest;

protected:
	RepairRam(GameObject* owner, QuestLine* questLine);
	virtual ~RepairRam();

	void onLoad(QuestState questState) override;
	void onActivate(bool isActiveThroughSkippable) override;
	void onComplete() override;
	void onSkipped() override;

private:
	typedef QuestTask super;

	void runDialogue();
	void refreshWheels();
	void onRamInteract();
	void runCinematicSequence();

	Ram* ram;
	InteractObject* repairInteract;
	AnimationPart* wheel1;
	AnimationPart* wheel2;
	AnimationPart* wheel3;
	Sound* impactSound;
	WorldSound* rollSound;

	Scrappy* scrappy = nullptr;
	Squally* squally = nullptr;
	Inventory* inventory;

	bool wasActivated;

	static const std::string WheelFoundCount;
};
