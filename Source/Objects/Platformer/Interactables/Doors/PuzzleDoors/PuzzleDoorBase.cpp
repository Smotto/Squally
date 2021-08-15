#include "PuzzleDoorBase.h"

#include "cocos/2d/CCActionInstant.h"
#include "cocos/2d/CCActionInterval.h"
#include "cocos/2d/CCActionEase.h"
#include "cocos/2d/CCDrawNode.h"
#include "cocos/2d/CCSprite.h"
#include "cocos/base/CCValue.h"

#include "Engine/Animations/SmartAnimationSequenceNode.h"
#include "Engine/Localization/ConstantString.h"
#include "Engine/Localization/LocalizedLabel.h"
#include "Engine/Physics/CollisionObject.h"
#include "Engine/Sound/WorldSound.h"
#include "Engine/UI/SmartClippingNode.h"
#include "Engine/Utils/GameUtils.h"
#include "Engine/Utils/MathUtils.h"
#include "Scenes/Platformer/Level/Physics/PlatformerCollisionType.h"

#include "Resources/ObjectResources.h"
#include "Resources/ParticleResources.h"
#include "Resources/SoundResources.h"
#include "Resources/UIResources.h"

using namespace cocos2d;

const int PuzzleDoorBase::RuneCount = 4;
const Color4B PuzzleDoorBase::PassColor = Color4B(70, 144, 75, 255);
const Color4B PuzzleDoorBase::FailColor = Color4B(144, 70, 70, 255);
const std::string PuzzleDoorBase::UnlockedSaveKey = "PUZZLE_DOOR_UNLOCKED";
const std::string PuzzleDoorBase::PropertyUnlockedByDefault = "unlocked";

PuzzleDoorBase::PuzzleDoorBase(ValueMap& properties,
		CSize doorClipSize,
		Vec2 doorClipOffset,
		Vec2 portalOffset,
		Vec2 indexPosition,
		Vec2 hackLabelPosition,
		Vec2 truthLabelPosition,
		Vec2 runeBasePosition,
		float runeSpacing,
		float doorOpenDelta)
	: super(properties, doorClipSize, portalOffset)
{
	this->barLeft = Sprite::create(ObjectResources::Doors_PuzzleDoor_BarLeft);
	this->barRight = Sprite::create(ObjectResources::Doors_PuzzleDoor_BarRight);
	this->lightLeft = Sprite::create(ObjectResources::Doors_PuzzleDoor_Light);
	this->lightRight = Sprite::create(ObjectResources::Doors_PuzzleDoor_Light);
	this->marker = Sprite::create(ObjectResources::Doors_PuzzleDoor_Marker);
	this->backNode = Node::create();
	this->doorNode = Node::create();
	this->frontNode = Node::create();
	this->indexString = ConstantString::create(std::to_string(0));
	this->truthString = ConstantString::create(std::to_string(0));
	this->hackableString = ConstantString::create(std::to_string(0));
	this->indexLabel = LocalizedLabel::create(LocalizedLabel::FontStyle::Main, LocalizedLabel::FontSize::M3, this->indexString);
	this->truthLabel = LocalizedLabel::create(LocalizedLabel::FontStyle::Main, LocalizedLabel::FontSize::M3, this->truthString);
	this->hackableLabel = LocalizedLabel::create(LocalizedLabel::FontStyle::Main, LocalizedLabel::FontSize::M3, this->hackableString);
	this->doorOpenSound = WorldSound::create();
	this->sliderSound = WorldSound::create(SoundResources::Platformer_Objects_Machines_StoneSlideHeavy1);
	this->sliderResetSound = WorldSound::create(SoundResources::Platformer_Objects_Machines_StoneSlideHeavy3);
	this->electricitySound = WorldSound::create(SoundResources::Cipher_Lightning);
	this->doorClipSize = doorClipSize;
	this->doorClipOffset = doorClipOffset;
	this->portalOffset = portalOffset;
	this->indexPosition = indexPosition;
	this->hackLabelPosition = hackLabelPosition;
	this->truthLabelPosition = truthLabelPosition;
	this->runeBasePosition = runeBasePosition;
	this->runeSpacing = runeSpacing;
	this->doorOpenDelta = doorOpenDelta;
	this->unlockedByDefault = GameUtils::getKeyOrDefault(this->properties, PuzzleDoorBase::PropertyUnlockedByDefault, Value(false)).asBool();

	for (int index = 0; index < PuzzleDoorBase::RuneCount; index++)
	{
		this->runeStates[index] = RuneState::Unset;
		this->runes.push_back(Sprite::create(ObjectResources::Doors_PuzzleDoor_Rune));
		this->runesPassed.push_back(Sprite::create(ObjectResources::Doors_PuzzleDoor_RuneGreen));
		this->runesFailed.push_back(Sprite::create(ObjectResources::Doors_PuzzleDoor_RuneRed));
	}

	this->doorClip = SmartClippingNode::create(this->doorNode, CRect(Vec2(-this->doorClipSize.width / 2.0f, -this->doorClipSize.height / 2.0f), this->doorClipSize));

	this->indexLabel->enableOutline(Color4B::BLACK, 4);
	this->truthLabel->enableOutline(Color4B::BLACK, 4);
	this->hackableLabel->enableOutline(Color4B::BLACK, 4);

	this->indexLabel->enableShadow(Color4B(0, 0, 0, 128), CSize(2.0f, -2.0f));
	this->truthLabel->enableShadow(Color4B(0, 0, 0, 128), CSize(2.0f, -2.0f));
	this->hackableLabel->enableShadow(Color4B(0, 0, 0, 128), CSize(2.0f, -2.0f));

	this->indexLabel->setTextColor(PuzzleDoorBase::PassColor);
	this->truthLabel->setTextColor(PuzzleDoorBase::PassColor);
	this->hackableLabel->setTextColor(PuzzleDoorBase::FailColor);

	this->firstRun = true;
	this->puzzleIndex = 0;
	this->realValue = 0;
	this->hackValue = 0;

	for (int index = 0; index < PuzzleDoorBase::RuneCount; index++)
	{
		this->runesPassed[index]->setOpacity(0);
		this->runesFailed[index]->setOpacity(0);
	}

	this->lightLeft->setOpacity(0);
	this->lightRight->setOpacity(0);

	this->sliderSound->setCustomMultiplier(0.25f);
	this->sliderResetSound->setCustomMultiplier(0.25f);
	this->electricitySound->setCustomMultiplier(0.75f);

	this->contentNode->addChild(this->backNode);
	this->contentNode->addChild(this->barLeft);
	this->contentNode->addChild(this->barRight);
	this->contentNode->addChild(this->lightLeft);
	this->contentNode->addChild(this->lightRight);
	this->contentNode->addChild(this->doorClip);
	this->contentNode->addChild(this->frontNode);
	this->contentNode->addChild(this->indexLabel);
	this->contentNode->addChild(this->truthLabel);
	this->contentNode->addChild(this->hackableLabel);

	for (int index = 0; index < PuzzleDoorBase::RuneCount; index++)
	{
		this->contentNode->addChild(this->runes[index]);
		this->contentNode->addChild(this->runesFailed[index]);
		this->contentNode->addChild(this->runesPassed[index]);
	}

	this->contentNode->addChild(this->marker);
	this->addChild(this->electricitySound);
	this->addChild(this->sliderSound);
	this->addChild(this->sliderResetSound);
	this->addChild(this->doorOpenSound);
}

