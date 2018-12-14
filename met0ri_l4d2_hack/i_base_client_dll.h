#pragma once

class i_base_client_dll
{
public:
	/*client_class* get_all_classes()
	{
		typedef client_class*(__thiscall* fn)(void*);
		return call_vfunc<fn>(this, 8)(this);
	}*/
};
i_base_client_dll* g_client = nullptr;