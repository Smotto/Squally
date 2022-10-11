#pragma once

#include "Scenes/Hexus/Components/Tutorials/TutorialBase.h"

class FocusTakeOver;
class HelpArrow;
class LocalizedLabel;
class ClickableTextNode;

class TutorialPuzzleShlOverflow : public TutorialBase
{
public:
	static TutorialPuzzleShlOverflow* create();

protected:
	TutorialPuzzleShlOverflow();
	virtual ~TutorialPuzzleShlOverflow();

	void onEnter() override;
	void initializePositions() override;
	void initializeListeners() override;
	void onBeforeStateChange(GameState* gameState) override;
	void onAnyStateChange(GameState* gameState) override;
	bool tryHijackState(GameState* gameState) override;
	void unHijackState(GameState* gameState) override;

private:
	typedef TutorialBase super;

	void initializeCallbacks(GameState* gameState);
	void runTutorialIntro(GameState* gameState);
\
	FocusTakeOver* focusTakeOver = nullptr;
	LocalizedLabel* introTutorialLabel = nullptr;
	ClickableTextNode* introNextButton = nullptr;
};
