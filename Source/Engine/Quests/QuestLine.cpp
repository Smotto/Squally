#include "QuestLine.h"

#include "cocos/base/CCValue.h"

#include "Engine/Events/QuestEvents.h"
#include "Engine/Quests/QuestTask.h"
#include "Engine/Quests/Quests.h"
#include "Engine/Save/SaveManager.h"
#include "Engine/Utils/StrUtils.h"

using namespace cocos2d;

const std::string QuestLine::QuestLineSaveKeyComplete = "COMPLETE_";

QuestLine::QuestLine(std::string questLine, const std::vector<QuestData> quests, QuestLine* prereq, std::string prereqTask)
{
	this->questLine = questLine;
	this->quests = quests;
	this->prereq = prereq;
	this->prereqTask = prereqTask;

	if (this->prereq != nullptr)
	{
		this->addChild(this->prereq);
	}
}

QuestLine::~QuestLine()
{
}

QuestTask* QuestLine::deserialize(GameObject* owner, std::string questTask)
{
	for (auto next : this->quests)
	{
		if (next.questTask == questTask)
		{
			return next.deserializer(owner, this);
		}
	}
	
	return nullptr;
}

const std::vector<QuestLine::QuestMeta> QuestLine::getQuests()
{
	std::vector<QuestMeta> questData = std::vector<QuestMeta>();
	std::string currentQuestTask = Quests::getCurrentQuestTaskForLine(this->questLine);
	bool hasEncounteredActive = false;
	bool activeThroughSkippable = false;
	bool prereqComplete = this->prereq == nullptr ? true : this->prereq->isCompleteUpTo(this->prereqTask);

	if (currentQuestTask.empty() && !this->quests.empty())
	{
		currentQuestTask = this->quests.front().questTask;
	}
	else if (StrUtils::startsWith(currentQuestTask, QuestLine::QuestLineSaveKeyComplete, false) && !this->quests.empty())
	{
		// Quest chain is (probably) completed. This may not be the case if we find that a new quest was added in a patch. Check for this.
		std::string lastKnownCompletedQuest = StrUtils::ltrim(currentQuestTask, QuestLine::QuestLineSaveKeyComplete, false);

		// If the last known completed quest doesn't match the end of the chain, a new quest was added
		if (lastKnownCompletedQuest != this->quests.back().questTask)
		{
			for (auto it = this->quests.begin(); it != this->quests.end(); it++)
			{
				if ((*it).questTask == lastKnownCompletedQuest)
				{
					if (++it != this->quests.end())
					{
						// Set the current quest to the newly added next quest in the chain
						currentQuestTask = (*it).questTask;
					}

					break;
				}
			}
		}
	}
	
	for (auto next : this->quests)
	{
		bool isActive = prereqComplete && (activeThroughSkippable || next.questTask == currentQuestTask);
		bool isComplete = prereqComplete && !isActive && !hasEncounteredActive;
		bool isSkippable = next.isSkippable;

		questData.push_back(QuestMeta(next.questTask, isActive, isSkippable, isComplete));

		activeThroughSkippable = (activeThroughSkippable || isActive) && isSkippable;
		hasEncounteredActive |= isActive;
	}

	return questData;
}

std::string QuestLine::getQuestLine()
{
	return this->questLine;
}

LocalizedString* QuestLine::getQuestLineName()
{
	return nullptr;
}

LocalizedString* QuestLine::getQuestLineObjective(std::string questTask)
{
	return nullptr;
}

bool QuestLine::isComplete()
{
	const std::vector<QuestMeta> questMeta = this->getQuests();

	if (questMeta.empty())
	{
		return true;
	}

	return questMeta.back().isComplete;
}

bool QuestLine::isCompleteUpTo(std::string questTask)
{
	if (questTask.empty())
	{
		return this->isComplete();
	}

	const std::vector<QuestMeta> questMeta = this->getQuests();

	for (auto next : questMeta)
	{
		if (next.isComplete && next.questTask == questTask)
		{
			return true;
		}
	}

	return false;
}

void QuestLine::advanceNextQuest(QuestTask* currentQuest)
{
	if (currentQuest == nullptr)
	{
		return;
	}

	std::vector<QuestLine::QuestMeta> quests = this->getQuests();

	for (auto it = quests.begin(); it != quests.end(); it++)
	{
		std::string questTaskName = (*it).questTask;

		if (questTaskName == currentQuest->getQuestTaskName())
		{
			if (++it != quests.end())
			{
				// Progress to next task
				Quests::saveQuestLineProgress(this->questLine, (*it).questTask);
			}
			else
			{
				// Quest line complete
				Quests::saveQuestLineProgress(this->questLine, QuestLine::QuestLineSaveKeyComplete + questTaskName);
			}

			break;
		}
	}
	
	QuestEvents::TriggerQuestTaskComplete(QuestEvents::QuestTaskCompleteArgs(this->questLine, currentQuest));
}

void QuestLine::waiveQuestPrereq()
{
	if (this->prereq != nullptr)
	{
		// If there is no explicit prerequisite task, waive the entire prerequisite quest line
		if (this->prereqTask.empty())
		{
			std::vector<QuestLine::QuestMeta> quests = this->prereq->getQuests();

			if (!quests.empty() && !quests.back().isComplete)
			{
				Quests::saveQuestLineProgress(this->prereq->getQuestLine(), QuestLine::QuestLineSaveKeyComplete + quests.back().questTask);
			}
		}
		else
		{
			// TODO: If we care enough, we can waive up to the pre-requisite task. Probably not an issue.
		}
	}
}
