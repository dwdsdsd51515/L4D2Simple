#pragma once

class i_entity_list
{
public:
	template <typename t>
	t* get_client_entity(int index)
	{
		typedef t* (__thiscall* fn)(void*, int);
		return call_vfunc<fn>(this, 3)(this, index);
	}
};
i_entity_list* g_entity_list = nullptr;