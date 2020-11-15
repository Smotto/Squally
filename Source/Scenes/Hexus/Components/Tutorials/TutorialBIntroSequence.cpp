#include "TutorialBIntroSequence.h"

#include <vector>

#include "cocos/2d/CCActionInterval.h"
#include "cocos/base/CCDirector.h"

#include "Engine/Input/ClickableTextNode.h"
#include "Engine/Localization/LocalizedLabel.h"
#include "Engine/UI/Controls/HelpArrow.h"
#include "Engine/UI/HUD/FocusTakeOver.h"
#include "Scenes/Hexus/CardRow.h"
#include "Scenes/Hexus/HexusConfig.h"
#include "Scenes/Hexus/StateOverride.h"

#include "Resources/UIResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

TutorialBIntroSequence* TutorialBIntroSequence::create()
{
	TutorialBIntroSequence* instance = new TutorialBIntroSequence();

	instance->autorelease();

	return instance;
}

TutorialBIntroSequence::TutorialBIntroSequence() : super(GameState::StateType::Neutral)
{
	this->focusTakeOver = FocusTakeOver::create();
	this->scoreTotalsTutorialLabel = LocalizedLabel::create(LocalizedLabel::FontStyle::Main, LocalizedLabel::FontSize::P, Strings::Hexus_Tutorials_B_ScoreLead::create(), Size(420.0f, 0.0f));
	this->handCardsTutorialLabel = LocalizedLabel::create(LocalizedLabel::FontStyle::Main, LocalizedLabel::FontSize::P, Strings::Hexus_Tutorials_B_SaveCards::create(), Size(640.0f, 0.0f), TextHAlignment::CENTER);
	this->lastStandTutorialLabel = LocalizedLabel::create(LocalizedLabel::FontStyle::Main, LocalizedLabel::FontSize::P, Strings::Hexus_Tutorials_B_UseLastStand::create(), Size(640.0f, 0.0f), TextHAlignment::CENTER);
	this->helpArrowScoreTotals = HelpArrow::create();
	this->helpArrowHandCards = HelpArrow::create();
	this->helpArrowLastStand = HelpArrow::create();
	this->scoreTotalsNextButton = this->createNextButton();
	this->handCardsNextButton = this->createNextButton();
	this->lastStandNextButton = this->createNextButton();
	
	this->scoreTotalsTutorialLabel->enableOutline(Color4B::BLACK, 2);
	this->handCardsTutorialLabel->enableOutline(Color4B::BLACK, 2);
	this->lastStandTutorialLabel->enableOutline(Color4B::BLACK, 2);

	this->scoreTotalsTutorialLabel->setAnchorPoint(Vec2(0.5f, 0.0f));
	this->handCardsTutorialLabel->setAnchorPoint(Vec2(0.5f, 0.0f));
	this->lastStandTutorialLabel->setAnchorPoint(Vec2(0.5f, 0.0f));

	this->helpArrowScoreTotals->setRotation(-90.0f);
	this->helpArrowHandCards->setRotation(0.0f);
	this->helpArrowLastStand->setRotation(0.0f);

	this->addChild(this->focusTakeOver);
	this->addChild(this->scoreTotalsTutorialLabel);
	this->addChild(this->handCardsTutorialLabel);
	this->addChild(this->lastStandTutorialLabel);

	this->addChild(this->scoreTotalsNextButton);
	this->addChild(this->handCardsNextButton);
	this->addChild(this->lastStandNextButton);

	this->addChild(this->helpArrowScoreTotals);
	this->addChild(this->helpArrowHandCards);
	this->addChild(this->helpArrowLastStand);
}

TutorialBIntroSequence::~TutorialBIntroSequence()
{
}

void TutorialBIntroSequence::onEnter()
{
	super::onEnter();

	this->scoreTotalsTutorialLabel->setOpacity(0);
	this->handCardsTutorialLabel->setOpacity(0);
	this->lastStandTutorialLabel->setOpacity(0);

	this->scoreTotalsNextButton->disableInteraction(0);
	this->handCardsNextButton->disableInteraction(0);
	this->lastStandNextButton->disableInteraction(0);
}

