#pragma once
#include <set>

#include "Objects/Platformer/Interactables/Doors/Portal.h"

class LocalizedString;

class OptionWarp : public Portal
{
public:
	static OptionWarp* create(cocos2d::ValueMap& properties);
	
	std::string getStrKey();

	static const std::string MapKey;
	static const std::string PropertyWarpFrom;
	static const std::string PropertyWarpTo;
	static const std::string PropertyNoWarpCamera;
	static const std::string PropertyRelayer;
	static const std::string PropertyZoom;

protected:
	OptionWarp(cocos2d::ValueMap& properties);
	virtual ~OptionWarp();
	
	void onEnter() override;
	void initializePositions() override;
	void initializeListeners() override;
	void loadMap() override;

private:
	typedef Portal super;

	void resolveToStrKeys();
	void openDialogue();
	LocalizedString* getOptionString(std::string strKey);
	bool cancelOptionChoice();
	void doRelayer();
	void applyZoomOverride();

	bool canChooseOption = false;
	std::string from;
	std::string strKey;
	std::vector<std::string> to;
	std::vector<std::string> toStrKeys;
	bool warpCamera = false;
	bool relayer = false;
	float zoomOverride = 0.0f;

	static const std::string PropertyStrKey;
	static const std::string EventWarpToPrefix;
};
