#pragma once
#include "Engine/SmartNode.h"

class GlobalNode : public SmartNode
{
public:
	static GlobalNode* create();

protected:
	GlobalNode();
	virtual ~GlobalNode();

	virtual void pause() override;
	
private:
	typedef SmartNode super;
};
