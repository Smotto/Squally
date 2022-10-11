#pragma once

#include <map>
#include <string>

enum class HackFlags
{
	None	 	= 0,
	Fire	 	= 1 << 0,
	Frost	 	= 1 << 1,
	Light	 	= 1 << 2,
	Lightning	= 1 << 3,
	Nature	 	= 1 << 4,
	Shadow	 	= 1 << 5,
	Water	 	= 1 << 6,
	Wind	 	= 1 << 7,
	Arcane	 	= 1 << 8,
	Gravity	 	= 1 << 9,
};

class HackFlagUtils
{
public:
	static int GetCurrentHackFlags();

private:
	static std::map<std::string, int> EssenceFlagMap;
};

