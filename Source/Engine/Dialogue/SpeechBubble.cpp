#include "SpeechBubble.h"

#include "cocos/2d/CCActionInstant.h"
#include "cocos/2d/CCActionInterval.h"
#include "cocos/2d/CCDrawNode.h"

#include "Engine/Camera/GameCamera.h"
#include "Engine/Events/ObjectEvents.h"
#include "Engine/Localization/LocalizedLabel.h"
#include "Engine/Localization/LocalizedString.h"
#include "Engine/Sound/WorldSound.h"
#include "Engine/UI/FX/TypeWriterEffect.h"

#include "Strings/Strings.h"

using namespace cocos2d;

const float SpeechBubble::InfiniteDuration = -1.0f;
const Color4F SpeechBubble::BubbleColor = Color4F(Color4B(189, 215, 221, 196));
const Color4F SpeechBubble::BubbleEdgeColor = Color4F(Color4B(47, 71, 78, 196));
const Color4B SpeechBubble::BubbleTextColor = Color4B(47, 71, 78, 255);
const float SpeechBubble::BubbleBorderSize = 3.0f;

SpeechBubble* SpeechBubble::create(bool uiBound)
{
	SpeechBubble* instance = new SpeechBubble(uiBound);

	instance->autorelease();

	return instance;
}

SpeechBubble::SpeechBubble(bool uiBound)
{
	this->uiBound = uiBound;
	this->stem = DrawNode::create(3.0f);
	this->bubble = DrawNode::create(3.0f);
	this->voiceSound = WorldSound::create("");
	this->text = LocalizedLabel::create(LocalizedLabel::FontStyle::Main, LocalizedLabel::FontSize::P, Strings::Common_Empty::create());
	this->speechBubbleNode = Node::create();

	this->text->setTextColor(SpeechBubble::BubbleTextColor);

	this->bubble->setOpacity(0);
	this->stem->setOpacity(0);
	this->text->setOpacity(0);

	this->speechBubbleNode->addChild(this->stem);
	this->speechBubbleNode->addChild(this->bubble);
	this->speechBubbleNode->addChild(this->text);
	this->addChild(this->voiceSound);
	this->addChild(this->speechBubbleNode);
}

SpeechBubble::~SpeechBubble()
{
}

void SpeechBubble::initializePositions()
{
	super::initializePositions();

	const float offsetY = 32.0f;

	this->bubble->setPositionY(offsetY);
	this->stem->setPositionY(offsetY);
	this->text->setPositionY(offsetY);
}

void SpeechBubble::initializeListeners()
{
	super::initializeListeners();
}

void SpeechBubble::runDialogue(LocalizedString* localizedString, std::string soundResource, float sustainDuration, std::function<void()> onComplete, Direction direction, bool instant)
{
	const CSize padding = CSize(16.0f, 16.0f);
	const float centerAutoOffset = 256.0f;
	const CSize triangleSize = CSize(16.0f, 32.0f);

	this->isDialogueRunning = true;
	this->voiceSound->setSoundResource(soundResource);

	if (!this->hasBound && this->uiBound)
	{
		ObjectEvents::TriggerBindObjectToUI(RelocateObjectArgs(this->speechBubbleNode));
		hasBound = true;
	}

	if (direction == Direction::Auto)
	{
		Vec2 cameraPosition = GameCamera::getInstance()->getCameraPosition();
		Vec2 thisPosition = -this->getParent()->convertToNodeSpace(this->getPosition());

		if (cameraPosition.x > thisPosition.x - centerAutoOffset && cameraPosition.x < thisPosition.x + centerAutoOffset)
		{
			direction = Direction::Centered;
		}
		else if (cameraPosition.x < thisPosition.x)
		{
			direction = Direction::ExpandLeft;
		}
		else
		{
			direction = Direction::ExpandRight;
		}
	}

	this->bubble->runAction(FadeTo::create(0.5f, 255));
	this->stem->runAction(FadeTo::create(0.5f, 255));
	this->text->runAction(FadeTo::create(0.5f, 255));

	this->text->setLocalizedString(localizedString);
	this->text->setDimensions(320.0f, 0.0f);

	if (!instant)
	{
		TypeWriterEffect::runTypeWriterEffect(this, this->text);
	}

	CSize textSize = this->text->getContentSize();
	std::vector<Vec2> trianglePoints = std::vector<Vec2>();
	Vec2 source;
	Vec2 dest;

	switch(direction)
	{
		default:
		case Direction::Centered:
		{
			this->text->setAnchorPoint(Vec2(0.5f, 0.0f));

			source = Vec2(-textSize.width / 2.0f - padding.width, -padding.height);
			dest = Vec2(textSize.width / 2.0f + padding.width, textSize.height + padding.height);

			trianglePoints.push_back(Vec2(-triangleSize.width / 2.0f, -padding.height));
			trianglePoints.push_back(Vec2(0.0f, -padding.height - triangleSize.height));
			trianglePoints.push_back(Vec2(triangleSize.width / 2.0f, -padding.height));
			break;
		}
		case Direction::ExpandLeft:
		{
			this->text->setAnchorPoint(Vec2(1.0f, 0.0f));

			source = Vec2(-textSize.width - padding.width, -padding.height);
			dest = Vec2(padding.width, textSize.height + padding.height);

			trianglePoints.push_back(Vec2(-triangleSize.width, -padding.height));
			trianglePoints.push_back(Vec2(0.0f, -padding.height - triangleSize.height));
			trianglePoints.push_back(Vec2(0.0f, -padding.height));
			break;
		}
		case Direction::ExpandRight:
		{
			this->text->setAnchorPoint(Vec2(0.0f, 0.0f));

			source = Vec2(-padding.width, -padding.height);
			dest = Vec2(textSize.width + padding.width, textSize.height + padding.height);

			trianglePoints.push_back(Vec2(0.0f, -padding.height));
			trianglePoints.push_back(Vec2(0.0f, -padding.height - triangleSize.height));
			trianglePoints.push_back(Vec2(triangleSize.width, -padding.height));
			break;
		}
	}

	this->stem->clear();
	this->stem->drawTriangle(trianglePoints[0], trianglePoints[1], trianglePoints[2], SpeechBubble::BubbleColor);
	this->stem->drawPoly(trianglePoints.data(), trianglePoints.size(), false, SpeechBubble::BubbleEdgeColor);

	this->bubble->clear();
	this->bubble->drawSolidRect(source, dest, SpeechBubble::BubbleColor);
	this->bubble->drawRect(source, dest, SpeechBubble::BubbleEdgeColor);

	if (!this->voiceSound->isPlaying())
	{
		this->voiceSound->play();
	}

	if (sustainDuration >= 0.0f)
	{
		this->runAction(Sequence::create(
			DelayTime::create(sustainDuration),
			CallFunc::create([=]()
			{
				this->hideDialogue();

				if (onComplete != nullptr)
				{
					onComplete();
				}
			}),
			nullptr
		));
	}
}

bool SpeechBubble::isRunning()
{
	return this->isDialogueRunning;
}

void SpeechBubble::hideDialogue()
{
	TypeWriterEffect::cancelEffect(this->text);

	this->isDialogueRunning = false;
	this->bubble->runAction(FadeTo::create(0.5f, 0));
	this->stem->runAction(FadeTo::create(0.5f, 0));
	this->text->runAction(FadeTo::create(0.5f, 0));
}
