#pragma once

typedef bool(__stdcall * fn_createmove)(int sequence_number, float input_sample_frametime, bool active);
fn_createmove original_createmove;

void __stdcall hooked_createmove(int sequence_number, float input_sample_frametime, bool active)
{
	original_createmove(sequence_number, input_sample_frametime, active);

	auto verified_commands = *(c_verified_usercmd**)((uintptr_t)g_input + 0xE0);
	if (!verified_commands)
		return;

	auto verified_cmd = &verified_commands[sequence_number % 150];

	auto cmd = g_input->get_usercmd(0, sequence_number);
	if (!cmd || !verified_cmd)
		return;

	auto local_player = g_entity_list->get_client_entity<c_base_player>(g_engine->get_local_player());
	if (!local_player)
		return;

	// bunnyhop
	if (cmd->buttons & IN_JUMP && !(local_player->m_fFlags() & FL_ONGROUND))
		cmd->buttons &= ~IN_JUMP;

	// teleport: working
	/*if (GetAsyncKeyState(VK_RMENU))
	{
		//cmd->viewangles.x = FLT_MAX;
		//cmd->viewangles.y = FLT_MAX;
		cmd->viewangles.z = FLT_MAX;
	}*/

	// airstuck: working
	/*if (GetAsyncKeyState(VK_RMENU))
	{
		cmd->tick_count = 0xFFFFFF;
	}*/

	// failed try to implement instant medkit using old cs go exploit
	/*static int exploit_tick = 0;
	if (GetAsyncKeyState(VK_LMENU))
	{
		if ((cmd->buttons & IN_ATTACK))
		{
			if (exploit_tick >= 0 &&
				exploit_tick <  2)
			{
				g_client_state->lastoutgoingcommand() += 3 * 150;
				cmd->command_number += 3 * 150;
				g_client_state->get_netchannel()->m_nOutSequenceNr() += 3 * 150;

				exploit_tick++;
			}
		}
		else
		{
			exploit_tick = 0;
		}
	}*/

	// failed try to implement instant medkit using tickbase manipulation
	/*if (GetAsyncKeyState(VK_LMENU))
	{
		if ((cmd->buttons & IN_ATTACK))
		{
			tickBaseShift = TIME_TO_TICKS(6.f);
		}
	}*/

	verified_cmd->m_cmd = *cmd;
	verified_cmd->m_crc = cmd->get_checksum();
}