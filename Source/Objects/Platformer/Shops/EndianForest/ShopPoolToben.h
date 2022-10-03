#pragma once

#include "Objects/Platformer/Shops/ShopPool.h"

class ShopPoolToben : public ShopPool
{
public:
	static ShopPoolToben* create(cocos2d::ValueMap& properties);

	static const std::string MapKey;
	static const std::string PoolName;

protected:
	ShopPoolToben(cocos2d::ValueMap& properties);
	virtual ~ShopPoolToben();

private:
	typedef ShopPool super;
};
