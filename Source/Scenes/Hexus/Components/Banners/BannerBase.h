#pragma once
#include <string>

#include "Scenes/Hexus/Components/ComponentBase.h"

namespace cocos2d
{
	class LayerColor;
}

class LocalizedLabel;
class LocalizedString;

class BannerBase : public ComponentBase
{
public:
	static BannerBase* create();

protected:
	BannerBase();
	virtual ~BannerBase();

	void onEnter() override;
	void initializePositions() override;
	void onBeforeStateChange(GameState* gameState) override;
	void onAnyStateChange(GameState* gameState) override;
	void setBannerText(LocalizedString* text);
	void flashBanner();
	void showBanner();
	void hideBanner();
	void addBannerChild(Node* child);

private:
	typedef ComponentBase super;

	cocos2d::LayerColor* bannerOverlay = nullptr;
	LocalizedLabel* bannerLabel = nullptr;
	cocos2d::Node* bannerChildrenNode = nullptr;
};
