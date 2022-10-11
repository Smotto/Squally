#pragma once

#include "Engine/Camera/CameraTrackingData.h"
#include "Engine/Maps/GameObject.h"

namespace cocos2d
{
	class DrawNode;
}

class CameraTrackingData;

class CameraTarget : public GameObject
{
public:
	static CameraTarget* create(cocos2d::ValueMap& properties);

	CameraTrackingData getTrackingData();

	static const std::string MapKey;

private:
	typedef GameObject super;
	
	CameraTarget(cocos2d::ValueMap& properties);
	virtual ~CameraTarget();

	void onEnter() override;
	void onDeveloperModeEnable(int debugLevel) override;
	void onDeveloperModeDisable() override;

	cocos2d::DrawNode* debugDraw = nullptr;
	float zoom = 0.0f;

	static const std::string PropertyZoom;
};
