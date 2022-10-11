#pragma once

#include "Engine/Camera/CameraTrackingData.h"
#include "Engine/Maps/GameObject.h"

class CameraFocus : public GameObject
{
public:
	static CameraFocus* create(cocos2d::ValueMap& properties);

	CameraTrackingData getTrackingData();

	static const std::string MapKey;
	static const std::string MapKeyZoom;
	static const std::string MapKeyScrollBoundsX;
	static const std::string MapKeyScrollBoundsY;
	static const std::string MapKeyScrollSpeedX;
	static const std::string MapKeyScrollSpeedY;
	static const std::string MapKeyScrollType;

protected:
	CameraFocus(cocos2d::ValueMap& properties);
	virtual ~CameraFocus();

	void onEnter() override;
	void onEnterTransitionDidFinish() override;

private:
	typedef GameObject super;

	float zoom = 0.0f;
	cocos2d::Vec2 scrollBounds;
	cocos2d::Vec2 scrollSpeed;
	std::string scrollType;
};
