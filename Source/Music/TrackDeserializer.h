#pragma once

#include "Engine/Events/SoundEvents.h"
#include "Engine/GlobalNode.h"

class Track;

class TrackDeserializer : public GlobalNode
{
public:
	static TrackDeserializer* getInstance();
	static void RegisterGlobalNode();

	void deserialize(SoundEvents::RequestTrackDeserializationArgs args);

protected:
	TrackDeserializer();
	virtual ~TrackDeserializer();
	
	void initializeListeners() override;

private:
	typedef GlobalNode super;

	std::map<std::string, std::function<Track*()>> deserializers;

	static TrackDeserializer* Instance;
};
