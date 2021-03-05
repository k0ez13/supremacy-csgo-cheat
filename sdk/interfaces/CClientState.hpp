#pragma once

#include <cstdint>

#include "..\math\Vector.hpp"
#include "..\misc\bf_write.h"

#define GenDefineVFunc(...) ( this, __VA_ARGS__ ); }
#define VFUNC( index, func, sig ) auto func { return call_virtual< sig >( this, index ) GenDefineVFunc

class CClockDriftMgr {
public:
	float m_ClockOffsets[ 17 ];   //0x0000
	uint32_t m_iCurClockOffset; //0x0044
	uint32_t m_nServerTick;     //0x0048
	uint32_t m_nClientTick;     //0x004C
}; 

class INetChannel
{
public:
	char pad_0x0000[ 0x18 ]; //0x0000
	__int32 m_nOutSequenceNr; //0x0018 
	__int32 m_nInSequenceNr; //0x001C 
	__int32 m_nOutSequenceNrAck; //0x0020 
	__int32 m_nOutReliableState; //0x0024 
	__int32 m_nInReliableState; //0x0028 
	__int32 m_nChokedPackets; //0x002C

	void Transmit(bool onlyreliable)
	{
		using Fn = bool(__thiscall*)(void*, bool);
		call_virtual<Fn>(this, 49)(this, onlyreliable);
	}

	void send_datagram()
	{
		using Fn = int(__thiscall*)(void*, void*);
		call_virtual<Fn>(this, 46)(this, 0);
	}

	void SetTimeOut(float seconds)
	{
		using Fn = void(__thiscall*)(void*, float);
		return call_virtual<Fn>(this, 4)(this, seconds);
	}

	int RequestFile(const char* filename)
	{
		using Fn = int(__thiscall*)(void*, const char*);
		return call_virtual<Fn>(this, 62)(this, filename);
	}
};

class INetMessage
{
public:
	virtual	~INetMessage() {};

	// Use these to setup who can hear whose voice.
	// Pass in client indices (which are their ent indices - 1).

	virtual void	SetNetChannel(INetChannel * netchan) = 0; // netchannel this message is from/for
	virtual void	SetReliable(bool state) = 0;	// set to true if it's a reliable message

	virtual bool	Process(void) = 0; // calles the recently set handler to process this message

	virtual	bool	ReadFromBuffer(bf_read &buffer) = 0; // returns true if parsing was OK
	virtual	bool	WriteToBuffer(bf_write &buffer) = 0;	// returns true if writing was OK

	virtual bool	IsReliable(void) const = 0;  // true, if message needs reliable handling

	virtual int				GetType(void) const = 0; // returns module specific header tag eg svc_serverinfo
	virtual int				GetGroup(void) const = 0;	// returns net message group of this message
	virtual const char		*GetName(void) const = 0;	// returns network message name, eg "svc_serverinfo"
	virtual INetChannel		*GetNetChannel(void) const = 0;
	virtual const char		*ToString(void) const = 0; // returns a human readable string about message content
};

class CEventInfo {
#define CONCAT_IMPL( x, y ) x##y
#define MACRO_CONCAT( x, y ) CONCAT_IMPL( x, y )
#define PAD( size ) uint8_t MACRO_CONCAT( _pad, __COUNTER__ )[ size ];
public:
	enum {
		EVENT_INDEX_BITS = 8,
		EVENT_DATA_LEN_BITS = 11,
		MAX_EVENT_DATA = 192,  // ( 1<<8 bits == 256, but only using 192 below )
	};

	// 0 implies not in use
	short					m_class_id;
	float					m_fire_delay;
	const void* m_send_table;
	const ClientClass* m_client_class;//void*
	int						m_bits;
	uint8_t* m_data;
	int						m_flags;
	//PAD(0x1C);
	PAD(0x16);
	CEventInfo* m_next;
};
class CClientState
{
public:
	std::byte        pad0[0x9C];
	INetChannel* pNetChannel;
	int                iChallengeNr;
	std::byte        pad1[0x64];
	int                iSignonState;
	std::byte        pad2[0x8];
	float            flNextCmdTime;
	int                iServerCount;
	int                iCurrentSequence;
	char _0x0120[4];
	__int32 m_iClockDriftMgr;
	char _0x0128[68];
	__int32 m_iServerTick;
	__int32 m_iClientTick;
	int                iDeltaTick;
	bool            bPaused;
	std::byte        pad4[0x7];
	int                iViewEntity;
	int                iPlayerSlot;
	char            szLevelName[260];
	char            szLevelNameShort[80];
	char            szGroupName[80];
	std::byte        pad5[0x5C];
	int                iMaxClients;
	std::byte        pad6[0x4984];
	float            flLastServerTickTime;
	bool            bInSimulation;
	std::byte        pad7[0xB];
	int                iOldTickcount;
	float            flTickRemainder;
	float            flFrameTime;
	int                nLastOutgoingCommand;
	int                iChokedCommands;
	int                nLastCommandAck;
	int                iCommandAck;
	int                iSoundSequence;
	std::byte        pad8[0x50];
	Vector            angViewPoint;
	std::byte        pad9[0xD0];
	CEventInfo* m_events;
}; 