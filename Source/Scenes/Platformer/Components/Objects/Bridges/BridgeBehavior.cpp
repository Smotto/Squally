#include "BridgeBehavior.h"

#include "cocos/2d/CCActionEase.h"
#include "cocos/2d/CCActionInstant.h"
#include "cocos/2d/CCActionInterval.h"
#include "cocos/base/CCEventCustom.h"
#include "cocos/base/CCEventListenerCustom.h"
#include "cocos/base/CCValue.h"

#include "Engine/Terrain/TerrainObject.h"
#include "Engine/Maps/GameObject.h"
#include "Engine/Sound/WorldSound.h"
#include "Engine/Utils/GameUtils.h"
#include "Events/PlatformerEvents.h"

#include "Resources/SoundResources.h"

using namespace cocos2d;

const std::string BridgeBehavior::MapKey = "bridge";
const std::string BridgeBehavior::PropertyGroup = "bridge-group";
const std::string BridgeBehavior::PropertyBridgeIndex = "bridge-index";
const std::string BridgeBehavior::PropertyDelta = "bridge-delta";
const std::string BridgeBehavior::PropertySpeed = "bridge-speed";
const std::string BridgeBehavior::PropertyAudioMode = "bridge-audio-mode";
const std::string BridgeBehavior::SaveKeyRaised = "SAVE_KEY_RAISED";
const float BridgeBehavior::DefaultDelta = -512.0f;
const float BridgeBehavior::DefaultSpeed = 0.25f;

BridgeBehavior* BridgeBehavior::create(GameObject* owner)
{
	BridgeBehavior* instance = new BridgeBehavior(owner);

	instance->autorelease();

	return instance;
}

BridgeBehavior::BridgeBehavior(GameObject* owner) : super(owner)
{
	this->object = owner;
	this->group = "";
	this->bridgeIndex = 0;
	this->raiseSound = WorldSound::create(SoundResources::Platformer_FX_Woosh_WooshPulse1);
	this->bridgeDelta = GameUtils::getKeyOrDefault(this->object->properties, BridgeBehavior::PropertyDelta, Value(BridgeBehavior::DefaultDelta)).asFloat();
	this->bridgeSpeed = GameUtils::getKeyOrDefault(this->object->properties, BridgeBehavior::PropertySpeed, Value(BridgeBehavior::DefaultSpeed)).asFloat();

	this->raiseSound->disableZDepth();

	std::string audioModeStr = GameUtils::getKeyOrDefault(this->object->properties, BridgeBehavior::PropertyAudioMode, Value("post")).asString();

	if (audioModeStr == "none")
	{
		this->audioMode = AudioMode::None;
	}
	else if (audioModeStr == "pre")
	{
		this->audioMode = AudioMode::Pre;
	}
	else
	{
		this->audioMode = AudioMode::Post;
	}

	if (this->object == nullptr)
	{
		this->invalidate();
	}
	else
	{
		this->group = GameUtils::getKeyOrDefault(this->object->properties, BridgeBehavior::PropertyGroup, Value("")).asString();
		this->bridgeIndex = GameUtils::getKeyOrDefault(this->object->properties, BridgeBehavior::PropertyBridgeIndex, Value(0)).asInt();
	}

	this->addChild(this->raiseSound);
}

BridgeBehavior::~BridgeBehavior()
{
}

void BridgeBehavior::onLoad()
{
	if (this->object->loadObjectStateOrDefault(BridgeBehavior::SaveKeyRaised, Value(false)).asBool())
	{
		return;
	}

	TerrainObject* terrain = dynamic_cast<TerrainObject*>(this->object);

	if (terrain != nullptr)
	{
		// If it is terrain we need to set it as dynamic for culling reasons. It normally caches the spawn position otherwise.
		terrain->setDynamic(true);
	}

	this->originalPosition = this->object->getPosition();
	this->object->setPositionY(this->originalPosition.y + this->bridgeDelta);

	this->object->listenForMapEvent(this->group, [=](ValueMap args)
	{
		this->raiseBridge(false);
	});
}

void BridgeBehavior::onDisable()
{
	super::onDisable();
}

void BridgeBehavior::raiseBridge(bool isInit)
{
	float delayPrev = this->bridgeSpeed * float(this->bridgeIndex) - this->bridgeSpeed / 2.0f;
	float delay = this->bridgeSpeed * float(this->bridgeIndex + 1) - this->bridgeSpeed / 2.0f;

	if (!isInit)
	{
		this->object->saveObjectState(BridgeBehavior::SaveKeyRaised, Value(true));
	}

	this->object->runAction(MoveTo::create(isInit ? 0.0f : delay, originalPosition));

	if (!isInit)
	{
		switch(this->audioMode)
		{
			case AudioMode::Pre:
			{
				this->object->runAction(Sequence::create(
					DelayTime::create(delayPrev <= 0.0f ? 0.0f : delayPrev),
					CallFunc::create([=]()
					{
						this->raiseSound->play();
					}),
					nullptr
				));

				break;
			}
			case AudioMode::Post:
			{
				this->object->runAction(Sequence::create(
					DelayTime::create(delay),
					CallFunc::create([=]()
					{
						this->raiseSound->play();
					}),
					nullptr
				));
				
				break;
			}
			default:
			case AudioMode::None:
			{
				break;
			}
		}
	}
}