PuzzleDoorBase::~PuzzleDoorBase()
{
}

void PuzzleDoorBase::onEnter()
{
	super::onEnter();

	if (this->loadObjectStateOrDefault(PuzzleDoorBase::UnlockedSaveKey, Value(this->unlockedByDefault)).asBool())
	{
		this->unlock(false);
	}
	else
	{
		this->lock(false);
	}

	this->runAction(RepeatForever::create(
		Sequence::create(
			CallFunc::create([=]()
			{
				if (!this->isUnlocked)
				{
					// Increment counter (not on first run tho)
					if (firstRun)
					{
						firstRun = false;
					}
					else
					{
						this->puzzleIndex = MathUtils::wrappingNormalize(this->puzzleIndex + 1, 0, 3);
						this->runesFailed[this->puzzleIndex]->runAction(FadeTo::create(0.25f, 0));
						this->runesPassed[this->puzzleIndex]->runAction(FadeTo::create(0.25f, 0));

						if (this->puzzleIndex == 0)
						{
							this->sliderResetSound->play();
						}
						else
						{
							this->sliderSound->play();
						}
					}

					this->indexString->setString(std::to_string(this->puzzleIndex));

					this->marker->runAction(MoveTo::create(1.0f, this->runes[this->puzzleIndex]->getPosition() + Vec2(0.0f, 0.0f)));
					this->marker->runAction(FadeTo::create(0.25f, 255));
				}
				else
				{
					this->marker->runAction(FadeTo::create(0.25f, 0));
				}
			}),
			CallFunc::create([=]()
			{
				// Run light effect
				if (!this->isUnlocked)
				{
					this->electricitySound->play();
					this->lightLeft->setPosition(Vec2(-4.0f, 296.0f));
					this->lightRight->setPosition(Vec2(-8.0f, 296.0f));

					this->lightLeft->runAction(Sequence::create(
						FadeTo::create(0.0f, 255),
						MoveBy::create(0.4f, Vec2(-286.0f, 0.0f)),
						MoveBy::create(1.0f, Vec2(0.0f, -360.0f)),
						FadeTo::create(0.0f, 0),
						nullptr
					));

					this->lightRight->runAction(Sequence::create(
						FadeTo::create(0.0f, 255),
						MoveBy::create(0.4f, Vec2(286.0f, 0.0f)),
						MoveBy::create(1.0f, Vec2(0.0f, -360.0f)),
						FadeTo::create(0.0f, 0),
						nullptr
					));
				}
			}),
			DelayTime::create(1.5f),
			CallFunc::create([=]()
			{
				if (!this->isUnlocked)
				{
					this->runOperation(this->puzzleIndex);

					if (this->realValue == this->hackValue)
					{
						this->runeStates[this->puzzleIndex] = RuneState::Passed;
						this->hackableLabel->setTextColor(PuzzleDoorBase::PassColor);
						this->runesFailed[this->puzzleIndex]->runAction(FadeTo::create(0.25f, 0));
						this->runesPassed[this->puzzleIndex]->runAction(FadeTo::create(0.25f, 255));
					}
					else
					{
						this->runeStates[this->puzzleIndex] = RuneState::Failed;
						this->hackableLabel->setTextColor(PuzzleDoorBase::FailColor);
						this->runesPassed[this->puzzleIndex]->runAction(FadeTo::create(0.25f, 0));
						this->runesFailed[this->puzzleIndex]->runAction(FadeTo::create(0.25f, 255));
					}
				}
			}),
			DelayTime::create(1.5f),
			CallFunc::create([=]()
			{
				if (!this->isUnlocked)
				{
					for (int index = 0; index < PuzzleDoorBase::RuneCount; index++)
					{
						if (this->runeStates[index] != RuneState::Passed)
						{
							return;
						}
					}

					this->unlock();
				}
			}),
			nullptr
		)
	));
}

