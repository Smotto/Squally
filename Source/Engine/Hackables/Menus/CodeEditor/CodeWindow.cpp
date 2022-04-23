﻿#include "CodeWindow.h"

#include "cocos/2d/CCLayer.h"
#include "cocos/2d/CCSprite.h"
#include "cocos/base/CCEventDispatcher.h"
#include "cocos/base/CCEventListenerCustom.h"

#include "Engine/UI/Controls/ScrollPane.h"
#include "Engine/UI/Controls/UIRichText.h"
#include "Engine/Events/HackableEvents.h"
#include "Engine/Events/LocalizationEvents.h"
#include "Engine/Hackables/Menus/CodeEditor/ScriptEntry.h"
#include "Engine/Input/ClickableNode.h"
#include "Engine/Input/InputText.h"
#include "Engine/Localization/ConstantString.h"
#include "Engine/Localization/LocalizedLabel.h"
#include "Engine/Localization/LocalizedString.h"
#include "Engine/Utils/StrUtils.h"

#include "Strings/Strings.h"

#include "Resources/UIResources.h"

using namespace cocos2d;
using namespace cocos2d::ui;

const std::string CodeWindow::Delimiters = "[],:; +-*\n\t";

const std::set<std::string> CodeWindow::Registers =
{
	// General registers
	"ax", "bx", "cx", "dx", "si", "di", "bp", "sp", "ip",
	"al", "bl", "cl", "dl", "sil", "dil", "bpl", "spl",
	"ah", "bh", "ch", "dh",
	"eax", "ebx", "ecx" ,"edx" ,"esi", "edi", "ebp", "esp",
	"r8w", "r9w", "r10w", "r11w", "r12w", "r13w", "r14w", "r15w",
	"r8b", "r9b", "r10b", "r11b", "r12b", "r13b", "r14b", "r15b",
	"r8d", "r9d", "r10d", "r11d", "r12d", "r13d", "r14d", "r15d",
	"rax", "rbx", "rcx" ,"rdx" ,"rdi", "rsi", "rbp", "rsp",
	"r8", "r9", "r10", "r11", "r12", "r13", "r14", "r15",

	// FPU registers
	"fp0", "fp1", "fp2", "fp3", "fp4", "fp5", "fp6", "fp7",

	// FPU stack registers
	"st0", "st1", "st2", "st3", "st4", "st5", "st6", "st7",

	// MMX registers
	"mm0", "mm1", "mm2", "mm3", "mm4", "mm5", "mm6", "mm7",
	"mm8", "mm9", "mm10", "mm11", "mm12", "mm13", "mm14", "mm15",
	"xmm0", "xmm1", "xmm2", "xmm3", "xmm4", "xmm5", "xmm6", "xmm7",
	"xmm8", "xmm9", "xmm10", "xmm11", "xmm12", "xmm13", "xmm14", "xmm15",
	"ymm0", "ymm1", "ymm2", "ymm3", "ymm4", "ymm5", "ymm6", "ymm7",
	"ymm8", "ymm9", "ymm10", "ymm11", "ymm12", "ymm13", "ymm14", "ymm15",

	// Segment registers
	"cs", "ds", "es", "fs", "gs", "ss",

	// Instruction pointers
	"eip", "rip"
};

const Color3B CodeWindow::DefaultColor = Color3B(255, 255, 255);
const Color3B CodeWindow::RegisterColor = Color3B(86, 156, 214);
const Color3B CodeWindow::NumberColor = Color3B(181, 206, 168);
const Color3B CodeWindow::CommentColor = Color3B(87, 166, 74);
const Color3B CodeWindow::LineNumberColor = Color3B(166, 166, 166);
const CSize CodeWindow::Padding = CSize(8.0f, 4.0f);
const float CodeWindow::MarginSize = 48.0f;
const float CodeWindow::TitleBarHeight = 64.0f;
const Color4B CodeWindow::DefaultTitleBarColor = Color4B(59, 92, 97, 255);
const Color4B CodeWindow::DefaultWindowColor = Color4B(39, 58, 61, 255);
const Color4B CodeWindow::ReadonlyWindowColor = Color4B(70, 66, 10, 255);

