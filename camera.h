#ifndef _CAMERA_H
#define _CAMERA_H

#include "maths\vector4.h"

class camera
{
public:
	inline virtual ~camera() {}

	virtual void Update() = 0;
	//virtual void SetCameraPosition(const float& yaw, const float& pitch, const float& roll, const gef::Vector4& position) = 0;

	inline virtual void MoveForward(const float dt) = 0;
	inline virtual void MoveBackwards(float dt) = 0;

	inline virtual void MoveUp(float dt) = 0;
	inline virtual void MoveDown(float dt) = 0;

	inline virtual void MoveSideLeft(float dt) = 0;
	inline virtual void MoveSideRight(float dt) = 0;

	inline virtual float GetYaw() const = 0;
	inline virtual void SetYaw(float arg) = 0;
	inline virtual void AddYaw(float dt, float value) = 0;
	inline virtual void subtractYaw(float dt, float value) = 0;

	inline virtual float GetPitch() const = 0;
	inline virtual void SetPitch(float arg) = 0;
	inline virtual void AddPitch(float dt, float value) = 0;
	inline virtual void subtractPitch(float dt, float value) = 0;

	inline virtual float GetRoll() const = 0;
	inline virtual void SetRoll(float arg) = 0;
	inline virtual void AddRoll(float dt, float value) = 0;
	inline virtual void subtractRoll(float dt, float value) = 0;
	
	inline virtual gef::Vector4 GetPositionVector() const = 0;
	inline virtual gef::Vector4 GetForawrdVector() const = 0;
	inline virtual gef::Vector4 GetLookAtVector() const = 0;
	inline virtual gef::Vector4 GetSideVector() const = 0;
	inline virtual gef::Vector4 GetUpVector() const = 0;

	virtual void DisplayCameraPosition() const = 0;
};

#endif // _CAMERA_H