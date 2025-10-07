#pragma once
#include "Window.h"
#include <cmath>

//2 Byte große Definitionen für verschiedene Bewegungen -> Werden kombiniert zu n vielen
#define DirectionX			(unsigned int)0x0001
#define DirectionXn			(unsigned int)0x0002
#define DirectionY			(unsigned int)0x0004
#define DirectionYn			(unsigned int)0x0008
#define DirectionZ			(unsigned int)0x0010
#define DirectionZn			(unsigned int)0x0020
#define RotationYaw			(unsigned int)0x0040
#define RotationYawn		(unsigned int)0x0080
#define RotationPitch		(unsigned int)0x0100
#define RotationPitchn		(unsigned int)0x0200
#define RotationRoll		(unsigned int)0x0400
#define RotationRolln		(unsigned int)0x0800
#define END					(unsigned int)0x1000
#define TransformType		unsigned int

class Camera {
public:
	Camera();
	struct Transform
	{
		float x = 0;
		float y = 0;
		float z = 0;
		float yaw = 0;
		float pitch = 0;
		float roll = 0;
	};
private:
	Transform structTransform;
public:
	void UpdatePosition(TransformType HeldKeys, float ValuePos , float ValueRot);
	Camera::Transform& GetPosition();
};