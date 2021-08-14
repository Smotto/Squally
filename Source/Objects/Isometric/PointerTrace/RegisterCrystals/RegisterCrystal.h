#pragma once

#include "Objects/Isometric/PointerTrace/GridObject.h"

namespace cocos2d
{
	class Sprite;
}

class LocalizedLabel;
class LocalizedString;
class SmartAnimationSequenceNode;

class RegisterCrystal : public GridObject
{
protected:
	RegisterCrystal(cocos2d::ValueMap& properties);
	virtual ~RegisterCrystal();

	void onEnter() override;
	void initializePositions() override;
	void initializeListeners() override;
	void update(float dt) override;
	virtual void updateRegister(int value) = 0;
	virtual int getRegisterValue() = 0;
	void updateMemory(int writeValue);
	void buildString(LocalizedString* registerString);
	int getValueAtPtr();
	int getValue();
	int getOffset();

	static const std::string PropertyRegisterInstruction;
	static const std::string PropertyRegisterIsPointer;
	static const std::string PropertyRegisterOffset;
	static const std::string PropertyRegisterValue;

	cocos2d::Node* crystalContainerNode = nullptr;
	cocos2d::Node* crystalNode = nullptr;
	LocalizedString* assemblyString;

private:
	typedef GridObject super;

	int runInstruction();
	void buildInstructionString(LocalizedString* registerString, LocalizedString* instructionString, bool bindValue);
	void buildInstructionPtrString(LocalizedString* registerString, LocalizedString* instructionString, bool bindValue);

	int value = 0;
	int offset = 0;
	bool isPointer = 0;
	std::string instruction;

	cocos2d::Sprite* shadow = nullptr;
	LocalizedLabel* assemblyLabel = nullptr;
	SmartAnimationSequenceNode* shineFx = nullptr;
};
