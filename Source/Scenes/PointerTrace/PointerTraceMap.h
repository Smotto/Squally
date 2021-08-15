#pragma once
#include <set>

#include "Events/PointerTraceEvents.h"
#include "Scenes/MapBase.h"

class MemoryGrid;
class Music;
class PointerTraceHud;
class VictoryMenu;

class PointerTraceMap : public MapBase
{
public:
	static PointerTraceMap* create(std::string mapFile, std::function<void()> onLevelClearCallback);

protected:
	PointerTraceMap(std::string mapFile, std::function<void()> onLevelClearCallback);
	virtual ~PointerTraceMap();

	void initializePositions() override;
	void initializeListeners() override;
	void onDeveloperModeEnable(int debugLevel) override;
	void onDeveloperModeDisable() override;
	void onEnter() override;
	void update(float dt) override;

private:
	typedef MapBase super;
	
	void buildCollisionMaps();
	void initializeGridObjects();
	void resetState();
	void tryResumeMovement(PointerTraceEvents::PointerTraceRequestMovementArgs args);
	void moveGridEntity(PointerTraceEvents::PointerTraceRequestMovementArgs args);
	void doSegfault();
	void openVictoryMenu();

	std::function<void()> onLevelClearCallback = nullptr;
	std::set<int> collisionMap;
	std::set<int> segfaultMap;
	MemoryGrid* memoryGrid = nullptr;
	cocos2d::Node* collisionDebugNode = nullptr;
	PointerTraceHud* pointerTraceHud = nullptr;
	VictoryMenu* victoryMenu = nullptr;
};
