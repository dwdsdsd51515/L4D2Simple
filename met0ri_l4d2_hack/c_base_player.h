#pragma once

enum PlayerStates_t
{
	FL_ONGROUND = (1 << 0),
	FL_DUCKING = (1 << 1),
	FL_WATERJUMP = (1 << 2),
	FL_ONTRAIN = (1 << 3),
	FL_INRAIN = (1 << 4),
	FL_FROZEN = (1 << 5),
	FL_ATCONTROLS = (1 << 6),
	FL_CLIENT = (1 << 7),
	FL_FAKECLIENT = (1 << 8),
	FL_INWATER = (1 << 9),
};

class c_base_player : public c_base_entity
{
public:
	int& m_fFlags()
	{
		return *(int*)((uintptr_t)this + 0xF0);
	}

	vector3 m_vecVelocity()
	{
		return *(vector3*)((uintptr_t)this + 0x100);
	}
};