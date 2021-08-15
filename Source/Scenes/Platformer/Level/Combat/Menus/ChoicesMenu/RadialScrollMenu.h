#pragma once

#include "Engine/SmartNode.h"

namespace cocos2d
{
	class Sprite;
}

class RadialEntry;
class LocalizedString;
class Sound;

class RadialScrollMenu : public SmartNode
{
public:
	static RadialScrollMenu* create(float radius, float angleDelta = float(M_PI) / 6.0f);

	int getIndex();
	void clearItems();
	RadialEntry* addEntry(LocalizedString* labelStr, std::vector<LocalizedString*> lowerLabelStrs, std::string iconResource, std::string backgroundResource, std::function<void()> callback);
	void toggleAll(bool disableInteraction = true, bool fadeOpacity = false, bool hideText = true);
	void enableAll();
	void focus();
	void unfocus();
	void scrollUp();
	void scrollDown();
	void scrollTo(int index);
	void setBackCallback(std::function<void()> backCallback);

protected:
	RadialScrollMenu(float radius, float angleDelta = float(M_PI) / 6.0f);
	virtual ~RadialScrollMenu();
	
	void initializePositions() override;
	void initializeListeners() override;

private:
	typedef SmartNode super;

	void positionButtons();
	void goBack();

	std::vector<RadialEntry*> buttons;
	cocos2d::Sprite* arrow = nullptr;
	cocos2d::Node* buttonsNode = nullptr;
	Sound* indexChangeSound = nullptr;
	Sound* errorSound = nullptr;
	float radius = 0.0f;
	float angleDelta = 0.0f;

	int currentIndex = 0;
	bool focused = true;
	bool isTimelineCinematicPaused = false;
	
	std::function<void()> backCallback = nullptr;
};
