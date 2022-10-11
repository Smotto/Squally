#pragma once
#include <functional>
#include <string>

#include "cocos/math/Vec2.h"

class BlockBase;
class BoltBase;
class CipherPuzzleData;
class CipherState;
class Connection;
class InputBolt;
class ImmediateBlock;

class CipherEvents
{
public:
	static const std::string EventOpenCipher;
	static const std::string EventExitCipher;
	static const std::string EventRequestBlockSpawn;
	static const std::string EventRequestConnectionCreate;
	static const std::string EventConnectionUpdated;
	static const std::string EventConnectionDestroy;
	static const std::string EventChangeActiveCipher;
	static const std::string EventChangeDisplayDataType;
	static const std::string EventOpenImmediateEditor;
	static const std::string EventOpenAsciiTable;
	static const std::string EventTryUnlockCurrentCipher;
	static const std::string EventRequestStateUpdate;
	static const std::string EventBeforeRequestStateUpdate;
	static const std::string EventBeforeStateUpdate;
	static const std::string EventOnStateUpdate;

	struct CipherOpenArgs
	{
		CipherPuzzleData* cipherPuzzleData = nullptr;

		CipherOpenArgs(CipherPuzzleData* cipherPuzzleData) : cipherPuzzleData(cipherPuzzleData)
		{
		}
	};

	struct CipherBlockSpawnArgs
	{
		BlockBase* block = nullptr;
		cocos2d::Vec2 spawnCoords;

		CipherBlockSpawnArgs(BlockBase* block, cocos2d::Vec2 spawnCoords) : block(block), spawnCoords(spawnCoords)
		{
		}
	};

	struct CipherConnectionCreateArgs
	{
		BoltBase* sourceBolt = nullptr;
		Connection* connection = nullptr;
		cocos2d::Vec2 destination = cocos2d::Vec2::ZERO;

		CipherConnectionCreateArgs(BoltBase* sourceBolt, Connection* connection, cocos2d::Vec2 destination) : sourceBolt(sourceBolt), connection(connection), destination(destination), handled(false)
		{
		}

		void handle()
		{
			this->handled = true;
		}

		bool isHandled()
		{
			return this->handled;
		}

		private:
			bool handled = false;
	};

	struct CipherConnectionUpdatedArgs
	{
		Connection* connection = nullptr;

		CipherConnectionUpdatedArgs(Connection* connection) : connection(connection)
		{
		}
	};

	struct CipherConnectionDestroyArgs
	{
		InputBolt* inputBolt = nullptr;

		CipherConnectionDestroyArgs(InputBolt* inputBolt) : inputBolt(inputBolt)
		{
		}
	};

	struct CipherChangeActiveCipherArgs
	{
		unsigned char input = 0;
		unsigned char output = 0;
		int index = 0;
		bool autoScroll = false;

		CipherChangeActiveCipherArgs(unsigned char input, unsigned char output, int index) : input(input), output(output), index(index), autoScroll(false)
		{
		}
		CipherChangeActiveCipherArgs(unsigned char input, unsigned char output, int index, bool autoScroll) : input(input), output(output), index(index), autoScroll(autoScroll)
		{
		}
	};

	enum class DisplayDataType
	{
		Ascii,
		Bin,
		Dec,
		Hex
	};

	struct CipherChangeDisplayDataTypeArgs
	{
		DisplayDataType displayDataType = DisplayDataType::Ascii;

		CipherChangeDisplayDataTypeArgs(DisplayDataType displayDataType) : displayDataType(displayDataType)
		{
		}
	};

	struct CipherOpenAsciiTableArgs
	{
		ImmediateBlock* immediateBlock = nullptr;

		CipherOpenAsciiTableArgs(ImmediateBlock* immediateBlock) : immediateBlock(immediateBlock)
		{
		}
	};

	struct UnlockArgs
	{
		int cipherIndex = 0;
		bool success = false;
		std::function<void()> callback = nullptr;

		UnlockArgs(int cipherIndex, bool success, std::function<void()> callback) : cipherIndex(cipherIndex), success(success), callback(callback)
		{
		}
	};

	struct CipherExitArgs
	{
		bool victory = false;

		CipherExitArgs(bool victory) : victory(victory)
		{
		}
	};

	static void TriggerOpenCipher(CipherOpenArgs args);
	static void TriggerExitCipher(CipherExitArgs args);
	static void TriggerRequestBlockSpawn(CipherBlockSpawnArgs args);
	static void TriggerRequestConnectionCreate(CipherConnectionCreateArgs args);
	static void TriggerConnectionUpdated(CipherConnectionUpdatedArgs args);
	static void TriggerDestroyConnectionToInput(CipherConnectionDestroyArgs args);
	static void TriggerChangeActiveCipher(CipherChangeActiveCipherArgs args);
	static void TriggerChangeDisplayDataType(CipherChangeDisplayDataTypeArgs args);
	static void TriggerOpenAsciiTable(CipherOpenAsciiTableArgs args);
	static void TriggerTryUnlockCurrentCipher(UnlockArgs args);
	static void TriggerBeforeRequestStateUpdate(CipherState* gameState);
	static void TriggerRequestStateUpdate(CipherState* gameState);
	static void TriggerBeforeStateUpdate(CipherState* gameState);
	static void TriggerOnStateUpdate(CipherState* gameState);
};
