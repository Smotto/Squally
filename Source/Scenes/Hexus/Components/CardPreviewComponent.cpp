#include "CardPreviewComponent.h"

#include "cocos/base/CCDirector.h"

#include "Engine/Localization/ConstantString.h"
#include "Engine/Localization/LocalizedLabel.h"
#include "Engine/Utils/HackUtils.h"
#include "Events/HexusEvents.h"
#include "Scenes/Hexus/CardData/CardData.h"
#include "Scenes/Hexus/CardPreview.h"
#include "Scenes/Hexus/CardRow.h"
#include "Scenes/Hexus/Deck.h"
#include "Scenes/Hexus/GameState.h"
#include "Scenes/Hexus/HexusConfig.h"

#include "Strings/Strings.h"

using namespace cocos2d;

CardPreviewComponent* CardPreviewComponent::create()
{
	CardPreviewComponent* instance = new CardPreviewComponent();

	instance->autorelease();

	return instance;
}

CardPreviewComponent::CardPreviewComponent()
{
	this->cardPreview = CardPreview::create();

	this->addChild(this->cardPreview);
}

CardPreviewComponent::~CardPreviewComponent()
{
}

void CardPreviewComponent::initializePositions()
{
	super::initializePositions();

	CSize visibleSize = Director::getInstance()->getVisibleSize();

	this->cardPreview->setPosition(visibleSize.width / 2.0f + HexusConfig::rightColumnCenter, visibleSize.height / 2.0f + HexusConfig::previewOffsetY);
}

CardPreview* CardPreviewComponent::getCardPreview()
{
	return this->cardPreview;
}

void CardPreviewComponent::setHelpClickCallback(std::function<void(CardData*)> onHelpClick)
{
	this->cardPreview->setHelpClickCallback(onHelpClick);
}

void CardPreviewComponent::onBeforeStateChange(GameState* gameState)
{
	super::onBeforeStateChange(gameState);
}

void CardPreviewComponent::onAnyStateChange(GameState* gameState)
{
	super::onAnyStateChange(gameState);

	switch (gameState->stateType)
	{
		case GameState::StateType::CoinFlip:
		case GameState::StateType::CardReplace:
		case GameState::StateType::Neutral:
		case GameState::StateType::PeekCards:
		{
			this->initializeCallbacks(gameState);
			break;
		}
		case GameState::StateType::SelectionStaged:
		case GameState::StateType::CombineStaged:
		{
			if (gameState->selectedHandCard == nullptr)
			{
				this->initializeCallbacks(gameState);
				break;
			}

			switch (gameState->selectedHandCard->cardData->getCardType())
			{
				case CardData::CardType::Binary:
				case CardData::CardType::Decimal:
				case CardData::CardType::Hexidecimal:
				case CardData::CardType::Special_SHL:
				case CardData::CardType::Special_SHR:
				case CardData::CardType::Special_GREED:
				case CardData::CardType::Special_BONUS_MOVES:
				case CardData::CardType::Special_SUDDEN_DEATH:
				{
					// Do not enable intraction for rows when these card types are staged
					break;
				}
				default:
				{
					this->initializeCallbacks(gameState);
					break;
				}
			}

			break;
		}
		default:
		{
			this->cardPreview->clearPreview();
			break;
		}
	}
}

void CardPreviewComponent::initializeCallbacks(GameState* gameState)
{
	gameState->playerGraveyard->enableTopCardInteraction(CC_CALLBACK_1(CardPreviewComponent::onPreviewCard, this));
	gameState->enemyGraveyard->enableTopCardInteraction(CC_CALLBACK_1(CardPreviewComponent::onPreviewCard, this));

	gameState->playerHand->enableRowCardInteraction();
	gameState->enemyHand->enableRowCardInteraction();

	gameState->playerHand->setMouseOverCallback(CC_CALLBACK_1(CardPreviewComponent::onPreviewCardBroadcast, this));
	gameState->enemyHand->setMouseOverCallback(CC_CALLBACK_1(CardPreviewComponent::onPreviewCardBroadcast, this));
	gameState->playerHand->setMouseOutCallback(CC_CALLBACK_1(CardPreviewComponent::onCardMouseOut, this));
	gameState->enemyHand->setMouseOutCallback(CC_CALLBACK_1(CardPreviewComponent::onCardMouseOut, this));

	gameState->playerBinaryCards->enableRowCardInteraction();
	gameState->playerDecimalCards->enableRowCardInteraction();
	gameState->playerHexCards->enableRowCardInteraction();

	gameState->playerBinaryCards->setMouseOverCallback(CC_CALLBACK_1(CardPreviewComponent::onPreviewCardBroadcast, this));
	gameState->playerDecimalCards->setMouseOverCallback(CC_CALLBACK_1(CardPreviewComponent::onPreviewCardBroadcast, this));
	gameState->playerHexCards->setMouseOverCallback(CC_CALLBACK_1(CardPreviewComponent::onPreviewCardBroadcast, this));
	gameState->playerBinaryCards->setMouseOutCallback(CC_CALLBACK_1(CardPreviewComponent::onCardMouseOut, this));
	gameState->playerDecimalCards->setMouseOutCallback(CC_CALLBACK_1(CardPreviewComponent::onCardMouseOut, this));
	gameState->playerHexCards->setMouseOutCallback(CC_CALLBACK_1(CardPreviewComponent::onCardMouseOut, this));

	gameState->enemyBinaryCards->enableRowCardInteraction();
	gameState->enemyDecimalCards->enableRowCardInteraction();
	gameState->enemyHexCards->enableRowCardInteraction();

	gameState->enemyBinaryCards->setMouseOverCallback(CC_CALLBACK_1(CardPreviewComponent::onPreviewCardBroadcast, this));
	gameState->enemyDecimalCards->setMouseOverCallback(CC_CALLBACK_1(CardPreviewComponent::onPreviewCardBroadcast, this));
	gameState->enemyHexCards->setMouseOverCallback(CC_CALLBACK_1(CardPreviewComponent::onPreviewCardBroadcast, this));
	gameState->enemyBinaryCards->setMouseOutCallback(CC_CALLBACK_1(CardPreviewComponent::onCardMouseOut, this));
	gameState->enemyDecimalCards->setMouseOutCallback(CC_CALLBACK_1(CardPreviewComponent::onCardMouseOut, this));
	gameState->enemyHexCards->setMouseOutCallback(CC_CALLBACK_1(CardPreviewComponent::onCardMouseOut, this));
}

void CardPreviewComponent::onPreviewCard(Card* card)
{
	this->cardPreview->previewCard(card);

	HexusEvents::TriggerCardPreviewed(HexusEvents::CardPreviewArgs(card));
}

void CardPreviewComponent::onPreviewCardBroadcast(Card* card)
{
	this->cardPreview->previewCard(card);

	HexusEvents::TriggerCardPreviewed(HexusEvents::CardPreviewArgs(card));
}

void CardPreviewComponent::onCardMouseOut(Card* card)
{
	HexusEvents::TriggerCardMousedOut();
}
