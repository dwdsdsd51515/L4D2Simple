#pragma once

// 41 - sendnetmsg
// 
class i_net_channel
{
public:
	bool transmit(bool only_reliable = false)
	{
		typedef bool(__thiscall* fn)(void*, bool);
		return call_vfunc<fn>(this, 48)(this, only_reliable);
	}

	uint32_t& m_nOutSequenceNr()
	{
		return *(uint32_t*)((uintptr_t)this + 0x8);
	}
};

// 0x0010 - net channel
// 0x0068 - signon state
// 0x4AA4 - viewangles
class c_client_state
{
public:
	i_net_channel* get_netchannel()
	{
		return *(i_net_channel**)((uintptr_t)this + 0x0010);
	}

	int get_signon_state()
	{
		return *(int*)((uintptr_t)this + 0x0068);
	}

	uint32_t& lastoutgoingcommand()
	{
		return *(uint32_t*)((uintptr_t)this + 0x4A44);
	}

	uint32_t& chokedcommands()
	{
		return *(uint32_t*)((uintptr_t)this + 0x4A48);
	}
};
c_client_state* g_client_state = nullptr;