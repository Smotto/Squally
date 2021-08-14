#pragma once

#include "Engine/SmartNode.h"
#include "Engine/Hackables/HackableCode.h"

class LocalizedString;
class LocalizedLabel;

class HackablePreview : public SmartNode
{
public:
	void setAssemblyTextVisible(bool isVisible);

	virtual HackablePreview* clone() = 0;

protected:
	HackablePreview();
	virtual ~HackablePreview();

	LocalizedLabel* createRegisterEqualsValueLabel(HackableCode::Register reg, bool isPointer, LocalizedString* value);
	LocalizedString* getRegisterString(HackableCode::Register reg);

	cocos2d::Node* assemblyTextNode = nullptr;
	cocos2d::Node* previewNode = nullptr;

	static const float PreviewRadius;
	static const cocos2d::Color4B RegisterColor;

private:
	typedef SmartNode super;
};
