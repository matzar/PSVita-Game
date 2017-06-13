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
	inline virtual void MoveBackwards(const float dt) = 0;

	inline virtual void MoveUp(const float dt) = 0;
	inline virtual void MoveDown(const float dt) = 0;

	inline virtual void MoveSideLeft(const float dt) = 0;
	inline virtual void MoveSideRight(const float dt) = 0;

	inline virtual float GetYaw() const = 0;
	inline virtual void SetYaw(const float arg) = 0;
	inline virtual void AddYaw(const float dt, float value) = 0;
	inline virtual void subtractYaw(const float dt, float value) = 0;

	inline virtual float GetPitch() const = 0;
	inline virtual void SetPitch(const float arg) = 0;
	inline virtual void AddPitch(const float dt, float value) = 0;
	inline virtual void subtractPitch(const float dt, float value) = 0;

	inline virtual float GetRoll() const = 0;
	inline virtual void SetRoll(const float arg) = 0;
	inline virtual void AddRoll(const float dt, float value) = 0;
	inline virtual void subtractRoll(const float dt, float value) = 0;
	
	inline virtual gef::Vector4 GetPositionVector() const = 0;
	inline virtual gef::Vector4 GetForawrdVector() const = 0;
	inline virtual gef::Vector4 GetLookAtVector() const = 0;
	inline virtual gef::Vector4 GetSideVector() const = 0;
	inline virtual gef::Vector4 GetUpVector() const = 0;

	virtual void DisplayCameraPosition() const = 0;
};

#endif // _CAMERA_H