void PuzzleDoorBase::initializePositions()
{
	super::initializePositions();

	this->barLeft->setPosition(Vec2(-228.0f, 118.0f));
	this->barRight->setPosition(Vec2(220.0f, 118.0f));
	this->indexLabel->setPosition(this->indexPosition);
	this->truthLabel->setPosition(this->truthLabelPosition);
	this->hackableLabel->setPosition(this->hackLabelPosition);
	this->doorClip->setPosition(this->doorClipOffset);

	for (int index = 0; index < PuzzleDoorBase::RuneCount; index++)
	{
		const Vec2 basePosition = Vec2(-208.0f, 354.0f);

		this->runes[index]->setPosition(this->runeBasePosition + Vec2(float(index) * this->runeSpacing, 0.0f));
		this->runesPassed[index]->setPosition(this->runeBasePosition + Vec2(float(index) * this->runeSpacing, 0.0f));
		this->runesFailed[index]->setPosition(this->runeBasePosition + Vec2(float(index) * this->runeSpacing, 0.0f));
	}

	this->marker->setPosition(this->runes[0]->getPosition() + Vec2(0.0f, 0.0f));
}

void PuzzleDoorBase::initializeListeners()
{
	super::initializeListeners();
}

void PuzzleDoorBase::setRealValue(int value)
{
	this->realValue = value;

	this->truthString->setString(std::to_string(value));
}

void PuzzleDoorBase::setHackValue(int value)
{
	this->hackValue = value;

	this->hackableString->setString(std::to_string(value));
}

void PuzzleDoorBase::lock(bool animate)
{
	super::lock(animate);

	this->saveObjectState(PuzzleDoorBase::UnlockedSaveKey, Value(false));

	this->isUnlocked = false;

	this->indexLabel->runAction(FadeTo::create(0.25f, 255));
	this->hackableLabel->runAction(FadeTo::create(0.25f, 255));
	this->truthLabel->runAction(FadeTo::create(0.25f, 255));

	for (int index = 0; index < PuzzleDoorBase::RuneCount; index++)
	{
		this->runes[index]->runAction(FadeTo::create(0.25f, 255));
	}

	float currentProgress = this->doorNode->getPositionY() / this->doorOpenDelta;

	if (animate)
	{
		this->doorNode->stopAllActions();
		this->doorNode->runAction(MoveTo::create(5.0f * currentProgress, Vec2::ZERO));

		this->doorOpenSound->play();
	}
	else
	{
		this->doorNode->setPosition(Vec2::ZERO);
	}
}

void PuzzleDoorBase::unlock(bool animate)
{
	super::unlock(animate);

	this->saveObjectState(PuzzleDoorBase::UnlockedSaveKey, Value(true));

	this->isUnlocked = true;
	this->indexLabel->runAction(FadeTo::create(0.25f, 0));
	this->hackableLabel->runAction(FadeTo::create(0.25f, 0));
	this->truthLabel->runAction(FadeTo::create(0.25f, 0));

	for (int index = 0; index < PuzzleDoorBase::RuneCount; index++)
	{
		this->runes[index]->runAction(FadeTo::create(0.25f, 0));
		this->runesPassed[index]->runAction(FadeTo::create(0.25f, 0));
		this->runesFailed[index]->runAction(FadeTo::create(0.25f, 0));
	}

	float currentProgress = 1.0f - this->doorNode->getPositionY() / this->doorOpenDelta;

	if (animate)
	{
		this->doorNode->stopAllActions();
		this->doorNode->runAction(MoveTo::create(5.0f * currentProgress, Vec2(0.0f, this->doorOpenDelta)));
		
		this->doorOpenSound->play();
	}
	else
	{
		this->doorNode->setPosition(Vec2(0.0f, this->doorOpenDelta));
	}
}

bool PuzzleDoorBase::getIsUnlocked()
{
	return this->isUnlocked;
}
