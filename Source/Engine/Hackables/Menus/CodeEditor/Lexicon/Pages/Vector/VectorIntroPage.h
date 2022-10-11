#pragma once

#include "Engine/Hackables/Menus/CodeEditor/Lexicon/Pages/LexiconPage.h"

namespace cocos2d
{
	class Sprite;
}

class LocalizedLabel;

class VectorIntroPage : public LexiconPage
{
public:
	static VectorIntroPage* create();

	static const std::string Identifier;

protected:
	VectorIntroPage();
	virtual ~VectorIntroPage();
	
	void initializePositions() override;
	void initializeListeners() override;
	
private:
	typedef LexiconPage super;

	cocos2d::Sprite* barrelSprite = nullptr;
	cocos2d::Sprite* chapterSprite = nullptr;
	LocalizedLabel* introText = nullptr;
	LocalizedLabel* titleLabel = nullptr;
};
