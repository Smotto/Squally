#pragma once

#include "Engine/Hackables/HackableObject.h"

namespace cocos2d
{
	class Sprite;
}

class ClickableNode;
class LocalizedLabel;

class LevelNode : public HackableObject
{
public:
	static LevelNode* create(const cocos2d::ValueMap& properties);

	cocos2d::Vec2 getPositionOffset();

	std::string nodeMapFile;

	static const std::string MapKey;

protected:
	LevelNode(const cocos2d::ValueMap& properties);
	virtual ~LevelNode();

private:
	typedef HackableObject super;
	void onEnter() override;
	void initializePositions() override;
	void initializeListeners() override;
	void setLockState();
	std::string getSaveKey();
	std::string getPreviousSaveKey();

	int levelIndex = 0;
	int groupIndex = 0;
	bool isGroupFinale = false;
	cocos2d::Sprite* disabledSprite = nullptr;
	ClickableNode* sprite = nullptr;
	LocalizedLabel* indexLabel = nullptr;

	static const std::string PropertyMapFile;
	static const std::string PropertyGroupIndex;
	static const std::string PropertyGroupFinale;
	static const std::string PropertyLevelIndex;
	static const std::string SaveKeyPrefix;
};
