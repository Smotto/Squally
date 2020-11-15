#include "LessThanOrEqualsBlock.h"

#include "cocos/2d/CCActionInterval.h"
#include "cocos/2d/CCSprite.h"
#include "cocos/base/CCDirector.h"

#include "Engine/Input/ClickableNode.h"
#include "Scenes/Cipher/CipherConfig.h"

#include "Resources/CipherResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

LessThanOrEqualsBlock* LessThanOrEqualsBlock::create(BlockType blockType)
{
	LessThanOrEqualsBlock* instance = new LessThanOrEqualsBlock(blockType);

	instance->autorelease();

	return instance;
}

LessThanOrEqualsBlock::LessThanOrEqualsBlock(BlockType blockType) : super(blockType, CipherResources::Icons_LessThanOrEquals, Strings::Cipher_Operations_SHR::create())
{
}

LessThanOrEqualsBlock::~LessThanOrEqualsBlock()
{
}

unsigned char LessThanOrEqualsBlock::compute()
{
	return this->inputLeft;
}

bool LessThanOrEqualsBlock::compare(unsigned char inputA, unsigned char inputB)
{
	return inputA <= inputB;
}
