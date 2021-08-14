#pragma once

#include "Engine/GlobalNode.h"

class HackActivatedAbility;
class HackableCode;

class GlobalHackAttributeContainer : public GlobalNode
{
public:
	static GlobalHackAttributeContainer* create();

	static void RegisterGlobalNode();
	static HackableCode* GetHackableCode(void* startAddress);
	static void RegisterHackableCode(HackableCode* hackableCode);
	static HackActivatedAbility* GetHackActivatedAbility(std::string identifier);
	static void RegisterHackActivatedAbility(HackActivatedAbility* hackActivatedAbility);

protected:
	GlobalHackAttributeContainer();
	virtual ~GlobalHackAttributeContainer();

	void initializeListeners() override;

private:
	typedef GlobalNode super;
	
	static GlobalHackAttributeContainer* getInstance();

	std::map<void*, HackableCode*> codeMap;
	std::map<std::string, HackActivatedAbility*> abilityMap;
	cocos2d::Node* hackableCodeContainer = nullptr;
	cocos2d::Node* hackableAbilityContainer = nullptr;

	static GlobalHackAttributeContainer* instance;
};
