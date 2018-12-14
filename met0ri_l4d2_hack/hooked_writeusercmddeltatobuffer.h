#pragma once

//#define TICK_INTERVAL			(g_globals->interval_per_tick)
#define TICK_INTERVAL			(100.f / 64.f)
#define TIME_TO_TICKS( dt )		( (int)( 0.5f + (float)(dt) / TICK_INTERVAL ) )

struct INetMessage {
	virtual ~INetMessage();
};

template<typename T>
class CNetMessagePB : public INetMessage, public T {};

class CCLCMsg_Move {
private:
	char __PAD0[0x8];
public:
	int numBackupCommands;
	int numNewCommands;
};
using CCLCMsg_Move_t = CNetMessagePB<CCLCMsg_Move>;

typedef bool(__thiscall *fn_writeusercmddeltatobuffer)(void*, int slot, void* buf, int from, int to, bool is_new_cmd);
fn_writeusercmddeltatobuffer original_writeusercmddeltatobuffer;

int tickBaseShift = 0;
bool inSendMove = false, firstSendMovePack = false;

bool __fastcall hooked_writeusercmddeltatobuffer(void* this0, void* edx, int slot, void* buf,
	int from, int to, bool is_new_cmd) {

	if (tickBaseShift <= 0/* || returnAddress() != &engine->WriteUsercmdDeltaToBufferReturn*/) {
		return original_writeusercmddeltatobuffer(this0, slot, buf, from, to, is_new_cmd);
	}
	if (from != -1) {
		return true; // Skip next calls
	}

	auto CL_SendMove = []()
	{
		using CL_SendMove_t = void(__fastcall*)(void);
		static CL_SendMove_t CL_SendMoveF = follow_rel32<CL_SendMove_t>(offsets::find_pattern(GetModuleHandleA("engine.dll"), ("E8 ? ? ? ? 8B 5D C0")), 1);

		CL_SendMoveF();
	};

	auto WriteUsercmd = [](void* buf, c_usercmd *in, c_usercmd *out)
	{
		using WriteUsercmd_t = void(__cdecl*)(void*, c_usercmd*, c_usercmd*);
		static WriteUsercmd_t WriteUsercmdF = follow_rel32<WriteUsercmd_t>(offsets::find_pattern(GetModuleHandleA("client.dll"), ("E8 ? ? ? ? 83 C4 0C 38 5E 10")), 1);

		WriteUsercmdF(buf, in, out);
	};

	uintptr_t *framePtr;
	__asm mov framePtr, ebp;
	auto msg = reinterpret_cast<CCLCMsg_Move_t*>(*framePtr + 0xFCC);
	auto netchan = g_client_state->get_netchannel();
	//auto& state = GH->clientState();
	//auto netChan = GH->netChan();

	int32_t new_commands = msg->numNewCommands;
	
	if (!inSendMove)
	{
		if (new_commands <= 0)
			return false;

		inSendMove = true;
		firstSendMovePack = true;
		tickBaseShift += new_commands;

		while (tickBaseShift > 0)
		{
			CL_SendMove();
			netchan->transmit(false);
			firstSendMovePack = false;
		}

		inSendMove = false;
		return false;
	}

	if (!firstSendMovePack)
	{
		int32_t loss = min(tickBaseShift, 10);

		tickBaseShift -= loss;
		netchan->m_nOutSequenceNr() += loss;
	}

	int32_t next_cmdnr = g_client_state->lastoutgoingcommand() + g_client_state->chokedcommands() + 1;
	int32_t total_new_commands = min(tickBaseShift, 62);
	tickBaseShift -= total_new_commands;

	from = -1;
	msg->numNewCommands = total_new_commands;
	msg->numBackupCommands = 0;

	for (to = next_cmdnr - new_commands + 1; to <= next_cmdnr; to++)
	{
		if (!original_writeusercmddeltatobuffer(this0, slot, buf, from, to, true))
			return false;

		from = to;
	}

	c_usercmd *last_realCmd = g_input->get_usercmd(slot, from);
	c_usercmd fromCmd;

	if (last_realCmd)
		fromCmd = *last_realCmd;

	c_usercmd toCmd = fromCmd;
	toCmd.command_number++;
	toCmd.tick_count += 200;

	for (int i = new_commands; i <= total_new_commands; i++)
	{
		WriteUsercmd(buf, &toCmd, &fromCmd);
		fromCmd = toCmd;
		toCmd.command_number++;
		toCmd.tick_count++;
	}

	return true;
}