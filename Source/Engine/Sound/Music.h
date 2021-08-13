#pragma once

#include "Engine/Sound/SoundBase.h"

class Track;

class Music : public SoundBase
{
public:
	void unfreeze() override;
	
	Track* getOwner();

protected:
	friend class MusicPlayer;
	friend class Track;

	static Music* createAndAddGlobally(Track* owner, std::string musicResource);

	Music(cocos2d::ValueMap& properties, Track* owner, std::string musicResource);
	virtual ~Music();
	
	void initializeListeners() override;
	void pause() override;
	void play(bool repeat = true, float startDelay = 0.0f) override;
	void copyStateFrom(Music* music);
	void clearState();
	float getConfigVolume() override;
	void orphanMusic();
	bool isOrphaned();

private:
	typedef SoundBase super;

	void cancelIfDelayed();

	Track* owner = nullptr;
	bool orphaned = false;
};
