#include "MemoryEditingTab.h"

#include "cocos/2d/CCSprite.h"
#include "cocos/base/CCDirector.h"

#include "Engine/Config/ConfigManager.h"
#include "Engine/Events/NavigationEvents.h"
#include "Engine/Input/ClickableTextNode.h"
#include "Engine/Localization/LocalizedLabel.h"
#include "Engine/Save/SaveManager.h"
#include "Engine/UI/Controls/ScrollPane.h"
#include "Entities/Platformer/Enemies/BallmerPeaks/Cryogen.h"
#include "Entities/Platformer/Enemies/CastleValgrind/Agnes.h"
#include "Entities/Platformer/Enemies/DaemonsHallow/Asmodeus.h"
#include "Entities/Platformer/Enemies/DaemonsHallow/Krampus.h"
#include "Entities/Platformer/Enemies/LambdaCrypts/KingZul.h"
#include "Entities/Platformer/Enemies/LambdaCrypts/Lazarus.h"
#include "Entities/Platformer/Helpers/BallmerPeaks/Snowman.h"
#include "Entities/Platformer/Helpers/EndianForest/Guano.h"
#include "Menus/Tutorials/TutorialEntry.h"
#include "Menus/Tutorials/TutorialSelectMenu.h"
#include "Scenes/Platformer/Level/PlatformerMap.h"
#include "Scenes/Platformer/Save/SaveKeys.h"
#include "Scenes/Tutorials/Save/TutorialSaveKeys.h"

#include "Resources/MapResources.h"
#include "Resources/UIResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

MemoryEditingTab* MemoryEditingTab::create()
{
	MemoryEditingTab* instance = new MemoryEditingTab();

	instance->autorelease();

	return instance;
}

MemoryEditingTab::MemoryEditingTab()
{
	this->knownValueInt = this->createEntry(TutorialSaveKeys::SaveKeyKnownValueInt);
	this->knownValueFloat = this->createEntry(TutorialSaveKeys::SaveKeyKnownValueFloat, this->knownValueInt);
	this->unknownValueInt = this->createEntry(TutorialSaveKeys::SaveKeyUnknownValueInt, this->knownValueFloat);
	this->unknownValueFloat = this->createEntry(TutorialSaveKeys::SaveKeyUnknownValueFloat, this->unknownValueInt);
	this->knownValueDouble = this->createEntry(TutorialSaveKeys::SaveKeyKnownValueDouble, this->unknownValueFloat);
	this->unknownValueFloatAdvanced = this->createEntry(TutorialSaveKeys::SaveKeyKnownValueFloatAdvanced, this->knownValueDouble);

	ValueMap properties = ValueMap();
	this->kingZul = this->knownValueInt->addEntity(KingZul::deserialize(properties), Vec2(-16.0f, 48.0f));
	this->agnes = this->knownValueFloat->addEntity(Agnes::deserialize(properties), Vec2(-32.0f, 64.0f));
	this->lazarus = this->unknownValueInt->addEntity(Lazarus::deserialize(properties), Vec2(-8.0f, -24.0f));
	this->asmodeus = this->unknownValueFloat->addEntity(Asmodeus::deserialize(properties), Vec2(-32.0f, 24.0f));
	this->cryogen = this->knownValueDouble->addEntity(Cryogen::deserialize(properties), Vec2(-48.0f, 24.0f));
	this->krampus = this->unknownValueFloatAdvanced->addEntity(Krampus::deserialize(properties), Vec2(-24.0f, 16.0f));
}

MemoryEditingTab::~MemoryEditingTab()
{
}

void MemoryEditingTab::onEnter()
{
	super::onEnter();
}

