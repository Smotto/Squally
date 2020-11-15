#include "TutorialACardStaged.h"

#include <vector>

#include "cocos/2d/CCActionInstant.h"
#include "cocos/2d/CCActionInterval.h"
#include "cocos/base/CCDirector.h"

#include "Engine/Input/ClickableTextNode.h"
#include "Engine/Localization/LocalizedLabel.h"
#include "Engine/UI/Controls/HelpArrow.h"
#include "Engine/UI/HUD/FocusTakeOver.h"
#include "Scenes/Hexus/CardRow.h"
#include "Scenes/Hexus/GameState.h"
#include "Scenes/Hexus/HexusConfig.h"
#include "Scenes/Hexus/StateOverride.h"

#include "Resources/UIResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

TutorialACardStaged* TutorialACardStaged::create()
{
	TutorialACardStaged* instance = new TutorialACardStaged();

	instance->autorelease();

	return instance;
}

TutorialACardStaged::TutorialACardStaged() : super(GameState::StateType::SelectionStaged)
{
	this->binaryCardArrow = HelpArrow::create();
	this->decimalCardArrow = HelpArrow::create();
	this->hexCardArrow = HelpArrow::create();

	this->addChild(this->binaryCardArrow);
	this->addChild(this->decimalCardArrow);
	this->addChild(this->hexCardArrow);
}

TutorialACardStaged::~TutorialACardStaged()
{
}

void TutorialACardStaged::onEnter()
{
	super::onEnter();
}

void TutorialACardStaged::initializePositions()
{
	super::initializePositions();

	Size visibleSize = Director::getInstance()->getVisibleSize();

	this->binaryCardArrow->setPosition(Vec2(visibleSize.width / 2.0f + HexusConfig::centerColumnCenter, visibleSize.height / 2.0f - HexusConfig::binaryRowOffsetY + 212.0f));
	this->decimalCardArrow->setPosition(Vec2(visibleSize.width / 2.0f + HexusConfig::centerColumnCenter, visibleSize.height / 2.0f - HexusConfig::decimalRowOffsetY + 212.0f));
	this->hexCardArrow->setPosition(Vec2(visibleSize.width / 2.0f + HexusConfig::centerColumnCenter, visibleSize.height / 2.0f - HexusConfig::hexRowOffsetY + 212.0f));
}

void TutorialACardStaged::initializeListeners()
{
	super::initializeListeners();
}

bool TutorialACardStaged::tryHijackState(GameState* gameState)
{
	return false;
}

void TutorialACardStaged::unHijackState(GameState* gameState)
{
}

void TutorialACardStaged::onBeforeStateChange(GameState* gameState)
{
	super::onBeforeStateChange(gameState);
}

void TutorialACardStaged::onAnyStateChange(GameState* gameState)
{
	super::onAnyStateChange(gameState);

	this->binaryCardArrow->hidePointer();
	this->decimalCardArrow->hidePointer();
	this->hexCardArrow->hidePointer();

	if (gameState->selectedHandCard == nullptr || gameState->selectedHandCard->cardData == nullptr || gameState->stateType != GameState::StateType::SelectionStaged)
	{
		return;
	}

	switch(gameState->selectedHandCard->cardData->getCardType())
	{
		case CardData::CardType::Binary:
		{
			this->binaryCardArrow->showPointer();
			break;
		}
		case CardData::CardType::Decimal:
		{
			this->decimalCardArrow->showPointer();
			break;
		}
		case CardData::CardType::Hexidecimal:
		{
			this->hexCardArrow->showPointer();
			break;
		}
		default:
		{
			break;
		}
	}
}
