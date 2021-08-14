#pragma once
#include <functional>

#include "Engine/SmartNode.h"

// forward declarations
namespace cocos2d
{
	class Sprite;
}

class SmartAnimationSequenceNode : public SmartNode
{
public:
	static SmartAnimationSequenceNode* create();
	static SmartAnimationSequenceNode* create(const std::string& defaultSprite);

	SmartAnimationSequenceNode* clone();
	bool isPlayingAnimation();
	void stopAnimation();
	void playSingleFrame(const std::string& initialSequenceResourceFile, int index, float animationSpeed, std::function<void()> onAnimationComplete = nullptr);
	void playSingleFrame(const std::vector<std::string>& animationFiles, int index, float animationSpeed, std::function<void()> onAnimationComplete = nullptr);
	void playAnimation(const std::string& initialSequenceResourceFile, float animationSpeed, bool insertBlankFrame = false, std::function<void()> onAnimationComplete = nullptr);
	void playAnimation(const std::vector<std::string>& animationFiles, float animationSpeed, bool insertBlankFrame = false, std::function<void()> onAnimationComplete = nullptr);
	void playAnimationRepeat(const std::string& initialSequenceResourceFile, float animationSpeed, float repeatDelay = 0.0f, bool insertBlankFrame = false, int repeatCount = -1, std::function<void()> onAnimationComplete = nullptr);
	void playAnimationRepeat(const std::vector<std::string>& animationFiles, float animationSpeed, float repeatDelay = 0.0f, bool insertBlankFrame = false, int repeatCount = -1, std::function<void()> onAnimationComplete = nullptr);
	void playAnimationAndReverse(const std::string& initialSequenceResourceFile, float animationSpeedIn, float reverseDelay, float animationSpeedOut, bool insertBlankFrame = false, bool startReversed = false, std::function<void()> onAnimationComplete = nullptr);
	void playAnimationAndReverse(const std::vector<std::string>& animationFiles, float animationSpeedIn, float reverseDelay, float animationSpeedOut, bool insertBlankFrame = false, bool startReversed = false, std::function<void()> onAnimationComplete = nullptr);
	void playAnimationAndReverseRepeat(const std::string& initialSequenceResourceFile, float animationSpeedIn, float reverseDelay, float animationSpeedOut, float repeatDelay = 0.0f, bool insertBlankFrame = false, bool startReversed = false, int repeatCount = -1, std::function<void()> onAnimationComplete = nullptr);
	void playAnimationAndReverseRepeat(const std::vector<std::string>& animationFiles, float animationSpeedIn, float reverseDelay, float animationSpeedOut, float repeatDelay = 0.0f, bool insertBlankFrame = false, bool startReversed = false, int repeatCount = -1, std::function<void()> onAnimationComplete = nullptr);
	void setAnimationAnchor(cocos2d::Vec2 anchor);
	void setFlippedX(bool isFlipped);
	void setFlippedY(bool isFlipped);
	void setSpriteChangeCallback(std::function<void(const std::string&)> spriteChangeCallback);

	static void PrimeCache(const std::string& initialSequenceResourceFile);
	static int GetAnimationLength(const std::string& initialSequenceResourceFile);
	static const std::vector<std::string>& GetAllAnimationFiles(const std::string& initialSequenceResourceFile);

protected:
	SmartAnimationSequenceNode(const std::string& defaultSprite);
	virtual ~SmartAnimationSequenceNode();

	cocos2d::Sprite* sprite = nullptr;

private:
	typedef SmartNode super;

	void setNewSpriteImage(const std::string& spriteImage);

	cocos2d::Vec2 animationAnchor;
	bool isFlippedX = false;
	bool isFlippedY = false;
	std::string defaultSprite;
	bool hasPlayingAnimation = false;
	int repeatIndex = 0;
	std::function<void(const std::string&)> spriteChangeCallback = nullptr;

	static std::map<std::string, std::vector<std::string>> AnimationFileCache;
	static std::map<std::string, std::map<int, std::vector<std::string>>> SingleFrameCache;
};