void MemoryEditingTab::initializeListeners()
{
	super::initializeListeners();

	this->knownValueInt->setClickCallback([=]()
	{
		this->loadKnownValueIntTutorial();
	});

	this->knownValueFloat->setClickCallback([=]()
	{
		this->loadKnownValueFloatTutorial();
	});

	this->unknownValueInt->setClickCallback([=]()
	{
		this->loadUnknownValueIntTutorial();
	});

	this->unknownValueFloat->setClickCallback([=]()
	{
		this->loadUnknownValueFloatTutorial();
	});

	this->knownValueDouble->setClickCallback([=]()
	{
		this->loadKnownValueDoubleTutorial();
	});

	this->unknownValueFloatAdvanced->setClickCallback([=]()
	{
		this->loadUnknownValueFloatTutorialAdvanced();
	});
}

void MemoryEditingTab::initializePositions()
{
	super::initializePositions();
}

void MemoryEditingTab::loadKnownValueIntTutorial()
{
	NavigationEvents::LoadScene(NavigationEvents::LoadSceneArgs([=]()
	{
		const int UNUSED_SAVE_PROFILE = 99;

		SaveManager::deleteAllProfileData(UNUSED_SAVE_PROFILE);
		SaveManager::setActiveSaveProfile(UNUSED_SAVE_PROFILE);
		SaveManager::SoftSaveProfileData(SaveKeys::SaveKeySpellBookWind, Value(true));
		SaveManager::SoftSaveProfileData(SaveKeys::SaveKeySpellBookWater, Value(true));
		SaveManager::SoftSaveProfileData(SaveKeys::SaveKeyHelperName, Value(Guano::MapKey));
		SaveManager::SoftSaveProfileData(SaveKeys::SaveKeyScrappyFound, Value(true));
		SaveManager::SoftSaveProfileData(SaveKeys::SaveKeyRespawnMap, Value(MapResources::Tutorials_KnownValueInt32));
	
		PlatformerMap* map = PlatformerMap::create();

		map->loadMap(MapResources::Tutorials_KnownValueInt32);

		return map;
	}));
}

void MemoryEditingTab::loadKnownValueFloatTutorial()
{
	NavigationEvents::LoadScene(NavigationEvents::LoadSceneArgs([=]()
	{
		const int UNUSED_SAVE_PROFILE = 99;

		SaveManager::deleteAllProfileData(UNUSED_SAVE_PROFILE);
		SaveManager::setActiveSaveProfile(UNUSED_SAVE_PROFILE);
		SaveManager::SoftSaveProfileData(SaveKeys::SaveKeySpellBookWind, Value(true));
		SaveManager::SoftSaveProfileData(SaveKeys::SaveKeySpellBookWater, Value(true));
		SaveManager::SoftSaveProfileData(SaveKeys::SaveKeyHelperName, Value(Guano::MapKey));
		SaveManager::SoftSaveProfileData(SaveKeys::SaveKeyScrappyFound, Value(true));
		SaveManager::SoftSaveProfileData(SaveKeys::SaveKeyRespawnMap, Value(MapResources::Tutorials_KnownValueFloat));
		
		PlatformerMap* map = PlatformerMap::create();

		map->loadMap(MapResources::Tutorials_KnownValueFloat);

		return map;
	}));
}

void MemoryEditingTab::loadUnknownValueIntTutorial()
{
	NavigationEvents::LoadScene(NavigationEvents::LoadSceneArgs([=]()
	{
		const int UNUSED_SAVE_PROFILE = 99;

		SaveManager::deleteAllProfileData(UNUSED_SAVE_PROFILE);
		SaveManager::setActiveSaveProfile(UNUSED_SAVE_PROFILE);
		SaveManager::SoftSaveProfileData(SaveKeys::SaveKeySpellBookWind, Value(true));
		SaveManager::SoftSaveProfileData(SaveKeys::SaveKeySpellBookWater, Value(true));
		SaveManager::SoftSaveProfileData(SaveKeys::SaveKeyHelperName, Value(Guano::MapKey));
		SaveManager::SoftSaveProfileData(SaveKeys::SaveKeyScrappyFound, Value(true));
		SaveManager::SoftSaveProfileData(SaveKeys::SaveKeyRespawnMap, Value(MapResources::Tutorials_UnknownValueInt32));
		
		PlatformerMap* map = PlatformerMap::create();

		map->loadMap(MapResources::Tutorials_UnknownValueInt32);

		return map;
	}));
}

