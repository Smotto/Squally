#pragma once

#include "Objects/Platformer/Interactables/Doors/Portal.h"

namespace cocos2d
{
	class Sprite;
}

class CollisionObject;
class ConstantString;
class InteractMenu;
class LocalizedLabel;
class SmartAnimationSequenceNode;
class SmartClippingNode;
class WorldSound;

class PuzzleDoorBase : public Portal
{
public:
	void lock(bool animate = true) override;
	void unlock(bool animate = true) override;

	bool getIsUnlocked();

	static const std::string PropertyUnlockedByDefault;

protected:
	PuzzleDoorBase(cocos2d::ValueMap& properties,
		cocos2d::Size doorClipSize,
		cocos2d::Vec2 doorClipOffset,
		cocos2d::Vec2 portalOffset,
		cocos2d::Vec2 indexPosition,
		cocos2d::Vec2 hackLabelPosition,
		cocos2d::Vec2 truthLabelPosition,
		cocos2d::Vec2 runeBasePosition,
		float runeSpacing,
		float doorOpenDelta);
	virtual ~PuzzleDoorBase();

	void onEnter() override;
	void initializePositions() override;
	void initializeListeners() override;
	virtual void runOperation(int puzzleIndex) = 0;
	void setRealValue(int value);
	void setHackValue(int value);

	cocos2d::Node* backNode;
	cocos2d::Node* doorNode;
	SmartClippingNode* doorClip;
	cocos2d::Node* frontNode;
	WorldSound* doorOpenSound;
	WorldSound* sliderSound;
	WorldSound* sliderResetSound;
	WorldSound* electricitySound;

	static const std::string UnlockedSaveKey;

private:
	typedef Portal super;
	cocos2d::Sprite* barLeft;
	cocos2d::Sprite* barRight;
	cocos2d::Sprite* lightLeft;
	cocos2d::Sprite* lightRight;
	cocos2d::Sprite* marker;
	std::vector<cocos2d::Sprite*> runes;
	std::vector<cocos2d::Sprite*> runesPassed;
	std::vector<cocos2d::Sprite*> runesFailed;
	ConstantString* indexString;
	ConstantString* truthString;
	ConstantString* hackableString;
	LocalizedLabel* indexLabel;
	LocalizedLabel* truthLabel;
	LocalizedLabel* hackableLabel;

	// Positioning parameters
	cocos2d::Size doorClipSize;
	cocos2d::Vec2 doorClipOffset;
	cocos2d::Vec2 portalOffset;
	cocos2d::Vec2 indexPosition;
	cocos2d::Vec2 hackLabelPosition;
	cocos2d::Vec2 truthLabelPosition;
	cocos2d::Vec2 runeBasePosition;
	float runeSpacing;
	float doorOpenDelta;

	enum RuneState
	{
		Unset,
		Passed,
		Failed
	};

	bool unlockedByDefault;
	bool firstRun;
	bool isUnlocked;
	RuneState runeStates[4];
	int puzzleIndex;
	int realValue;
	int hackValue;

	static const int RuneCount;
	static const cocos2d::Color4B PassColor;
	static const cocos2d::Color4B FailColor;
};
