#pragma once

#include "Scenes/Cipher/Components/CipherComponentBase.h"

namespace cocos2d
{
	class Sprite;
}

class ClickableNode;
class Connection;

class BoltBase : public CipherComponentBase
{
public:
	virtual void setConnection(Connection* connection);

protected:
	BoltBase();
	virtual ~BoltBase();

	void onEnter() override;
	void initializePositions() override;
	void initializeListeners() override;
	void onAnyStateChange(CipherState* cipherState) override;
	virtual void onConnectionUpdated();

	Connection* connection = nullptr;
	ClickableNode* connectButton = nullptr;
	bool inputDebug = false;

private:
	typedef CipherComponentBase super;

	bool isCreatingConnection = false;
};