CodeWindow* CodeWindow::create(cocos2d::CSize windowSize)
{
	CodeWindow* instance = new CodeWindow(windowSize);

	instance->autorelease();

	return instance;
}

CodeWindow::CodeWindow(cocos2d::CSize windowSize)
{
	this->windowSize = windowSize;

	this->lineNumbers = RichText::create();
	this->editableText = InputText::create(
		CSize(windowSize.width - CodeWindow::MarginSize - CodeWindow::Padding.width * 2.0f, this->windowSize.height - CodeWindow::Padding.height * 2.0f),
		LocalizedLabel::FontStyle::Coding,
		LocalizedLabel::FontSize::H3
	);
	this->displayedText = RichText::create();
	this->contentPane = ScrollPane::create(
		windowSize,
		UIResources::Menus_Buttons_SliderButton,
		UIResources::Menus_Buttons_SliderButtonSelected,
		CodeWindow::Padding,
		CSize::ZERO
	);
	this->background = LayerColor::create(CodeWindow::DefaultWindowColor, this->windowSize.width, this->windowSize.height);
	this->titleBar = LayerColor::create(CodeWindow::DefaultTitleBarColor, this->windowSize.width, CodeWindow::TitleBarHeight);
	this->windowTitle = InputText::create(
		CSize(this->windowSize.width - 16.0f - 88.0f, CodeWindow::TitleBarHeight - 16.0f),
		LocalizedLabel::FontStyle::Main,
		LocalizedLabel::FontSize::H3
	);
	this->deleteButton = ClickableNode::create(UIResources::Menus_HackerModeMenu_TrashCan, UIResources::Menus_HackerModeMenu_TrashCanSelected);
	this->copyButtonGlow = Sprite::create(UIResources::HUD_EmblemGlow);
	this->copyButton = ClickableNode::create(UIResources::Menus_HackerModeMenu_Copy, UIResources::Menus_HackerModeMenu_CopySelected);
	this->copyPanel = LayerColor::create(Color4B::BLACK, 256.0f, 48.0f);
	this->copyLabel = LocalizedLabel::create(LocalizedLabel::FontStyle::Main, LocalizedLabel::FontSize::P, Strings::Menus_Hacking_CodeEditor_CopyScript::create());
	this->deletePanel = LayerColor::create(Color4B::BLACK, 256.0f, 48.0f);
	this->deleteLabel = LocalizedLabel::create(LocalizedLabel::FontStyle::Main, LocalizedLabel::FontSize::P, Strings::Menus_Hacking_CodeEditor_DeleteScript::create());

	this->windowTitle->setAnchorPoint(Vec2(0.0f, 0.5f));
	this->displayedText->setAnchorPoint(Vec2(0.0f, 1.0f));
	this->displayedText->setWrapMode(RichText::WrapMode::WRAP_PER_CHAR);
	this->displayedText->ignoreContentAdaptWithSize(false);

	this->lineNumbers->setAnchorPoint(Vec2(0.0f, 1.0f));
	this->lineNumbers->ignoreContentAdaptWithSize(false);
	this->editableText->setAnchorPoint(Vec2(0.0f, 1.0f));
	this->editableText->setLineBreakWithoutSpace(true);
	this->editableText->setTextColor(Color4B::WHITE);
	this->editableText->setCascadeOpacityEnabled(false);
	this->editableText->setCursorEnabled(true);
	this->editableText->enableWrap(true);
	this->editableText->setOpacity(0);

	this->contentPane->addChild(this->lineNumbers);
	this->contentPane->addChild(this->editableText);
	this->contentPane->addChild(this->displayedText);
	this->addChild(this->background);
	this->addChild(this->titleBar);
	this->addChild(this->windowTitle);
	this->addChild(this->contentPane);
	this->addChild(this->deleteButton);
	this->addChild(this->copyButtonGlow);
	this->addChild(this->copyButton);
	this->addChild(this->deletePanel);
	this->addChild(this->deleteLabel);
	this->addChild(this->copyPanel);
	this->addChild(this->copyLabel);
}

CodeWindow::~CodeWindow()
{
}


