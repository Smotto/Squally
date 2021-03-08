#pragma once

#include <string>

#include "Engine/Inventory/Currency.h"

class LocalizedString;

class IOU : Currency
{
public:
	static IOU* create();

	static std::string getIOUIdentifier();
	static std::string getIOUIconResource();
	static LocalizedString* getIOUString();
	static LocalizedString* getIOUStringPlural();

	LocalizedString* getString() override;
	LocalizedString* getStringPlural() override;
	const std::string& getIconResource() override;
	const std::string& getSerializationKey() override;

protected:
	IOU();
	virtual ~IOU();

private:
	typedef Currency super;
};
