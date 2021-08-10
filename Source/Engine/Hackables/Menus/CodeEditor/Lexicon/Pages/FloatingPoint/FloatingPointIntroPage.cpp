﻿#include "FloatingPointIntroPage.h"

#include "cocos/2d/CCSprite.h"

#include "Engine/Events/HackableEvents.h"
#include "Engine/Hackables/Menus/CodeEditor/Lexicon/Pages/ChapterSelectPage.h"
#include "Engine/Hackables/Menus/CodeEditor/Lexicon/Pages/IntroPage.h"
#include "Engine/Localization/LocalizedLabel.h"

#include "Resources/UIResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

const std::string FloatingPointIntroPage::Identifier = "FLOATING_POINT_INTRO_PAGE";

FloatingPointIntroPage* FloatingPointIntroPage::create()
{
	FloatingPointIntroPage* instance = new FloatingPointIntroPage();

	instance->autorelease();

	return instance;
}

FloatingPointIntroPage::FloatingPointIntroPage() : super(FloatingPointIntroPage::Identifier, PageType::Left)
{
	this->skullSprite = Sprite::create(UIResources::Menus_LexiconMenu_BookArt_Fly);
	this->chapterSprite = Sprite::create(UIResources::Menus_LexiconMenu_Chapter);
	this->titleLabel = LocalizedLabel::create(LocalizedLabel::FontStyle::Main, LocalizedLabel::FontSize::H2, Strings::Menus_Hacking_Lexicon_Pages_FloatingPoint_Title::create());
	this->introText = LocalizedLabel::create(
		LocalizedLabel::FontStyle::Main,
		LocalizedLabel::FontSize::P,
		Strings::Menus_Hacking_Lexicon_Pages_FloatingPoint_Intro::create(),
		CSize(super::PageSize.width - 160.0f, super::PageSize.height)
	);

	this->titleLabel->setTextColor(super::TextColor);
	this->titleLabel->setAnchorPoint(Vec2(0.0f, 1.0f));
	this->introText->setTextColor(super::TextColor);
	this->introText->setAnchorPoint(Vec2(0.0f, 1.0f));

	this->addChild(this->skullSprite);
	this->addChild(this->chapterSprite);
	this->addChild(this->introText);
	this->addChild(this->titleLabel);
}

FloatingPointIntroPage::~FloatingPointIntroPage()
{
}

void FloatingPointIntroPage::initializePositions()
{
	super::initializePositions();

	this->skullSprite->setPosition(Vec2(32.0f, -160.0f));
	this->introText->setPosition(super::IntroLocation);
	this->chapterSprite->setPosition(super::ChapterMarkerLocation);
	this->titleLabel->setPosition(super::ChapterLocation);
}

void FloatingPointIntroPage::initializeListeners()
{
	super::initializeListeners();

	this->enableBack(IntroPage::Identifier, ChapterSelectPage::Identifier);
}
