#include "ImmediateBlock.h"

#include "cocos/2d/CCActionInterval.h"
#include "cocos/2d/CCSprite.h"
#include "cocos/base/CCEventCustom.h"
#include "cocos/base/CCEventListenerCustom.h"
#include "cocos/base/CCDirector.h"

#include "Engine/Input/ClickableNode.h"
#include "Engine/Input/InputText.h"
#include "Engine/Localization/ConstantString.h"
#include "Engine/Localization/LocalizedLabel.h"
#include "Engine/Utils/HackUtils.h"
#include "Events/CipherEvents.h"
#include "Scenes/Cipher/CipherConfig.h"
#include "Scenes/Cipher/Components/Blocks/Connections/InputBolt.h"
#include "Scenes/Cipher/Components/Blocks/Connections/OutputBolt.h"
#include "Scenes/Cipher/Components/Letters/SmartAsciiLabel.h"

#include "Resources/CipherResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

ImmediateBlock* ImmediateBlock::create(unsigned char immediate, BlockType blockType)
{
	ImmediateBlock* instance = new ImmediateBlock(blockType);

	instance->setValue(immediate);

	instance->autorelease();

	return instance;
}

ImmediateBlock::ImmediateBlock(BlockType blockType) : super(blockType, ConnectionType::None, ConnectionType::Single, ClickableNode::create(CipherResources::Blocks_BlockDecLong, CipherResources::Blocks_BlockDecLong), CipherResources::Icons_Immediate, Strings::Cipher_Operations_Immediate::create())
{
	this->displayDataType = CipherEvents::DisplayDataType::Ascii;
	this->spriteAscii = Sprite::create(CipherResources::Blocks_BlockAsciiLong);
	this->spriteBin = Sprite::create(CipherResources::Blocks_BlockBinLong);
	this->spriteDec = Sprite::create(CipherResources::Blocks_BlockDecLong);
	this->spriteHex = Sprite::create(CipherResources::Blocks_BlockHexLong);
	this->spriteAsciiSelected = Sprite::create(CipherResources::Blocks_BlockAsciiLong);
	this->spriteBinSelected = Sprite::create(CipherResources::Blocks_BlockBinLong);
	this->spriteDecSelected = Sprite::create(CipherResources::Blocks_BlockDecLong);
	this->spriteHexSelected = Sprite::create(CipherResources::Blocks_BlockHexLong);
	this->displayLabel = SmartAsciiLabel::create();

	this->spriteAscii->setAnchorPoint(Vec2::ZERO);
	this->spriteBin->setAnchorPoint(Vec2::ZERO);
	this->spriteDec->setAnchorPoint(Vec2::ZERO);
	this->spriteHex->setAnchorPoint(Vec2::ZERO);
	this->spriteAsciiSelected->setAnchorPoint(Vec2::ZERO);
	this->spriteBinSelected->setAnchorPoint(Vec2::ZERO);
	this->spriteDecSelected->setAnchorPoint(Vec2::ZERO);
	this->spriteHexSelected->setAnchorPoint(Vec2::ZERO);

	this->block->getContent()->setOpacity(1);
	this->block->getContent()->setCascadeOpacityEnabled(false);
	this->block->getContentSelected()->setOpacity(1);
	this->block->getContentSelected()->setCascadeOpacityEnabled(false);
	
	if (this->blockType == BlockBase::BlockType::Static)
	{
		if (this->inputBoltLeft != nullptr)
		{
			inputBoltLeft->setVisible(false);
		}

		if (this->inputBoltRight != nullptr)
		{
			inputBoltRight->setVisible(false);
		}

		if (this->outputBoltLeft != nullptr)
		{
			outputBoltLeft->setVisible(false);
		}

		if (this->outputBoltLeft != nullptr)
		{
			outputBoltLeft->setVisible(false);
		}
	}

	if (this->blockType == BlockBase::BlockType::Normal || this->blockType == BlockBase::BlockType::Static)
	{
		this->icon->setVisible(false);
	}

	// Huh? I think we've got a bug where this isn't called on spawned objects, just patch it in
	this->initializePositions();

	this->block->getContent()->addChild(this->spriteAscii);
	this->block->getContent()->addChild(this->spriteBin);
	this->block->getContent()->addChild(this->spriteDec);
	this->block->getContent()->addChild(this->spriteHex);
	this->block->getContentSelected()->addChild(this->spriteAsciiSelected);
	this->block->getContentSelected()->addChild(this->spriteBinSelected);
	this->block->getContentSelected()->addChild(this->spriteDecSelected);
	this->block->getContentSelected()->addChild(this->spriteHexSelected);
	this->addChild(this->displayLabel);
}

ImmediateBlock::~ImmediateBlock()
{
}

void ImmediateBlock::onEnter()
{
	super::onEnter();
}

void ImmediateBlock::initializePositions()
{
	super::initializePositions();

	CSize visibleSize = Director::getInstance()->getVisibleSize();

	this->displayLabel->setPositionY(4.0f);
}

void ImmediateBlock::initializeListeners()
{
	super::initializeListeners();

	this->addEventListenerIgnorePause(EventListenerCustom::create(CipherEvents::EventChangeDisplayDataType, [&](EventCustom* eventCustom)
	{
		CipherEvents::CipherChangeDisplayDataTypeArgs* args = static_cast<CipherEvents::CipherChangeDisplayDataTypeArgs*>(eventCustom->getData());

		if (args != nullptr)
		{
			this->displayDataType = args->displayDataType;

			this->loadDisplayValue();
		}
	}));
}

void ImmediateBlock::setValue(unsigned char value)
{
	this->charValue = value;

	this->loadDisplayValue();
}

unsigned char ImmediateBlock::getValue()
{
	return this->charValue;
}

void ImmediateBlock::loadDisplayValue()
{
	this->spriteAscii->setVisible(false);
	this->spriteBin->setVisible(false);
	this->spriteDec->setVisible(false);
	this->spriteHex->setVisible(false);
	this->spriteAsciiSelected->setVisible(false);
	this->spriteBinSelected->setVisible(false);
	this->spriteDecSelected->setVisible(false);
	this->spriteHexSelected->setVisible(false);

	this->displayLabel->loadDisplayValue(this->charValue, this->displayDataType, false);
	
	switch(this->displayDataType)
	{
		default:
		case CipherEvents::DisplayDataType::Ascii:
		{
			this->spriteAscii->setVisible(true);
			this->spriteAsciiSelected->setVisible(true);
			break;
		}
		case CipherEvents::DisplayDataType::Bin:
		{
			this->spriteBin->setVisible(true);
			this->spriteBinSelected->setVisible(true);
			break;
		}
		case CipherEvents::DisplayDataType::Dec:
		{
			this->spriteDec->setVisible(true);
			this->spriteDecSelected->setVisible(true);
			break;
		}
		case CipherEvents::DisplayDataType::Hex:
		{
			this->spriteHex->setVisible(true);
			this->spriteHexSelected->setVisible(true);
			break;
		}
	}
}

unsigned char ImmediateBlock::compute()
{
	return this->charValue;
}
