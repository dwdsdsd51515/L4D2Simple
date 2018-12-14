#pragma once
#include "hooked_writeusercmddeltatobuffer.h"
#include "hooked_createmove.h"

vfunc_hook hk_client;
vfunc_hook hk_input;

void initialize_hooks()
{
	const auto idx_createmove = 20;
	const auto idx_writeusercmddeltatobuffer = 5;

	hk_client.setup(g_client);
	hk_input.setup(g_input);

	hk_client.hook_index(idx_createmove, hooked_createmove);
	original_createmove = hk_client.get_original<fn_createmove>(idx_createmove);

	hk_input.hook_index(idx_writeusercmddeltatobuffer, hooked_writeusercmddeltatobuffer);
	original_writeusercmddeltatobuffer = hk_input.get_original<fn_writeusercmddeltatobuffer>(idx_writeusercmddeltatobuffer);
}