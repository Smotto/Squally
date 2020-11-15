#include "GreaterThanBlock.h"

#include "cocos/2d/CCActionInterval.h"
#include "cocos/2d/CCSprite.h"
#include "cocos/base/CCDirector.h"

#include "Engine/Input/ClickableNode.h"
#include "Scenes/Cipher/CipherConfig.h"

#include "Resources/CipherResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

GreaterThanBlock* GreaterThanBlock::create(BlockType blockType)
{
	GreaterThanBlock* instance = new GreaterThanBlock(blockType);

	instance->autorelease();

	return instance;
}

GreaterThanBlock::GreaterThanBlock(BlockType blockType) : super(blockType, CipherResources::Icons_GreaterThan, Strings::Cipher_Operations_SHR::create())
{
}

GreaterThanBlock::~GreaterThanBlock()
{
}

unsigned char GreaterThanBlock::compute()
{
	return this->inputLeft;
}

bool GreaterThanBlock::compare(unsigned char inputA, unsigned char inputB)
{
	return inputA > inputB;
}
