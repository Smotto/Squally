#pragma once

#include "Scenes/Hexus/Components/Banners/BannerBase.h"

namespace cocos2d
{
	class Sprite;
}

class OpponentRoundWinBanner : public BannerBase
{
public:
	static OpponentRoundWinBanner* create();

protected:
	OpponentRoundWinBanner();
	virtual ~OpponentRoundWinBanner();

	void initializePositions() override;
	void onBeforeStateChange(GameState* gameState) override;
	void onAnyStateChange(GameState* gameState) override;

private:
	typedef BannerBase super;

	cocos2d::Sprite* playerPassBanner1 = nullptr;
	cocos2d::Sprite* playerPassBanner2 = nullptr;
};
