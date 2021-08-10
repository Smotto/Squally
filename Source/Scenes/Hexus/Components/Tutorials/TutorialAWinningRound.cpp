#include "TutorialAWinningRound.h"

#include <vector>

#include "cocos/2d/CCActionInterval.h"
#include "cocos/base/CCDirector.h"

#include "Engine/Input/ClickableTextNode.h"
#include "Engine/Localization/LocalizedLabel.h"
#include "Engine/UI/Controls/HelpArrow.h"
#include "Engine/UI/HUD/FocusTakeOver.h"
#include "Scenes/Hexus/HexusConfig.h"
#include "Scenes/Hexus/StateOverride.h"

#include "Resources/UIResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

TutorialAWinningRound* TutorialAWinningRound::create()
{
	TutorialAWinningRound* instance = new TutorialAWinningRound();

	instance->autorelease();

	return instance;
}

TutorialAWinningRound::TutorialAWinningRound() : super(GameState::StateType::TurnEnd)
{
	this->focusTakeOver = FocusTakeOver::create();
	this->scoreTotalsTutorialLabel = LocalizedLabel::create(LocalizedLabel::FontStyle::Main, LocalizedLabel::FontSize::P, Strings::Hexus_Tutorials_A_WinningRound::create(), CSize(420.0f, 0.0f));
	this->helpArrowScoreTotals = HelpArrow::create();
	this->scoreTotalsNextButton = this->createNextButton();
	
	this->scoreTotalsTutorialLabel->enableOutline(Color4B::BLACK, 2);
	this->scoreTotalsTutorialLabel->setAnchorPoint(Vec2(0.5f, 0.0f));
	this->helpArrowScoreTotals->setRotation(-90.0f);

	this->addChild(this->focusTakeOver);
	this->addChild(this->scoreTotalsTutorialLabel);
	this->addChild(this->scoreTotalsNextButton);
	this->addChild(this->helpArrowScoreTotals);
}

TutorialAWinningRound::~TutorialAWinningRound()
{
}

void TutorialAWinningRound::onEnter()
{
	super::onEnter();

	this->scoreTotalsTutorialLabel->setOpacity(0);

	this->scoreTotalsNextButton->disableInteraction(0);
}

void TutorialAWinningRound::initializePositions()
{
	super::initializePositions();

	CSize visibleSize = Director::getInstance()->getVisibleSize();

	this->scoreTotalsTutorialLabel->setPosition(visibleSize.width / 2.0f - 256.0f, visibleSize.height / 2.0f + 32.0f);
	this->scoreTotalsNextButton->setPosition(visibleSize.width / 2.0f - 256.0f, visibleSize.height / 2.0f - 32.0f);
	this->helpArrowScoreTotals->setPosition(Vec2(visibleSize.width / 2.0f + HexusConfig::leftColumnCenter + HexusConfig::totalAttackOffsetX - 144.0f, visibleSize.height / 2.0f - HexusConfig::totalAttackOffsetY));
}

void TutorialAWinningRound::initializeListeners()
{
	super::initializeListeners();
}

bool TutorialAWinningRound::tryHijackState(GameState* gameState)
{
	if (gameState->getPlayerTotal() > gameState->getEnemyTotal())
	{
		this->initializeCallbacks(gameState);
		this->runTutorialScoreTotal(gameState);

		return true;
	}

	return false;
}

void TutorialAWinningRound::onBeforeStateChange(GameState* gameState)
{
	super::onBeforeStateChange(gameState);
}

void TutorialAWinningRound::onAnyStateChange(GameState* gameState)
{
	super::onAnyStateChange(gameState);
}

void TutorialAWinningRound::initializeCallbacks(GameState* gameState)
{
	this->scoreTotalsNextButton->setMouseClickCallback([=](InputEvents::MouseEventArgs* args)
	{
		this->tryUnHijackState(gameState);
	});

	this->whenKeyPressed({ InputEvents::KeyCode::KEY_SPACE }, [=](InputEvents::KeyboardEventArgs* args)
	{
		this->scoreTotalsNextButton->interact();
	});
}

void TutorialAWinningRound::runTutorialScoreTotal(GameState* gameState)
{
	this->scoreTotalsNextButton->enableInteraction(0);
	this->scoreTotalsNextButton->runAction(FadeTo::create(0.25f, 255));
	this->scoreTotalsTutorialLabel->runAction(FadeTo::create(0.25f, 255));
	this->helpArrowScoreTotals->showPointer();

	std::vector<Node*> focusTargets = std::vector<Node*>();
	focusTargets.push_back(gameState->scoreTotalPointer);
	this->focusTakeOver->focus(focusTargets);
}

void TutorialAWinningRound::unHijackState(GameState* gameState)
{
	this->scoreTotalsNextButton->disableInteraction();
	this->scoreTotalsNextButton->runAction(FadeTo::create(0.25f, 0));
	this->scoreTotalsTutorialLabel->runAction(FadeTo::create(0.25f, 0));
	this->helpArrowScoreTotals->hidePointer();
	this->focusTakeOver->unfocus();
}
