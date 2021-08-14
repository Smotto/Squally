#pragma once

#include "Scenes/Cipher/Components/CipherComponentBase.h"

namespace cocos2d
{
	class Sprite;
}

class CipherState;
class ClickableNode;
class SmartAsciiLabel;

class InputOutputItem : public CipherComponentBase
{
public:
	static InputOutputItem* create(unsigned char input, unsigned char output, std::function<void(InputOutputItem*)> selectCallback);

	void enableInteraction();
	void disableInteraction();
	void setStatusPassed();
	void setStatusFailed();
	void clearStatus();

protected:
	InputOutputItem(unsigned char input, unsigned char output, std::function<void(InputOutputItem*)> selectCallback);
	virtual ~InputOutputItem();
	
	void onEnter() override;
	void initializePositions() override;
	void initializeListeners() override;
	void onAnyStateChange(CipherState* cipherState) override;

private:
	typedef CipherComponentBase super;

	ClickableNode* panel = nullptr;
	SmartAsciiLabel* inputLabel = nullptr;
	SmartAsciiLabel* outputLabel = nullptr;
	cocos2d::Sprite* failedIcon = nullptr;
	cocos2d::Sprite* passedIcon = nullptr;

	unsigned char input = 0;
	unsigned char output = 0;
	std::function<void(InputOutputItem*)> selectCallback = nullptr;
};
