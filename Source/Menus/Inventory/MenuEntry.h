#pragma once

#include "Engine/SmartNode.h"

namespace cocos2d
{
	class Sprite;
}

class LocalizedLabel;
class LocalizedString;

class MenuEntry : public SmartNode
{
public:
	static MenuEntry* create(LocalizedString* text, std::string iconResource);

	void hideIcon();
	void showIcon();

protected:
	MenuEntry(LocalizedString* text, std::string iconResource);
	virtual ~MenuEntry();
	
	void onEnter() override;
	void initializePositions() override;

	void sizeFont();

	cocos2d::Sprite* icon = nullptr;
	LocalizedLabel* label = nullptr;

private:
	typedef SmartNode super;

	static const cocos2d::CSize LabelSize;
};
