﻿#include "BinarySelectPage.h"

#include "cocos/2d/CCSprite.h"

#include "Engine/Events/HackableEvents.h"
#include "Engine/Hackables/Menus/CodeEditor/Lexicon/Pages/Binary/And/AndPage.h"
#include "Engine/Hackables/Menus/CodeEditor/Lexicon/Pages/Binary/Bswap/BswapPage.h"
#include "Engine/Hackables/Menus/CodeEditor/Lexicon/Pages/Binary/Not/NotPage.h"
#include "Engine/Hackables/Menus/CodeEditor/Lexicon/Pages/Binary/Or/OrPage.h"
#include "Engine/Hackables/Menus/CodeEditor/Lexicon/Pages/Binary/Shl/ShlPage.h"
#include "Engine/Hackables/Menus/CodeEditor/Lexicon/Pages/Binary/Shr/ShrPage.h"
#include "Engine/Hackables/Menus/CodeEditor/Lexicon/Pages/Binary/Xor/XorPage.h"
#include "Engine/Input/ClickableTextNode.h"
#include "Engine/Localization/LocalizedLabel.h"

#include "Resources/UIResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

const std::string BinarySelectPage::Identifier = "BINARY_SELECT_PAGE";

BinarySelectPage* BinarySelectPage::create()
{
	BinarySelectPage* instance = new BinarySelectPage();

	instance->autorelease();

	return instance;
}

BinarySelectPage::BinarySelectPage() : super(BinarySelectPage::Identifier, PageType::Right)
{
	this->decorLine = Sprite::create(UIResources::Menus_LexiconMenu_DecorLine);

	this->operationSelectLabel = LocalizedLabel::create(LocalizedLabel::FontStyle::Main, LocalizedLabel::FontSize::H2, Strings::Menus_Hacking_Lexicon_Pages_ChapterSelect_ChooseAnOperation::create());
	this->andOperationButton = this->buildInstructionLabel(Strings::Menus_Hacking_Lexicon_Instructions_Binary_And::create(), AndPage::Identifier);
	this->orOperationButton = this->buildInstructionLabel(Strings::Menus_Hacking_Lexicon_Instructions_Binary_Or::create(), OrPage::Identifier);
	this->xorOperationButton = this->buildInstructionLabel(Strings::Menus_Hacking_Lexicon_Instructions_Binary_Xor::create(), XorPage::Identifier);
	this->shlOperationButton = this->buildInstructionLabel(Strings::Menus_Hacking_Lexicon_Instructions_Binary_Shl::create(), ShlPage::Identifier);
	this->shrOperationButton = this->buildInstructionLabel(Strings::Menus_Hacking_Lexicon_Instructions_Binary_Shr::create(), ShrPage::Identifier);
	this->notOperationButton = this->buildInstructionLabel(Strings::Menus_Hacking_Lexicon_Instructions_Binary_Not::create(), NotPage::Identifier);
	this->bswapOperationButton = this->buildInstructionLabel(Strings::Menus_Hacking_Lexicon_Instructions_Binary_Bswap::create(), BswapPage::Identifier);

	this->operationSelectLabel->setTextColor(super::TextColor);

	this->addChild(this->decorLine);
	this->addChild(this->operationSelectLabel);
	this->addChild(this->andOperationButton);
	this->addChild(this->orOperationButton);
	this->addChild(this->xorOperationButton);
	this->addChild(this->shlOperationButton);
	this->addChild(this->shrOperationButton);
	this->addChild(this->notOperationButton);
	this->addChild(this->bswapOperationButton);
}

BinarySelectPage::~BinarySelectPage()
{
}

void BinarySelectPage::initializePositions()
{
	super::initializePositions();

	const float vOffset = -48.0f;
	const float vSpacing = -144.0f;

	this->decorLine->setPosition(Vec2(0.0f, super::PageSize.height / 2.0f + vOffset));
	this->operationSelectLabel->setPosition(Vec2(0.0f, super::PageSize.height / 2.0f + vOffset - 56.0f));
	this->notOperationButton->setPosition(Vec2(-112.0f, super::PageSize.height / 2.0f + vOffset - 144.0f + vSpacing * 0.0f));
	this->shlOperationButton->setPosition(Vec2(-112.0f, super::PageSize.height / 2.0f + vOffset - 144.0f + vSpacing * 1.0f));
	this->shrOperationButton->setPosition(Vec2(-112.0f, super::PageSize.height / 2.0f + vOffset - 144.0f + vSpacing * 2.0f));
	this->bswapOperationButton->setPosition(Vec2(-112.0f, super::PageSize.height / 2.0f + vOffset - 144.0f + vSpacing * 3.0f));

	this->andOperationButton->setPosition(Vec2(112.0f, super::PageSize.height / 2.0f + vOffset - 144.0f + vSpacing * 0.0f + vSpacing / 2.0f));
	this->orOperationButton->setPosition(Vec2(112.0f, super::PageSize.height / 2.0f + vOffset - 144.0f + vSpacing * 1.0f + vSpacing / 2.0f));
	this->xorOperationButton->setPosition(Vec2(112.0f, super::PageSize.height / 2.0f + vOffset - 144.0f + vSpacing * 2.0f + vSpacing / 2.0f));
}

void BinarySelectPage::initializeListeners()
{
	super::initializeListeners();
}