void CodeWindow::onEnter()
{
	super::onEnter();
	this->scheduleUpdate();

	this->deletePanel->setOpacity(0);
	this->deleteLabel->setOpacity(0);
	this->copyPanel->setOpacity(0);
	this->copyLabel->setOpacity(0);

	this->editableText->scheduleUpdate();
}

void CodeWindow::initializePositions()
{
	super::initializePositions();

	CSize windowSize = this->background->getContentSize();

	this->background->setPosition(-windowSize.width / 2.0f, -windowSize.height / 2.0f);
	this->titleBar->setPosition(-windowSize.width / 2.0f, windowSize.height / 2.0f);
	this->windowTitle->setPosition(-windowSize.width / 2.0f + 8.0f, windowSize.height / 2 + CodeWindow::TitleBarHeight / 2.0f);
	this->deleteButton->setPosition(windowSize.width / 2.0f - 32.0f - 40.0f, windowSize.height / 2 + CodeWindow::TitleBarHeight / 2.0f);
	this->copyButtonGlow->setPosition(windowSize.width / 2.0f - 32.0f, windowSize.height / 2 + CodeWindow::TitleBarHeight / 2.0f);
	this->copyButton->setPosition(windowSize.width / 2.0f - 32.0f, windowSize.height / 2 + CodeWindow::TitleBarHeight / 2.0f);

	this->displayedText->setContentSize(CSize(windowSize.width - CodeWindow::MarginSize - CodeWindow::Padding.width * 2.0f, windowSize.height - CodeWindow::Padding.height * 2.0f));
	this->lineNumbers->setContentSize(CSize(windowSize.width - CodeWindow::MarginSize - CodeWindow::Padding.width * 2.0f, windowSize.height - CodeWindow::Padding.height * 2.0f));

	this->lineNumbers->setPosition(Vec2(-this->contentPane->getPaneSize().width / 2.0f + 20.0f, 0.0f));
	this->displayedText->setPosition(Vec2(-this->contentPane->getPaneSize().width / 2.0f + CodeWindow::MarginSize, 0.0f));
	this->editableText->setPosition(Vec2(-this->contentPane->getPaneSize().width / 2.0f + CodeWindow::MarginSize, 0.0f));
	
	this->copyPanel->setPosition(this->copyButton->getPosition() + Vec2(0.0f, 48.0f) - Vec2(this->copyPanel->getContentSize() / 2.0f));
	this->copyLabel->setPosition(this->copyButton->getPosition() + Vec2(0.0f, 48.0f));
	this->deletePanel->setPosition(this->deleteButton->getPosition() + Vec2(0.0f, 48.0f) - Vec2(this->deletePanel->getContentSize() / 2.0f));
	this->deleteLabel->setPosition(this->deleteButton->getPosition() + Vec2(0.0f, 48.0f));
}

void CodeWindow::initializeListeners()
{
	super::initializeListeners();

	this->addEventListenerIgnorePause(EventListenerCustom::create(HackableEvents::EventHackableBaseEdit, [=](EventCustom* args)
	{
		this->editableText->getHitbox()->setAllowCollisionWhenInvisible(true);
	}));

	this->addEventListenerIgnorePause(EventListenerCustom::create(HackableEvents::EventHackableBaseEditDone, [=](EventCustom* args)
	{
		this->editableText->getHitbox()->setAllowCollisionWhenInvisible(false);
	}));

	this->addEventListenerIgnorePause(EventListenerCustom::create(HackableEvents::EventHackableObjectClose, [=](EventCustom* args)
	{
		this->editableText->getHitbox()->setAllowCollisionWhenInvisible(false);
	}));

	this->editableText->setStringChangeCallback([=](std::string newText)
	{
		if (this->script != nullptr)
		{
			this->script->setScript(newText);
		}

		this->constructTokenizedText(newText);

		this->contentPane->updateScrollBounds();
	});

	this->windowTitle->setStringChangeCallback([=](std::string newTitle)
	{
		if (this->script != nullptr && !this->script->isReadOnly)
		{
			this->script->setName(newTitle);
		}
	});

	this->copyButton->setMouseClickCallback([=](InputEvents::MouseEventArgs*)
	{
		if (this->script != nullptr)
		{
			this->script->copyScript();
		}
	});

	this->deleteButton->setMouseClickCallback([=](InputEvents::MouseEventArgs*)
	{
		if (this->script != nullptr)
		{
			this->script->deleteScript();
		}
	});

	this->deleteButton->setMouseOverCallback([=](InputEvents::MouseEventArgs*)
	{
		this->deletePanel->setOpacity(196);
		this->deleteLabel->setOpacity(255);
	});

	this->deleteButton->setMouseOutCallback([=](InputEvents::MouseEventArgs*)
	{
		this->deletePanel->setOpacity(0);
		this->deleteLabel->setOpacity(0);
	});

	this->copyButton->setMouseOverCallback([=](InputEvents::MouseEventArgs*)
	{
		this->copyPanel->setOpacity(196);
		this->copyLabel->setOpacity(255);
	});
	this->copyButton->setMouseOutCallback([=](InputEvents::MouseEventArgs*)
	{
		this->copyPanel->setOpacity(0);
		this->copyLabel->setOpacity(0);
	});
}