void MemoryEditingTab::loadUnknownValueFloatTutorial()
{
	NavigationEvents::LoadScene(NavigationEvents::LoadSceneArgs([=]()
	{
		const int UNUSED_SAVE_PROFILE = 99;

		SaveManager::deleteAllProfileData(UNUSED_SAVE_PROFILE);
		SaveManager::setActiveSaveProfile(UNUSED_SAVE_PROFILE);
		SaveManager::SoftSaveProfileData(SaveKeys::SaveKeySpellBookWind, Value(true));
		SaveManager::SoftSaveProfileData(SaveKeys::SaveKeySpellBookWater, Value(true));
		SaveManager::SoftSaveProfileData(SaveKeys::SaveKeyHelperName, Value(Guano::MapKey));
		SaveManager::SoftSaveProfileData(SaveKeys::SaveKeyScrappyFound, Value(true));
		SaveManager::SoftSaveProfileData(SaveKeys::SaveKeyRespawnMap, Value(MapResources::Tutorials_UnknownValueFloat));
		
		PlatformerMap* map = PlatformerMap::create();

		map->loadMap(MapResources::Tutorials_UnknownValueFloat);

		return map;
	}));
}

void MemoryEditingTab::loadKnownValueDoubleTutorial()
{
	NavigationEvents::LoadScene(NavigationEvents::LoadSceneArgs([=]()
	{
		const int UNUSED_SAVE_PROFILE = 99;

		SaveManager::deleteAllProfileData(UNUSED_SAVE_PROFILE);
		SaveManager::setActiveSaveProfile(UNUSED_SAVE_PROFILE);
		SaveManager::SoftSaveProfileData(SaveKeys::SaveKeySpellBookWind, Value(true));
		SaveManager::SoftSaveProfileData(SaveKeys::SaveKeySpellBookWater, Value(true));
		SaveManager::SoftSaveProfileData(SaveKeys::SaveKeyHelperName, Value(Guano::MapKey));
		SaveManager::SoftSaveProfileData(SaveKeys::SaveKeyScrappyFound, Value(true));
		SaveManager::SoftSaveProfileData(SaveKeys::SaveKeyRespawnMap, Value(MapResources::Tutorials_KnownValueDouble));
		
		PlatformerMap* map = PlatformerMap::create();

		map->loadMap(MapResources::Tutorials_KnownValueDouble);

		return map;
	}));
}

void MemoryEditingTab::loadUnknownValueFloatTutorialAdvanced()
{
	NavigationEvents::LoadScene(NavigationEvents::LoadSceneArgs([=]()
	{
		const int UNUSED_SAVE_PROFILE = 99;

		SaveManager::deleteAllProfileData(UNUSED_SAVE_PROFILE);
		SaveManager::setActiveSaveProfile(UNUSED_SAVE_PROFILE);
		SaveManager::SoftSaveProfileData(SaveKeys::SaveKeySpellBookWind, Value(true));
		SaveManager::SoftSaveProfileData(SaveKeys::SaveKeySpellBookWater, Value(true));
		SaveManager::SoftSaveProfileData(SaveKeys::SaveKeyHelperName, Value(Guano::MapKey));
		SaveManager::SoftSaveProfileData(SaveKeys::SaveKeyScrappyFound, Value(true));
		SaveManager::SoftSaveProfileData(SaveKeys::SaveKeyRespawnMap, Value(MapResources::Tutorials_KnownValueFloatAdvanced));
		
		PlatformerMap* map = PlatformerMap::create();

		map->loadMap(MapResources::Tutorials_KnownValueFloatAdvanced);

		return map;
	}));
}
