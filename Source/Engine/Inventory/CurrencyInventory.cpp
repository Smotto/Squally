#include "CurrencyInventory.h"

#include "cocos/base/CCEventCustom.h"
#include "cocos/base/CCEventListenerCustom.h"
#include "cocos/base/CCValue.h"

#include "Engine/Events/InventoryEvents.h"
#include "Engine/Save/SaveManager.h"

using namespace cocos2d;

CurrencyInventory* CurrencyInventory::create(std::string saveKey)
{
	return CurrencyInventory::create({ }, saveKey);
}

CurrencyInventory* CurrencyInventory::create(std::map<std::string, int> currency, std::string saveKey)
{
	CurrencyInventory* instance = new CurrencyInventory(currency, saveKey);

	instance->autorelease();

	return instance;
}

CurrencyInventory::CurrencyInventory(std::map<std::string, int> currency, std::string saveKey)
{
	this->saveKey = saveKey;
	this->load();

	for (auto next : currency)
	{
		this->currencyMap[next.first] = Value(next.second);
	}
}

CurrencyInventory::~CurrencyInventory()
{
}

void CurrencyInventory::onEnter()
{
	super::onEnter();
}

void CurrencyInventory::initializeListeners()
{
	super::initializeListeners();

	this->addEventListenerIgnorePause(EventListenerCustom::create(InventoryEvents::EventCurrencyInventoryInstanceChangedPrefix + this->saveKey, [=](EventCustom* eventCustom)
	{
		InventoryEvents::CurrencyInventoryInstanceChangedArgs* args = static_cast<InventoryEvents::CurrencyInventoryInstanceChangedArgs*>(eventCustom->getData());
		
		this->load();
		if (args != nullptr && args->instance != this)
		{
		}
	}));
}

int CurrencyInventory::getCurrencyCount(std::string currencyKey)
{
	if (this->currencyMap.find(currencyKey) != this->currencyMap.end())
	{
		return this->currencyMap.at(currencyKey).asInt();
	}

	return 0;
}

void CurrencyInventory::removeCurrency(std::string currencyKey, int count)
{
	if (this->currencyMap.find(currencyKey) != this->currencyMap.end())
	{
		this->currencyMap[currencyKey] = Value(this->currencyMap.at(currencyKey).asInt() - count);
	}
	else
	{
		this->currencyMap[currencyKey] = Value(0);
	}

	this->save();
}

void CurrencyInventory::addCurrency(std::string currencyKey, int count)
{
	if (this->currencyMap.find(currencyKey) != this->currencyMap.end())
	{
		this->currencyMap[currencyKey] = Value(this->currencyMap.at(currencyKey).asInt() + count);
	}
	else
	{
		this->currencyMap[currencyKey] = Value(count);
	}
	
	this->save();
}

void CurrencyInventory::save()
{
	if (!this->saveKey.empty())
	{
		SaveManager::SaveProfileData(this->saveKey, Value(this->currencyMap));
		
		InventoryEvents::TriggerCurrencyInventoryInstanceChanged(InventoryEvents::CurrencyInventoryInstanceChangedArgs(this, this->saveKey));
	}
}

void CurrencyInventory::load()
{
	if (!this->saveKey.empty())
	{
		this->currencyMap = SaveManager::GetProfileDataOrDefault(this->saveKey, Value(ValueMap())).asValueMap();
	}
}
