#pragma once
#include "i_base_client_dll.h"
#include "iv_engine_client.h"
#include "i_entity_list.h"
#include "c_input.h"
#include "c_client_state.h"

void capture_interfaces()
{
	g_client = capture_interface<i_base_client_dll>("client.dll", "VClient016");
	g_engine = capture_interface<iv_engine_client>("engine.dll", "VEngineClient013");
	g_entity_list = capture_interface<i_entity_list>("client.dll", "VClientEntityList003");

	g_input = *(c_input**)offsets::cinput;

	auto get_clientstate = []() -> uintptr_t
	{
		using fn = uintptr_t(__cdecl*)(int);
		static fn func = (fn)offsets::clientstate;

		return func(-1);
	};


	g_client_state = (c_client_state*)get_clientstate();
}