void CodeWindow::openScript(ScriptEntry* script)
{
	this->script = script;
	this->clearText();
	this->editableText->setString("");
	this->deleteButton->setVisible(false);

	if (this->script == nullptr)
	{
		return;
	}

	if (script->isReadOnly)
	{
		this->copyButtonGlow->setVisible(true);
		this->background->setColor(Color3B(CodeWindow::ReadonlyWindowColor));
		this->background->setOpacity(CodeWindow::ReadonlyWindowColor.a);
		this->windowTitle->getHitbox()->disableInteraction();
		this->editableText->getHitbox()->disableInteraction();
		this->unfocus();
	}
	else
	{
		this->copyButtonGlow->setVisible(false);
		this->background->setColor(Color3B(CodeWindow::DefaultWindowColor));
		this->background->setOpacity(CodeWindow::DefaultWindowColor.a);
		this->deleteButton->setVisible(true);
		this->windowTitle->getHitbox()->enableInteraction();
		this->editableText->getHitbox()->enableInteraction();
		this->focus();
	}

	std::string scriptText = script->getScript();
	LocalizedString* name = script->getName();

	this->setText(scriptText);
	this->constructTokenizedText(scriptText);
	this->setWindowTitle(name == nullptr ? "" : name->getString());
	this->contentPane->updateScrollBounds();
}

std::string CodeWindow::getText()
{
	return this->editableText->getString();
}

void CodeWindow::setText(std::string text)
{
	this->hasScriptChanges = true;
	this->editableText->setString(text);
}

bool CodeWindow::hasChanges()
{
	return this->hasScriptChanges;
}

void CodeWindow::clearHasChanges()
{
	this->hasScriptChanges = false;
}

void CodeWindow::focus()
{
	// Focusing readonly scripts is not allowed
	if (this->script != nullptr && this->script->isReadOnly)
	{
		this->unfocus();

		return;
	}

	this->editableText->focus();
}

void CodeWindow::unfocus()
{
	this->windowTitle->unfocus();
	this->editableText->unfocus();
}

void CodeWindow::clearText()
{
	this->currentLineNumber = 1;

	this->textElements.clear();
	this->lineNumberElements.clear();

	for (auto textInfo : this->textInfo)
	{
		std::get<0>(textInfo)->release();
	}

	this->textInfo.clear();
	this->displayedText->clearElements();
	this->lineNumbers->clearElements();
}