void TutorialBIntroSequence::initializePositions()
{
	super::initializePositions();

	Size visibleSize = Director::getInstance()->getVisibleSize();

	this->scoreTotalsTutorialLabel->setPosition(visibleSize.width / 2.0f - 256.0f, visibleSize.height / 2.0f + 32.0f);
	this->handCardsTutorialLabel->setPosition(visibleSize.width / 2.0f + HexusConfig::centerColumnCenter, visibleSize.height / 2.0f - 80.0f);
	this->lastStandTutorialLabel->setPosition(visibleSize.width / 2.0f + HexusConfig::leftColumnCenter + HexusConfig::passButtonOffsetX, visibleSize.height / 2.0f + 32.0f);
	
	this->scoreTotalsNextButton->setPosition(visibleSize.width / 2.0f - 256.0f, visibleSize.height / 2.0f - 32.0f);
	this->handCardsNextButton->setPosition(visibleSize.width / 2.0f + HexusConfig::centerColumnCenter, visibleSize.height / 2.0f - 144.0f);
	this->lastStandNextButton->setPosition(visibleSize.width / 2.0f + HexusConfig::leftColumnCenter + HexusConfig::passButtonOffsetX, visibleSize.height / 2.0f - 32.0f);
	
	this->helpArrowScoreTotals->setPosition(Vec2(visibleSize.width / 2.0f + HexusConfig::leftColumnCenter + HexusConfig::totalAttackOffsetX - 144.0f, visibleSize.height / 2.0f - HexusConfig::totalAttackOffsetY));
	this->helpArrowHandCards->setPosition(Vec2(visibleSize.width / 2.0f + HexusConfig::centerColumnCenter, visibleSize.height / 2.0f - 288.0f));
	this->helpArrowLastStand->setPosition(visibleSize.width / 2.0f + HexusConfig::leftColumnCenter + HexusConfig::passButtonOffsetX, visibleSize.height / 2.0f + HexusConfig::passButtonOffsetY + 144.0f);
}

void TutorialBIntroSequence::initializeListeners()
{
	super::initializeListeners();
}

bool TutorialBIntroSequence::tryHijackState(GameState* gameState)
{
	this->defer([=]()
	{
		this->initializeCallbacks(gameState);
		this->runTutorialScoreTotal(gameState);
	});

	return true;
}

void TutorialBIntroSequence::onBeforeStateChange(GameState* gameState)
{
	super::onBeforeStateChange(gameState);
}

void TutorialBIntroSequence::onAnyStateChange(GameState* gameState)
{
	super::onAnyStateChange(gameState);
}

void TutorialBIntroSequence::initializeCallbacks(GameState* gameState)
{
	this->scoreTotalsNextButton->setMouseClickCallback([=](InputEvents::MouseEventArgs* args)
	{
		this->runTutorialHandCards(gameState);
	});
	this->handCardsNextButton->setMouseClickCallback([=](InputEvents::MouseEventArgs* args)
	{
		this->runTutorialLastStand(gameState);
	});
	this->lastStandNextButton->setMouseClickCallback([=](InputEvents::MouseEventArgs* args)
	{
		this->tryUnHijackState(gameState);
	});

	this->whenKeyPressed({ EventKeyboard::KeyCode::KEY_SPACE }, [=](InputEvents::InputArgs* args)
	{
		this->scoreTotalsNextButton->interact();
		this->handCardsNextButton->interact();
		this->lastStandNextButton->interact();
	});
}

void TutorialBIntroSequence::runTutorialScoreTotal(GameState* gameState)
{
	this->scoreTotalsNextButton->enableInteraction(0);
	this->scoreTotalsNextButton->runAction(FadeTo::create(0.25f, 255));
	this->scoreTotalsTutorialLabel->runAction(FadeTo::create(0.25f, 255));
	this->helpArrowScoreTotals->showPointer();

	std::vector<Node*> focusTargets = std::vector<Node*>();
	focusTargets.push_back(gameState->scoreTotalPointer);
	this->focusTakeOver->focus(focusTargets);
}

void TutorialBIntroSequence::runTutorialHandCards(GameState* gameState)
{
	this->scoreTotalsNextButton->disableInteraction();
	this->scoreTotalsNextButton->runAction(FadeTo::create(0.25f, 0));
	this->scoreTotalsTutorialLabel->runAction(FadeTo::create(0.25f, 0));
	this->helpArrowScoreTotals->hidePointer();

	this->handCardsNextButton->enableInteraction(0);
	this->handCardsNextButton->runAction(FadeTo::create(0.25f, 255));
	this->handCardsTutorialLabel->runAction(FadeTo::create(0.25f, 255));
	this->helpArrowHandCards->showPointer();

	std::vector<Node*> focusTargets = std::vector<Node*>();
	focusTargets.push_back(gameState->playerHand);
	this->focusTakeOver->focus(focusTargets);
}

void TutorialBIntroSequence::runTutorialLastStand(GameState* gameState)
{
	this->handCardsNextButton->disableInteraction();
	this->handCardsNextButton->runAction(FadeTo::create(0.25f, 0));
	this->handCardsTutorialLabel->runAction(FadeTo::create(0.25f, 0));
	this->helpArrowHandCards->hidePointer();

	this->lastStandNextButton->enableInteraction(0);
	this->lastStandNextButton->runAction(FadeTo::create(0.25f, 255));
	this->lastStandTutorialLabel->runAction(FadeTo::create(0.25f, 255));
	this->helpArrowLastStand->showPointer();

	std::vector<Node*> focusTargets = std::vector<Node*>();
	focusTargets.push_back(gameState->lastStandButtonPointer);
	this->focusTakeOver->focus(focusTargets);
}

void TutorialBIntroSequence::unHijackState(GameState* gameState)
{
	this->lastStandNextButton->disableInteraction();
	this->lastStandNextButton->runAction(FadeTo::create(0.25f, 0));
	this->lastStandTutorialLabel->runAction(FadeTo::create(0.25f, 0));
	this->helpArrowLastStand->hidePointer();

	this->focusTakeOver->unfocus();
}
