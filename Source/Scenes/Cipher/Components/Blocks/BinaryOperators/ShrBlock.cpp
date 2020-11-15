#include "ShrBlock.h"

#include "cocos/2d/CCActionInterval.h"
#include "cocos/2d/CCSprite.h"
#include "cocos/base/CCDirector.h"

#include "Engine/Input/ClickableNode.h"
#include "Scenes/Cipher/CipherConfig.h"

#include "Resources/CipherResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

ShrBlock* ShrBlock::create(BlockType blockType)
{
	ShrBlock* instance = new ShrBlock(blockType);

	instance->autorelease();

	return instance;
}

ShrBlock::ShrBlock(BlockType blockType) : super(blockType, ConnectionType::Double, ConnectionType::Single, ClickableNode::create(CipherResources::Blocks_BlockTeal, CipherResources::Blocks_BlockTeal), CipherResources::Icons_ShiftRight, Strings::Cipher_Operations_SHR::create())
{
}

ShrBlock::~ShrBlock()
{
}

void ShrBlock::onEnter()
{
	super::onEnter();
}

void ShrBlock::initializePositions()
{
	super::initializePositions();

	Size visibleSize = Director::getInstance()->getVisibleSize();
}

unsigned char ShrBlock::compute()
{
	return this->inputLeft >> this->inputRight;
}
