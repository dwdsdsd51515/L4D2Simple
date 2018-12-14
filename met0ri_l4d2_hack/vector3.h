#pragma once

class vector3
{
public:
	float x, y, z;

	vector3()
	{
		this->x = 0;
		this->y = 0;
		this->z = 0;
	}

	vector3(float x, float y, float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}


};
typedef vector3 qangle;