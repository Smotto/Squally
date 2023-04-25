#include "UnlockTomb.h"

#include "Engine/Events/ObjectEvents.h"
#include "Objects/Platformer/Interactables/Doors/Dragon/DragonDoor.h"
#include "Objects/Platformer/Interactables/Puzzles/Brazier.h"
#include "Objects/Platformer/PlatformerDecorObject.h"
#include "Scenes/Platformer/Save/SaveKeys.h"

#include "Strings/Strings.h"

using namespace cocos2d;

const std::string UnlockTomb::MapKeyQuest = "unlock-tomb";
const std::string UnlockTomb::TagTombDoor = "tomb-door";
const std::string UnlockTomb::TagTorchLeft = "torch-left";
const std::string UnlockTomb::TagTorchRight = "torch-right";

UnlockTomb* UnlockTomb::create(GameObject* owner, QuestLine* questLine)
{
	UnlockTomb* instance = new UnlockTomb(owner, questLine);

	instance->autorelease();

	return instance;
}

UnlockTomb::UnlockTomb(GameObject* owner, QuestLine* questLine) : super(owner, questLine, UnlockTomb::MapKeyQuest, false)
{
}

UnlockTomb::~UnlockTomb()
{
}

void UnlockTomb::onLoad(QuestState questState)
{
	if (questState != QuestState::Complete)
	{
		ObjectEvents::WatchForObject<DragonDoor>(this, [=](DragonDoor* portal)
		{
			ObjectEvents::WatchForObject<Brazier>(this, [=](Brazier* logicTorchLeft)
			{
				ObjectEvents::WatchForObject<Brazier>(this, [=](Brazier* logicTorchRight)
				{
					if (!logicTorchLeft->isOn() || !logicTorchRight->isOn())
					{
						portal->lock(false);
					}
					else
					{
						portal->unlock(false);
					}
				}, UnlockTomb::TagTorchRight);
			}, UnlockTomb::TagTorchLeft);
		}, UnlockTomb::TagTombDoor);
	}
}

void UnlockTomb::onActivate(bool isActiveThroughSkippable, bool isInitialActivation)
{
}

void UnlockTomb::onComplete()
{
}

void UnlockTomb::onSkipped()
{
}
