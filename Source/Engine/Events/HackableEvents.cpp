#include "HackableEvents.h"

#include "cocos/base/CCDirector.h"
#include "cocos/base/CCEventDispatcher.h"

using namespace cocos2d;

const std::string HackableEvents::EventForceUseHackerMode = "EVENT_FORCE_USE_HACKERMODE";
const std::string HackableEvents::EventHackerModeToggle = "EVENT_HACKERMODE_TOGGLE";
const std::string HackableEvents::EventHackerModeEnable = "EVENT_HACKERMODE_ENABLE";
const std::string HackableEvents::EventHackerModeDisable = "EVENT_HACKERMODE_DISABLE";
const std::string HackableEvents::EventAllowHackerMode = "EVENT_ALLOW_HACKER_MODE";
const std::string HackableEvents::EventDisallowHackerMode = "EVENT_DISALLOW_HACKER_MODE";
const std::string HackableEvents::EventQueryHackerModeAllowed = "EVENT_QUERY_HACKER_MODE";
const std::string HackableEvents::EventHackableObjectOpen = "EVENT_OPEN_HACKABLE_OBJECT";
const std::string HackableEvents::EventHackableObjectClose = "EVENT_CLOSE_HACKABLE_OBJECT";
const std::string HackableEvents::EventHackableBaseEdit = "EVENT_EDIT_HACKABLE_OBJECT";
const std::string HackableEvents::EventHackableBaseEditDone = "EVENT_EDIT_HACKABLE_OBJECT_DONE";
const std::string HackableEvents::EventHackApplied = "EVENT_HACK_APPLIED";
const std::string HackableEvents::EventHackFlagsChanged = "EVENT_HACK_FLAGS_CHANGED";
const std::string HackableEvents::EventPauseHackTimers = "EVENT_PAUSE_HACK_TIMERS";
const std::string HackableEvents::EventResumeHackTimers = "EVENT_RESUME_HACK_TIMERS";
const std::string HackableEvents::EventOpenLexiconPage = "EVENT_OPEN_LEXICON_PAGE";
const std::string HackableEvents::EventCloseLeftLexiconPage = "EVENT_CLOSE_LEFT_LEXICON_PAGE";
const std::string HackableEvents::EventCloseRightLexiconPage = "EVENT_CLOSE_RIGHT_LEXICON_PAGE";

void HackableEvents::TriggerForceUseHackerMode()
{
	Director::getInstance()->getEventDispatcher()->dispatchEvent(
		HackableEvents::EventForceUseHackerMode
	);
}

void HackableEvents::TriggerHackerModeToggle(HackToggleArgs args)
{
	Director::getInstance()->getEventDispatcher()->dispatchEvent(
		HackableEvents::EventHackerModeToggle,
		&args
	);
}

void HackableEvents::TriggerHackerModeEnable(HackToggleArgs args)
{
	Director::getInstance()->getEventDispatcher()->dispatchEvent(
		HackableEvents::EventHackerModeEnable,
		&args
	);
}

void HackableEvents::TriggerHackerModeDisable()
{
	Director::getInstance()->getEventDispatcher()->dispatchEvent(
		HackableEvents::EventHackerModeDisable
	);
}

void HackableEvents::TriggerAllowHackerMode()
{
	Director::getInstance()->getEventDispatcher()->dispatchEvent(
		HackableEvents::EventAllowHackerMode
	);
}

void HackableEvents::TriggerDisallowHackerMode()
{
	Director::getInstance()->getEventDispatcher()->dispatchEvent(
		HackableEvents::EventDisallowHackerMode
	);
}

void HackableEvents::TriggerQueryHackerModeAllowed(HackerModeQueryArgs* args)
{
	Director::getInstance()->getEventDispatcher()->dispatchEvent(
		HackableEvents::EventQueryHackerModeAllowed,
		args
	);
}

void HackableEvents::TriggerOpenHackable(HackableObjectOpenArgs args)
{
	Director::getInstance()->getEventDispatcher()->dispatchEvent(
		HackableEvents::EventHackableObjectOpen,
		&args
	);
}

void HackableEvents::TriggerCloseHackable()
{
	Director::getInstance()->getEventDispatcher()->dispatchEvent(
		HackableEvents::EventHackableObjectClose
	);
}

void HackableEvents::TriggerEditHackableBase(HackableObjectEditArgs args)
{
	Director::getInstance()->getEventDispatcher()->dispatchEvent(
		HackableEvents::EventHackableBaseEdit,
		&args
	);
}

void HackableEvents::TriggerEditHackableBaseDone()
{
	Director::getInstance()->getEventDispatcher()->dispatchEvent(
		HackableEvents::EventHackableBaseEditDone
	);
}

void HackableEvents::TriggerOnHackApplied(HackAppliedArgs args)
{
	Director::getInstance()->getEventDispatcher()->dispatchEvent(
		HackableEvents::EventHackApplied,
		&args
	);
}

void HackableEvents::TriggerHackFlagsChanged(HackFlagsChangedArgs args)
{
	Director::getInstance()->getEventDispatcher()->dispatchEvent(
		HackableEvents::EventHackFlagsChanged,
		&args
	);
}

void HackableEvents::TriggerPauseHackTimers()
{
	Director::getInstance()->getEventDispatcher()->dispatchEvent(
		HackableEvents::EventPauseHackTimers
	);
}

void HackableEvents::TriggerResumeHackTimers()
{
	Director::getInstance()->getEventDispatcher()->dispatchEvent(
		HackableEvents::EventResumeHackTimers
	);
}

void HackableEvents::TriggerOpenLexiconPage(OpenLexiconPageArgs args)
{
	Director::getInstance()->getEventDispatcher()->dispatchEvent(
		HackableEvents::EventOpenLexiconPage,
		&args
	);
}

void HackableEvents::TriggerCloseLeftLexiconPage()
{
	Director::getInstance()->getEventDispatcher()->dispatchEvent(
		HackableEvents::EventCloseLeftLexiconPage
	);
}

void HackableEvents::TriggerCloseRightLexiconPage()
{
	Director::getInstance()->getEventDispatcher()->dispatchEvent(
		HackableEvents::EventCloseRightLexiconPage
	);
}
