#include "Camera.h"
#include <cmath>
#include <numbers>

Camera::Camera()
{
	structTransform = {-0.7, -1.5, 8.0, 0, 0, 0};
}

void Camera::UpdatePosition(TransformType HeldKeys, float ValuePos, float ValueRot)
{
	//Pi Values for Calculations
	float pi = 3.141592653f;
	float pihalf = pi / 2;

	for (unsigned int i = 1; i < END; i <<= 2) //durch alle Bewegungen iterieren -> *4 sorgt für iteration durch alle Bits von positiven Richtungen
	{
		if (HeldKeys & i)
		{
			switch (i)
			{
			case DirectionX:
			{
				structTransform.x += std::cos(structTransform.yaw) * ValuePos;
				structTransform.z += std::cos(structTransform.yaw + pihalf) * ValuePos;
				break;
			}
			case DirectionY:
			{
				structTransform.y += ValuePos;
				break;
			}
			case DirectionZ:
			{
				structTransform.z += std::cos(structTransform.yaw) * ValuePos;
				structTransform.x -= std::cos(structTransform.yaw + pihalf) * ValuePos;
				break;
			}
			case RotationYaw:
			{
				structTransform.yaw += ValueRot;
				break;
			}
			case RotationPitch:
			{
				structTransform.pitch += ValueRot;
				break;
			}
			case RotationRoll:
			{
				structTransform.roll += ValueRot;
				break;
			}
			}
		}
		if (HeldKeys & (i << 1)) //i * 2 Teilen um für die negativen Eingaben zu testen
		{
			switch (i << 1)
			{
			case DirectionXn:
			{
				structTransform.x -= std::cos(structTransform.yaw) * ValuePos;
				structTransform.z -= std::cos(structTransform.yaw + pihalf) * ValuePos;
				break;
			}
			case DirectionYn:
			{
				structTransform.y -= ValuePos;
				break;
			}
			case DirectionZn:
			{
				structTransform.z -= std::cos(structTransform.yaw) * ValuePos;
				structTransform.x += std::cos(structTransform.yaw + pihalf) * ValuePos;
				break;
			}
			case RotationYawn:
			{
				structTransform.yaw -= ValueRot;
				break;
			}
			case RotationPitchn:
			{
				structTransform.pitch -= ValueRot;
				break;
			}
			case RotationRolln:
			{
				structTransform.roll -= ValueRot;
				break;
			}
			}
		}
	}
}

Camera::Transform& Camera::GetPosition()
{
	return structTransform;
}