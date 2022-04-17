#include "MapBase.h"

#include "cocos/2d/CCFastTMXTiledMap.h"
#include "cocos/2d/CCLayer.h"
#include "cocos/2d/CCSprite.h"
#include "cocos/base/CCDirector.h"
#include "cocos/base/CCEventCustom.h"
#include "cocos/base/CCEventListenerCustom.h"

#include "Engine/Camera/GameCamera.h"
#include "Engine/Deserializers/LayerDeserializer.h"
#include "Engine/DeveloperMode/DeveloperModeController.h"
#include "Engine/Events/HackableEvents.h"
#include "Engine/Events/NavigationEvents.h"
#include "Engine/Events/SceneEvents.h"
#include "Engine/Hackables/Menus/CodeEditor/CodeHud.h"
#include "Engine/Hackables/Menus/RadialMenu.h"
#include "Engine/Optimization/LazyNode.h"
#include "Engine/Maps/GameMap.h"
#include "Engine/Utils/GameUtils.h"
#include "Engine/UI/HUD/Hud.h"
#include "Events/PlatformerEvents.h"
#include "Menus/Confirmation/ConfirmationMenu.h"
#include "Menus/MusicOverlay/MusicOverlay.h"
#include "Menus/Options/OptionsMenu.h"
#include "Menus/Pause/PlatformerPauseMenu.h"
#include "Menus/Pause/PauseMenu.h"
#include "Scenes/Platformer/Level/Backgrounds/MatrixRain/MatrixRain.h"

#include "Resources/BackgroundResources.h"

using namespace cocos2d;

std::map<std::string, cocos2d::cocos_experimental::TMXTiledMap*> MapBase::MapCache = std::map<std::string, cocos2d::cocos_experimental::TMXTiledMap*>();

MapBase::MapBase(bool allowHackerMode)
{
	CSize visibleSize = Director::getInstance()->getVisibleSize();

	this->allowHackerMode = allowHackerMode;
	this->mapNode = Node::create();
	this->musicOverlay = MusicOverlay::create();
	this->radialMenu = LazyNode<RadialMenu>::create(CC_CALLBACK_0(MapBase::buildRadialMenu, this));
	this->codeHud = LazyNode<CodeHud>::create(CC_CALLBACK_0(MapBase::buildCodeHud, this));
	this->optionsMenu = LazyNode<OptionsMenu>::create(CC_CALLBACK_0(MapBase::buildOptionsMenu, this));
	this->hudNode = Node::create();
	this->hud = Hud::create();
	this->hackerModeVisibleHud = Hud::create();
	this->miniGameHud = Hud::create();
	this->backMenuHud = Hud::create();
	this->menuBackDrop = Hud::create();
	this->hackMenuHud = Hud::create();
	this->menuHud = Hud::create();
	this->topMenuHud = Hud::create();
	this->confirmationMenuHud = Hud::create();
	this->hackerModeGlow = Hud::create();
	this->hackerModeRain = MatrixRain::create();
	
	Sprite* glowFx = Sprite::create(BackgroundResources::Hacking_HackerModeBackground);
	glowFx->setAnchorPoint(Vec2::ZERO);

	this->hackerModeGlow->addChild(glowFx);
	this->hackerModeGlow->setAnchorPoint(Vec2::ZERO);

	this->hackerModeGlow->setVisible(false);
	this->hackerModeRain->setVisible(false);

	this->menuBackDrop->addChild(LayerColor::create(Color4B::BLACK, visibleSize.width, visibleSize.height));
	
	this->hackMenuHud->addChild(this->radialMenu);
	this->hackMenuHud->addChild(this->codeHud);
	this->topMenuHud->addChild(this->optionsMenu);
	this->addChild(this->hackerModeRain);
	this->addChild(this->mapNode);
	this->addChild(this->hudNode);
	this->addChild(this->hud);
	this->addChild(this->hackerModeVisibleHud);
	this->addChild(this->hackerModeGlow);
	this->addChild(this->miniGameHud);
	this->addChild(this->musicOverlay);
	this->addChild(this->backMenuHud);
	this->addChild(this->menuBackDrop);
	this->addChild(this->menuHud);
	this->addChild(this->hackMenuHud);
	this->addChild(this->topMenuHud);
	this->addChild(this->confirmationMenuHud);
}

MapBase::~MapBase()
{
}

void MapBase::onEnter()
{
	super::onEnter();

	this->menuBackDrop->setOpacity(0);

	if (this->map != nullptr)
	{
		GameCamera::getInstance()->setMapBounds(CRect(0.0f, 0.0f, this->map->getMapSize().width, this->map->getMapSize().height));
	}
}

