#include "StateCoinFlip.h"

#include "cocos/2d/CCSprite.h"
#include "cocos/2d/CCActionInstant.h"
#include "cocos/2d/CCActionInterval.h"
#include "cocos/2d/CCActionEase.h"
#include "cocos/base/CCDirector.h"

#include "Engine/Animations/SmartAnimationSequenceNode.h"
#include "Scenes/Hexus/CardRow.h"
#include "Scenes/Hexus/GameState.h"
#include "Scenes/Hexus/HexusConfig.h"

#include "Resources/HexusResources.h"

using namespace cocos2d;

StateCoinFlip* StateCoinFlip::create()
{
	StateCoinFlip* instance = new StateCoinFlip();

	instance->autorelease();

	return instance;
}

StateCoinFlip::StateCoinFlip() : super(GameState::StateType::CoinFlip)
{
	this->coinAnimation = SmartAnimationSequenceNode::create(HexusResources::CoinFlip_0000);

	this->addChild(this->coinAnimation);
}

StateCoinFlip::~StateCoinFlip()
{
}

void StateCoinFlip::onEnter()
{
	super::onEnter();

	this->coinAnimation->setOpacity(0);
}

void StateCoinFlip::initializePositions()
{
	super::initializePositions();

	CSize visibleSize = Director::getInstance()->getVisibleSize();

	this->coinAnimation->setPosition(visibleSize.width / 2.0f + HexusConfig::centerColumnCenter, visibleSize.height / 2.0f);
}

void StateCoinFlip::onBeforeStateEnter(GameState* gameState)
{
	super::onBeforeStateEnter(gameState);
}

void StateCoinFlip::onStateEnter(GameState* gameState)
{
	super::onStateEnter(gameState);

	gameState->enemyBinaryCards->enableRowCardInteraction();
	gameState->enemyDecimalCards->enableRowCardInteraction();
	gameState->enemyHexCards->enableRowCardInteraction();
	gameState->playerBinaryCards->enableRowCardInteraction();
	gameState->playerDecimalCards->enableRowCardInteraction();
	gameState->playerHexCards->enableRowCardInteraction();

	// No coin flip after the first round
	if (gameState->roundNumber >= 1)
	{
		// Alternate turn instead
		gameState->turn = (gameState->turn == GameState::Turn::Player) ? GameState::Turn::Enemy : GameState::Turn::Player;
		
		switch (gameState->turn)
		{
			case GameState::Turn::Enemy:
			{
				this->runAction(Sequence::create(
					DelayTime::create(0.25f),
					CallFunc::create([=]()
					{
						GameState::updateState(gameState, GameState::StateType::OpponentTurnStart);
					}),
					nullptr
				));
				break;
			}
			default:
			case GameState::Turn::Player:
			{
				this->runAction(Sequence::create(
					DelayTime::create(0.25f),
					CallFunc::create([=]()
					{
						GameState::updateState(gameState, GameState::StateType::PlayerTurnStart);
					}),
					nullptr
				));
				break;
			}
		}

		return;
	}

	// Additional frames required for coin loop animation when the enemy wins
	static const std::vector<std::string> EnemyTurnAnimFrames =
	{
		HexusResources::CoinFlip_0000,
		HexusResources::CoinFlip_0001,
		HexusResources::CoinFlip_0002,
		HexusResources::CoinFlip_0003,
		HexusResources::CoinFlip_0004,
		HexusResources::CoinFlip_0005,
		HexusResources::CoinFlip_0006
	};

	float extraDuration = 0.0f;

	if (RandomHelper::random_real(0.0f, 1.0f) > 0.5f)
	{
		gameState->turn = GameState::Turn::Player;
	}
	else
	{
		gameState->turn = GameState::Turn::Enemy;
		extraDuration = float(EnemyTurnAnimFrames.size()) * HexusConfig::coinFlipSpeed;
	}
	
	// Some correction is needed to make the coin look like it is not still flipping after it lands
	const float timingCorrection = 0.5f;

	float loopDuration = float(SmartAnimationSequenceNode::GetAnimationLength(HexusResources::CoinFlip_0000)) * HexusConfig::coinFlipSpeed;
	float flipDuration = extraDuration + loopDuration * float(HexusConfig::coinFlipCount) + timingCorrection;
	this->coinAnimation->setScale(HexusConfig::coinFlipStartScale);
	this->coinAnimation->runAction(FadeTo::create(0.25f, 255));

	this->coinAnimation->runAction(Sequence::create(
		DelayTime::create(HexusConfig::coinFlipStartDelay),
		CallFunc::create([=]()
		{
			switch (gameState->turn)
			{
				case GameState::Turn::Player:
				{
					this->coinAnimation->playAnimationRepeat(HexusResources::CoinFlip_0000, HexusConfig::coinFlipSpeed, 0.0f, false, HexusConfig::coinFlipCount, [=]()
					{
						this->coinAnimation->playSingleFrame(HexusResources::CoinFlip_0000, 0, HexusConfig::coinFlipSpeed);
					});

					break;
				}
				case GameState::Turn::Enemy:
				{
					this->coinAnimation->playAnimationRepeat(HexusResources::CoinFlip_0000, HexusConfig::coinFlipSpeed, 0.0f, false, HexusConfig::coinFlipCount, [=]()
					{
						this->coinAnimation->playAnimation(EnemyTurnAnimFrames, HexusConfig::coinFlipSpeed);
					});
					break;
				}
			}
		}),
		EaseSineOut::create(ScaleTo::create(flipDuration / 2.0f, HexusConfig::coinFlipEndScale)),
		EaseSineOut::create(ScaleTo::create(flipDuration / 2.0f, HexusConfig::coinFlipStartScale)),
		CallFunc::create([=]()
		{
			switch (gameState->turn)
			{
				case GameState::Turn::Enemy:
				{
					GameState::updateState(gameState, GameState::StateType::OpponentTurnStart);
					break;
				}
				default:
				case GameState::Turn::Player:
				{
					GameState::updateState(gameState, GameState::StateType::PlayerTurnStart);
					break;
				}
			}
		}),
		DelayTime::create(HexusConfig::coinFlipRestDuration),
		FadeOut::create(HexusConfig::coinFlipFadeSpeed),
		nullptr
	));
}

void StateCoinFlip::onStateReload(GameState* gameState)
{
	super::onStateReload(gameState);
}

void StateCoinFlip::onStateExit(GameState* gameState)
{
	super::onStateExit(gameState);
}
