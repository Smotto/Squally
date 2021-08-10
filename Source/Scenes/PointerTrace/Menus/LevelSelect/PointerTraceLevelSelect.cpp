#include "PointerTraceLevelSelect.h"

#include "cocos/2d/CCActionEase.h"
#include "cocos/2d/CCSprite.h"
#include "cocos/base/CCEventCustom.h"
#include "cocos/base/CCEventListenerCustom.h"
#include "cocos/base/CCInputEvents.h"
#include "cocos/base/CCDirector.h"
#include "cocos/base/CCValue.h"

#include "Deserializers/Isometric/IsometricDecorDeserializer.h"
#include "Deserializers/Isometric/IsometricEntityDeserializer.h"
#include "Deserializers/Isometric/IsometricObjectDeserializer.h"
#include "Engine/Camera/GameCamera.h"
#include "Engine/Deserializers/Meta/BackgroundDeserializer.h"
#include "Engine/Deserializers/Meta/MetaLayerDeserializer.h"
#include "Engine/Deserializers/Meta/MusicDeserializer.h"
#include "Engine/Deserializers/Objects/ObjectLayerDeserializer.h"
#include "Engine/Events/NavigationEvents.h"
#include "Engine/GlobalDirector.h"
#include "Engine/Sound/Music.h"
#include "Engine/UI/HUD/Hud.h"
#include "Engine/UI/Mouse.h"
#include "Engine/Utils/GameUtils.h"

#include "Resources/IsometricMapResources.h"
#include "Resources/MusicResources.h"
#include "Resources/PointerTraceResources.h"
#include "Resources/UIResources.h"

using namespace cocos2d;

PointerTraceLevelSelect* PointerTraceLevelSelect::instance = nullptr;

PointerTraceLevelSelect* PointerTraceLevelSelect::getInstance()
{
	if (PointerTraceLevelSelect::instance == nullptr)
	{
		PointerTraceLevelSelect::instance = new PointerTraceLevelSelect();

		PointerTraceLevelSelect::instance->autorelease();
		PointerTraceLevelSelect::instance->initializeListeners();

		GlobalDirector::RegisterGlobalScene(PointerTraceLevelSelect::instance);
	}

	return PointerTraceLevelSelect::instance;
}

PointerTraceLevelSelect::PointerTraceLevelSelect() : super(false)
{
	this->addLayerDeserializers({
			MetaLayerDeserializer::create(
			{
				BackgroundDeserializer::create(),
				MusicDeserializer::create(),
			}),
			ObjectLayerDeserializer::create({
				{ IsometricDecorDeserializer::MapKeyTypeDecor, IsometricDecorDeserializer::create() },
				{ IsometricEntityDeserializer::MapKeyTypeEntity, IsometricEntityDeserializer::create() },
				{ IsometricObjectDeserializer::MapKeyTypeObject, IsometricObjectDeserializer::create() },
			})
		}
	);

	this->loadMap(IsometricMapResources::LevelSelectMap);
}

PointerTraceLevelSelect::~PointerTraceLevelSelect()
{
}

void PointerTraceLevelSelect::onEnter()
{
	super::onEnter();

	const float delay = 0.5f;
	const float duration = 0.75f;
	CSize visibleSize = Director::getInstance()->getVisibleSize();

	if (this->map == nullptr)
	{
		this->loadMap(IsometricMapResources::LevelSelectMap);
	}
}

void PointerTraceLevelSelect::initializePositions()
{
	super::initializePositions();

	CSize visibleSize = Director::getInstance()->getVisibleSize();
}

void PointerTraceLevelSelect::initializeListeners()
{
	super::initializeListeners();

	this->whenKeyPressed({ InputEvents::KeyCode::KEY_ESCAPE }, [=](InputEvents::KeyboardEventArgs* args)
	{
		if (!GameUtils::isVisible(this))
		{
			return;
		}

		args->handle();
		// NavigationEvents::NavigateBack();
	});
}