void MapBase::initializeListeners()
{
	super::initializeListeners();
	
	this->addEventListenerIgnorePause(EventListenerCustom::create(SceneEvents::EventBeforeSceneChange, [=](EventCustom* eventCustom)
	{
		this->canPause = false;
	}));
	
	this->addEventListenerIgnorePause(EventListenerCustom::create(SceneEvents::EventAfterSceneChange, [=](EventCustom* eventCustom)
	{
		this->canPause = true;
	}));
	
	this->addEventListenerIgnorePause(EventListenerCustom::create(PlatformerEvents::EventAllowPause, [=](EventCustom* eventCustom)
	{
		this->canPause = true;
	}));

	this->addEventListenerIgnorePause(EventListenerCustom::create(PlatformerEvents::EventDisallowPause, [=](EventCustom* eventCustom)
	{
		this->canPause = false;
	}));

	this->addEventListenerIgnorePause(EventListenerCustom::create(PlatformerEvents::EventQueryMapArgs, [=](EventCustom* eventCustom)
	{
		PlatformerEvents::QueryMapArgsArgs* args = static_cast<PlatformerEvents::QueryMapArgsArgs*>(eventCustom->getData());

		if (args != nullptr && args->argRef != nullptr)
		{
			for (auto next : this->mapArgs)
			{
				args->argRef->push_back(next);
			}
		}
	}));

	this->addEventListenerIgnorePause(EventListenerCustom::create(HackableEvents::EventHackerModeToggle, [=](EventCustom* eventCustom)
	{
		this->toggleHackerMode(eventCustom->getData());
	}));

	this->addEventListenerIgnorePause(EventListenerCustom::create(HackableEvents::EventAllowHackerMode, [=](EventCustom*)
	{
		this->allowHackerMode = true;
	}));

	this->addEventListenerIgnorePause(EventListenerCustom::create(HackableEvents::EventDisallowHackerMode, [=](EventCustom*)
	{
		this->allowHackerMode = false;
	}));

	this->addEventListenerIgnorePause(EventListenerCustom::create(HackableEvents::EventQueryHackerModeAllowed, [=](EventCustom* eventCustom)
	{
		HackerModeQueryArgs* args = static_cast<HackerModeQueryArgs*>(eventCustom->getData());

		if (args != nullptr)
		{
			args->hackerModeAllowed = this->allowHackerMode;
		}
	}));

	this->whenKeyPressed({ InputEvents::KeyCode::KEY_ESCAPE }, [=](InputEvents::KeyboardEventArgs* args)
	{
		if (!this->canPause ||!GameUtils::isFocused(this))
		{
			return;
		}
		
		args->handle();

		this->openPauseMenu(this);
	});
}

void MapBase::addLayerDeserializer(LayerDeserializer* layerDeserializer)
{
	this->addChild(layerDeserializer);
	this->layerDeserializers.push_back(layerDeserializer);
}

void MapBase::addLayerDeserializers(std::vector<LayerDeserializer*> layerDeserializers)
{
	for (auto next : layerDeserializers)
	{
		this->addChild(next);
		this->layerDeserializers.push_back(next);
	}
}

bool MapBase::loadMap(std::string mapResource)
{
	this->mapResource = mapResource;

	// No map caching for dev builds to allow for hot reloads of TMX files for faster debugging
	if (DeveloperModeController::IsDeveloperBuild)
	{
		return this->loadMapFromTmx(this->mapResource, GameMap::parse(this->mapResource));
	}

	if (MapBase::MapCache.find(mapResource) == MapBase::MapCache.end())
	{
		MapBase::MapCache[this->mapResource] = GameMap::parse(this->mapResource);
		MapBase::MapCache[this->mapResource]->retain();
	}	
	
	return this->loadMapFromTmx(this->mapResource, MapBase::MapCache[this->mapResource]);
}

bool MapBase::loadMapFromTmx(std::string mapResource, cocos_experimental::TMXTiledMap* mapRaw)
{
	if (this->map != nullptr)
	{
		this->mapNode->removeChild(this->map);
	}

	this->mapResource = mapResource;
	this->map = GameMap::deserialize(this->mapResource, mapRaw, this->layerDeserializers);
	
	if (this->map != nullptr)
	{
		this->mapNode->addChild(this->map);
		GameCamera::getInstance()->setMapBounds(CRect(0.0f, 0.0f, this->map->getMapSize().width, this->map->getMapSize().height));

		return true;
	}

	return false;
}

void MapBase::onDeveloperModeEnable(int debugLevel)
{
	if (this->map != nullptr)
	{
		this->map->setCollisionLayersVisible(true);
	}
}

void MapBase::onDeveloperModeDisable()
{
	if (this->map != nullptr)
	{
		this->map->setCollisionLayersVisible(false);
	}
}

void MapBase::onHackerModeEnable()
{
	super::onHackerModeEnable();

	GameUtils::pause(this);
	GameUtils::resume(this->hackerModeVisibleHud);

	// Good chance the user will use these soon, lazy init it so it can listen for events
	this->radialMenu->lazyGet();
	this->codeHud->lazyGet();

	this->hud->setVisible(false);
	this->hackerModeGlow->setVisible(true);
	this->hackerModeRain->setVisible(true);
}

void MapBase::onHackerModeDisable()
{
	super::onHackerModeDisable();

	this->hud->setVisible(true);
	this->hackerModeGlow->setVisible(false);
	this->hackerModeRain->setVisible(false);

	GameUtils::resume(this);
}

void MapBase::toggleHackerMode(void* userData)
{
	if (!this->allowHackerMode)
	{
		return;
	}

	if (!this->hackermodeEnabled)
	{
		HackToggleArgs args = *static_cast<HackToggleArgs*>(userData);
		
		HackableEvents::TriggerHackerModeEnable(args);
	}
	else
	{
		HackableEvents::TriggerHackerModeDisable();
	}
}

void MapBase::openPauseMenu(Node* refocusTarget)
{
	if (!this->canPause)
	{
		return;
	}
	
	this->menuBackDrop->setOpacity(196);
}

CodeHud* MapBase::buildCodeHud()
{
	CodeHud* instance = CodeHud::create();

	return instance;
}

RadialMenu* MapBase::buildRadialMenu()
{
	RadialMenu* instance = RadialMenu::create();

	return instance;
}

OptionsMenu* MapBase::buildOptionsMenu()
{
	OptionsMenu* instance = OptionsMenu::create(false);

	instance->setBackClickCallback([=]()
	{
		instance->setVisible(false);
		this->openPauseMenu(this);
	});

	return instance;
}
