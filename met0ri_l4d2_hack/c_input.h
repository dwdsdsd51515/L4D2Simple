#pragma once

class c_input
{
public:
	c_usercmd* get_usercmd(int slot, int sequence_number)
	{
		typedef c_usercmd*(__thiscall* fn)(void*, int, int);
		return call_vfunc<fn>(this, 8)(this, slot, sequence_number);
	}
};
c_input* g_input = nullptr;