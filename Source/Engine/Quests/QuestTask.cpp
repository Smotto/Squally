#include "QuestTask.h"

#include "cocos/base/CCEventCustom.h"
#include "cocos/base/CCEventListenerCustom.h"
#include "cocos/base/CCValue.h"

#include "Engine/Events/QuestEvents.h"
#include "Engine/Quests/QuestLine.h"
#include "Engine/Quests/Quests.h"
#include "Engine/Save/SaveManager.h"

using namespace cocos2d;

QuestTask::QuestTask(GameObject* owner, QuestLine* questLine, std::string questTask, bool skippable) : super()
{
	this->questState = QuestState::None;
	this->owner = owner;
	this->questLine = questLine;
	this->questTask = questTask;
	this->isSkippable = skippable;

	if (this->questLine != nullptr)
	{
		this->addTag(this->questLine->getQuestLine());
	}

	this->addTag(questTask);

	this->addChild(this->questLine);
}

QuestTask::~QuestTask()
{
}

void QuestTask::onEnterTransitionDidFinish()
{
	super::onEnterTransitionDidFinish();

	this->updateState();
}

void QuestTask::initializeListeners()
{
	super::initializeListeners();

	if (this->questLine != nullptr)
	{
		this->addEventListenerIgnorePause(EventListenerCustom::create(QuestEvents::EventQuestTaskComplete + this->questLine->getQuestLine(), [=](EventCustom* eventCustom)
		{
			QuestEvents::QuestTaskCompleteArgs* args = static_cast<QuestEvents::QuestTaskCompleteArgs*>(eventCustom->getData());
			
			if (args != nullptr && this->questLine != nullptr && args->questLine == this->questLine->getQuestLine())
			{
				this->updateState();
			}
		}));

		QuestLine* preReq = this->questLine->getPrereq();

		if (preReq != nullptr)
		{
			this->addEventListenerIgnorePause(EventListenerCustom::create(QuestEvents::EventQuestTaskComplete + preReq->getQuestLine(), [=](EventCustom* eventCustom)
			{
				QuestEvents::QuestTaskCompleteArgs* args = static_cast<QuestEvents::QuestTaskCompleteArgs*>(eventCustom->getData());
				
				if (args != nullptr && this->questLine != nullptr && args->questLine == preReq->getQuestLine())
				{
					this->updateState();
				}
			}));
		}
	}
}

std::string QuestTask::getQuestTaskName()
{
	return this->questTask;
}

QuestTask::QuestState QuestTask::getQuestState()
{
	return this->questState;
}

QuestTask::QuestState QuestTask::getQuestStateForTask(QuestLine* questLine, std::string questTask)
{
	const std::vector<QuestLine::QuestMeta> quests = questLine->getQuests();
	
	bool isPreviousSkippable = false;
	bool isPreviousComplete = false;
	QuestState questState = QuestState::None;

	for (auto next : quests)
	{
		if (next.questTask == questTask)
		{
			if (next.isComplete)
			{
				questState = QuestState::Complete;
			}
			else if (next.isActive && (isPreviousSkippable && !isPreviousComplete))
			{
				questState = QuestState::ActiveThroughSkippable;
			}
			else if (next.isActive)
			{
				questState = QuestState::Active;
			}
			
			break;
		}

		isPreviousSkippable = next.isSkippable;
		isPreviousComplete = next.isComplete;
	}

	return questState;
}

void QuestTask::updateState()
{
	QuestState previousState = this->questState;
	this->questState = QuestTask::getQuestStateForTask(this->questLine, this->questTask);

	if (!this->hasLoaded)
	{
		this->hasLoaded = true;
		this->onLoad(this->questState);
	}

	if (previousState == QuestState::None)
	{
		if (this->questState == QuestState::Active)
		{
			this->onActivate(false, false);
		}
		else if (this->questState == QuestState::ActiveThroughSkippable)
		{
			this->onActivate(true, false);
		}
	}
	// Trigger activation again if going from active through skippable to normally active
	else if (previousState == QuestState::ActiveThroughSkippable && this->questState == QuestState::Active)
	{
		this->onActivate(false, true);
	}

	// Check if this quest was skipped
	if (!this->completeCalled
		&& (previousState == QuestState::Active || previousState == QuestState::ActiveThroughSkippable)
		&& (this->questState == QuestState::None || this->questState == QuestState::Complete))
	{
		this->onSkipped();
	}
}

void QuestTask::waiveQuestPrereq()
{
	if (this->questLine != nullptr)
	{
		this->questLine->waiveQuestPrereq();
	}
}

bool QuestTask::isActive()
{
	return this->questState == QuestState::Active || this->questState == QuestState::ActiveThroughSkippable;
}

void QuestTask::complete()
{
	// Ideally we would also check to see if this quest is active, but we'll leave it up to the caller instead to
	// ensure complete() is never called in such a way to sequence break. Allowing sequence breaks makes debugging easier anyhow.
	if (this->completeCalled || this->questState == QuestState::Complete)
	{
		return;
	}

	this->completeCalled = true;
	this->questState = QuestState::Complete;

	this->onComplete();
	
	// Mostly for debugging purposes / a fail safe for patches. If the user completes a quest before its prereq, complete the prereq too.
	this->waiveQuestPrereq();

	this->questLine->advanceNextQuest(this);
}

void QuestTask::SaveQuestSaveState(std::string questLine, std::string questTask, std::string key, Value value)
{
	std::string combinedKey = questLine + "_" + questTask + "_" + key;

	SaveManager::SaveProfileData(combinedKey, value);
}

cocos2d::Value QuestTask::GetQuestSaveStateOrDefault(std::string questLine, std::string questTask, std::string key, cocos2d::Value value)
{
	std::string combinedKey = questLine + "_" + questTask + "_" + key;

	return SaveManager::GetProfileDataOrDefault(combinedKey, value);
}

void QuestTask::saveQuestSaveState(std::string key, Value value)
{
	QuestTask::SaveQuestSaveState(this->questLine->getQuestLine(), this->getQuestTaskName(), key, value);
}

cocos2d::Value QuestTask::getQuestSaveStateOrDefault(std::string key, cocos2d::Value value)
{
	return QuestTask::GetQuestSaveStateOrDefault(this->questLine->getQuestLine(), this->getQuestTaskName(), key, value);
}
