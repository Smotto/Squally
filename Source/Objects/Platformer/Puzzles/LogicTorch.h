#pragma once

#include "Objects/Platformer/Interactables/InteractObject.h"

namespace cocos2d
{
	class Sprite;
};

class SmartAnimationSequenceNode;
class WorldSound;

class LogicTorch : public InteractObject
{
public:
	enum class TorchColor
	{
		Red,
		Blue,
		Green,
		Purple
	};

	enum class Operation
	{
		None,
		And,
		Or,
		Xor
	};

	static LogicTorch* create(cocos2d::ValueMap& properties);

	static TorchColor StrToColor(std::string colorName);
	static Operation StrToOperation(std::string operationName);

	bool isTorchOn();
	void torchOn(bool playSfx = true);
	void torchOff(bool playSfx = true);

	static const std::string MapKey;
	static const std::string MapEventTorchLogicSwitchSavePrefix;
	static const std::string MapEventTorchLogicSwitchPrefix;
	static const std::string MapEventSolveTorches;
	static const std::string MapEventCheckComplete;
	static const std::string PropertyColor;
	static const std::string PropertyOperation;
	static const std::string SaveKeyIsSolved;

protected:
	LogicTorch(cocos2d::ValueMap& properties);
	virtual ~LogicTorch();

	void onEnter() override;
	void initializePositions() override;
	void update(float dt) override;
	void onInteract(PlatformerEntity* interactingEntity) override;

private:
	typedef InteractObject super;

	bool isSolved();
	void updateLogicTorchVisibility();

	cocos2d::Sprite* torch;
	cocos2d::Sprite* glow;
	SmartAnimationSequenceNode* fire;
	WorldSound* burnSound;
	WorldSound* onSound;
	WorldSound* offSound;
	WorldSound* interactSound;

	float cooldown;
	bool isOn;
	TorchColor color;
	Operation operation;
	std::string colorName;
	std::string operationName;
	std::string saveKey;

	static const std::string PropertyIsOn;
	static const std::string PropertyIsInteractable;
	static const std::string PropertySaveKey;
};
