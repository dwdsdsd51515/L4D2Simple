#pragma once

class iv_engine_client
{
public:
	int get_local_player()
	{
		typedef int(__thiscall* fn)(void*);
		return call_vfunc<fn>(this, 12)(this);
	}
};
iv_engine_client* g_engine = nullptr;