void CodeWindow::constructTokenizedText(std::string currentText)
{
	std::vector<AsmToken> tokens = std::vector<AsmToken>();

	// Due to RichTextBoxes being garbage, we need to split text down further if they contain newlines
	// Also split them down further if they contain comments
	std::vector<std::string> splitText = StrUtils::splitOn(currentText, ";\n", true);
	std::vector<std::string> textJoined = std::vector<std::string>();
	std::string currentString = "";
	bool isJoiningComment = false;

	for (auto next : splitText)
	{
		// Newlines end comments
		if (next == "\n")
		{
			if (!currentString.empty())
			{
				textJoined.push_back(currentString);
			}

			textJoined.push_back(next);

			isJoiningComment = false;
			currentString = "";
		}
		else if (next == ";" || next == "//" || isJoiningComment)
		{
			isJoiningComment = true;
			currentString += next;
		}
		else
		{
			textJoined.push_back(next);
		}
	}

	// Add final joined comment if exists
	if (isJoiningComment && !currentString.empty())
	{
		textJoined.push_back(currentString);
	}

	for (auto text : textJoined)
	{
		std::vector<std::string> tokenStrings;

		// Tokenize the string if it isn't a comment -- otherwise treat it as one token
		if (!StrUtils::startsWith(text, ";", false) && !StrUtils::startsWith(text, "//", false))
		{
			tokenStrings = StrUtils::tokenize(text, CodeWindow::Delimiters);
		}
		else
		{
			tokenStrings = std::vector<std::string>();
			tokenStrings.push_back(text);
		}

		// Iterate tokens
		for (auto token : tokenStrings)
		{
			Color3B color = CodeWindow::DefaultColor;

			if (CodeWindow::Registers.find(token) != CodeWindow::Registers.end())
			{
				color = CodeWindow::RegisterColor;
			}
			else if (StrUtils::isInteger(token) || StrUtils::isFloat(token) || StrUtils::isHexNumber(token))
			{
				color = CodeWindow::NumberColor;
			}
			else if (StrUtils::startsWith(token, ";", false) || StrUtils::startsWith(token, "//", false))
			{
				color = CodeWindow::CommentColor;
			}

			AsmToken nextToken = AsmToken(ConstantString::create(token), color);
			tokens.push_back(nextToken);
		}
	}

	this->clearText();
	this->insertNewline();

	for (auto token : tokens)
	{
		if (token.tokenStr->getString() == "\n")
		{
			this->insertNewline();
		}
		else
		{
			this->insertText(token.tokenStr, token.color);
		}
	}

	this->hasScriptChanges = true;
}

void CodeWindow::setWindowTitle(std::string windowTitle)
{
	this->windowTitle->setString(windowTitle);
}

void CodeWindow::insertNewline()
{
	RichElement* lineNumberText = RichElementText::create(0, CodeWindow::LineNumberColor, 0xFF, std::to_string(this->currentLineNumber), this->editableText->getFont(), this->editableText->getFontSize());
	RichElement* lineNumberNewLine = RichElementNewLine::create(0, CodeWindow::DefaultColor, 0xFF);
	LocalizedString* text = Strings::Common_Newline::create();
	RichElement* element = RichElementNewLine::create(0, CodeWindow::DefaultColor, 0xFF);
	int textElementIndex = this->currentLineNumber - 2;

	text->retain();
	this->textInfo.push_back(std::make_tuple(text, Color3B()));
	this->displayedText->pushBackElement(element);

	// Format call required to properly get newline counts on wraps
	this->displayedText->formatText();
	
	// Add additional newlines in line numbers to handle wraps
	if (textElementIndex >= 0 && textElementIndex < int(this->textElements.size()))
	{
		for (int wrapNewlineIndex = 0; wrapNewlineIndex < this->textElements[textElementIndex]->getNewlineCount(); wrapNewlineIndex++)
		{
			RichElement* wrapNewline = RichElementNewLine::create(0, CodeWindow::DefaultColor, 0xFF);

			this->lineNumberElements.push_back(wrapNewline);
			this->lineNumbers->pushBackElement(wrapNewline);
		}
	}

	this->lineNumberElements.push_back(lineNumberText);
	this->lineNumbers->pushBackElement(lineNumberText);
	this->lineNumberElements.push_back(lineNumberNewLine);
	this->lineNumbers->pushBackElement(lineNumberNewLine);

	this->currentLineNumber++;
}

void CodeWindow::insertText(LocalizedString* text, Color3B color)
{
	RichElementText* element = RichElementText::create(0, color, 0xFF, text->getString(), this->editableText->getFont(), this->editableText->getFontSize());

	text->retain();
	this->textInfo.push_back(std::make_tuple(text, color));
	this->textElements.push_back(element);
	this->displayedText->pushBackElement(element);
}
