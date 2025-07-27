#include "Window.h"
#include <cmath>

#pragma once
class Camera {
public:
	struct transformstruct {
		float x = 0;
		float y = 0;
		float z = 0;
		float rx = 0;
		float ry = 0;
		float rz = 0;
	};
public:
	static float RotationLoop(float angle)
	{
		if (2* std::atan(1) * 4 >= angle && angle >= -2* std::atan(1) * 4)
		{
			return angle;
		}
		else { return 0; }
	}
	static transformstruct UpDatePosition(transformstruct stf, WPARAM currentpressedkey)
	{
		switch (currentpressedkey)
		{
		case (VK_UP):
			{
			stf.rx += 0.05f;
			break;
			}
		case (VK_DOWN):
		{
			stf.rx += -0.05f;
			break;
		}
		case (VK_LEFT):
		{
			stf.ry += -0.05f;
			break;
		}
		case (VK_RIGHT):
		{
			stf.ry += 0.05f;
			break;
		}
		case ('Q'):
		{
			stf.rz += -0.05f;
			break;
		}
		case ('E'):
		{
			stf.rz += 0.05f;
			break;
		}


		case ('W'):
		{
			stf.z += -0.05f;
			break;
		}
		case ('A'):
		{
			stf.x += 0.05f;
			break;
		}
		case ('S'):
		{
			stf.z += 0.05f;
			break;
		}
		case ('D'):
		{
			stf.x += -0.05f;
			break;
		}
		case (VK_SPACE):
		{
			stf.y += -0.05f;
			break;
		}
		case (VK_CONTROL):
		{
			stf.y += 0.05f;
			break;
		}
		}
		return stf;
	}
};