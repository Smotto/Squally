#pragma once

#include "Engine/SmartNode.h"

namespace cocos2d
{
	class LayerColor;
	class Sprite;
}

class ClickableNode;

class StateAIDecideCard;
class StateAIDecideCardReplace;
class StateAIDecidePass;
class StateAIDecideTarget;
class StateCardReplace;
class StateCoinFlip;
class StateSourceCardStaged;
class StateDraw;
class StateDrawInitial;
class StateGameEnd;
class StateGameExit;
class StateLoadInitialState;
class StateGameStart;
class StateNeutral;
class StateOpponentTurnStart;
class StatePlayCard;
class StatePass;
class StatePeekCards;
class StatePlayerTurnStart;
class StateRoundEnd;
class StateRoundStart;
class StateHandCardStaged;
class StateTurnEnd;
class StateTutorial;

class CardReplaceBanner;
class OpponentFirstBanner;
class OpponentLastStandBanner;
class OpponentPassBanner;
class OpponentRoundWinBanner;
class OpponentTurnBanner;
class PlayerFirstBanner;
class PlayerLastStandBanner;
class PlayerPassBanner;
class PlayerRoundWinBanner;
class PlayerTurnBanner;
class ChallengeBanner;
class RoundBanner;
class RoundTieBanner;
class VictoryBanner;
class DefeatBanner;
class DrawBanner;
class DeckCardCountDisplay;
class HandCardCountDisplay;
class RemainingCardDisplay;
class DrawCountDisplay;
class LossesDisplay;
class RowTotals;
class ScoreTotal;
class DebugDisplay;

class Avatars;
class CardPreviewComponent;
class Deck;
class AssemblyHelpText;
class StagingHelperText;
class GameState;
class HexusOpponentData;
class HelpMenu;
class Track;

class Hexus : public SmartNode
{
public:
	static Hexus* create();

	void open(HexusOpponentData* opponentData);

private:
	typedef SmartNode super;
	Hexus();
	virtual ~Hexus();

	void onEnter() override;
	void initializePositions() override;
	void initializeListeners() override;

	void buildEnemyDeck(HexusOpponentData* opponentData);
	void buildPlayerDeck();

	GameState* gameState;
	cocos2d::Sprite* gameBackground;

	// GameComponents
	Avatars* avatars;
	CardPreviewComponent* cardPreviewComponent;
	StagingHelperText* stagingHelperText;
	AssemblyHelpText* assemblyHelpText;

	StateAIDecideCard* stateAIDecideCard;
	StateAIDecideCardReplace* stateAIDecideCardReplace;
	StateAIDecidePass* stateAIDecidePass;
	StateAIDecideTarget* stateAIDecideTarget;
	StateCardReplace* stateCardReplace;
	StatePeekCards* statePeekCards;
	StateCoinFlip* stateCoinFlip;
	StateSourceCardStaged* stateSourceCardStaged;
	StateDraw* stateDraw;
	StateDrawInitial* stateDrawInitial;
	StateGameEnd* stateGameEnd;
	StateGameExit* stateGameExit;
	StateLoadInitialState* stateLoadInitialState;
	StateGameStart* stateGameStart;
	StateNeutral* stateNeutral;
	StateOpponentTurnStart* stateOpponentTurnStart;
	StatePlayCard* statePlayCard;
	StatePass* statePass;
	StatePlayerTurnStart* statePlayerTurnStart;
	StateRoundEnd* stateRoundEnd;
	StateRoundStart* stateRoundStart;
	StateHandCardStaged* stateHandCardStaged;
	StateTurnEnd* stateTurnEnd;
	StateTutorial* stateTutorial;

	CardReplaceBanner* cardReplaceBanner;
	OpponentFirstBanner* opponentFirstBanner;
	OpponentLastStandBanner* opponentLastStandBanner;
	OpponentPassBanner* opponentPassBanner;
	OpponentRoundWinBanner* opponentRoundWinBanner;
	OpponentTurnBanner* opponentTurnBanner;
	PlayerFirstBanner* playerFirstBanner;
	PlayerLastStandBanner* playerLastStandBanner;
	PlayerPassBanner* playerPassBanner;
	PlayerRoundWinBanner* playerRoundWinBanner;
	PlayerTurnBanner* playerTurnBanner;
	ChallengeBanner* challengeBanner;
	RoundBanner* roundBanner;
	RoundTieBanner* roundTieBanner;
	VictoryBanner* victoryBanner;
	DefeatBanner* defeatBanner;
	DrawBanner* drawBanner;

	ClickableNode* boardSelection;
	DeckCardCountDisplay* deckCardCountDisplay;
	HandCardCountDisplay* handCardCountDisplay;
	RemainingCardDisplay* remainingCardDisplay;
	DrawCountDisplay* drawCountDisplay;
	LossesDisplay* lossesDisplay;
	RowTotals* rowTotals;
	ScoreTotal* scoreTotal;
	DebugDisplay* debugDisplay;

	cocos2d::Node* tutorialLayer;
	cocos2d::Node* relocateLayer;

	cocos2d::LayerColor* menuBackDrop;
	HelpMenu* helpMenu;

	Track* musicA;
	Track* musicB;

	static Hexus* instance;
};
