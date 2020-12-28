#pragma once

#include "Engine/SmartNode.h"

class GameState;

class ComponentBase : public SmartNode
{
protected:
	ComponentBase();
	virtual ~ComponentBase();

	void initializeListeners() override;
	virtual void onBeforeStateChange(GameState* gameState);
	virtual void onBeforeAnyRequestStateChange(GameState* gameState);
	virtual void onAnyRequestStateChange(GameState* gameState);
	virtual void onAnyStateChange(GameState* gameState);
	virtual void onAfterAnyStateChange(GameState* gameState);

private:
	typedef SmartNode super;
};
