#ifndef _CAMERA_H
#define _CAMERA_H

#include "maths\vector4.h"

class camera
{
public:
	inline virtual ~camera() {}
	virtual void Update() = 0;
	inline virtual void MoveForward(float dt) = 0;
	inline virtual void MoveBackwards(float dt) = 0;

	inline virtual void MoveUp(float dt) = 0;
	inline virtual void MoveDown(float dt) = 0;

	inline virtual void MoveSideLeft(float dt) = 0;
	inline virtual void MoveSideRight(float dt) = 0;

	inline virtual float GetYaw() = 0;
	inline virtual void SetYaw(float arg) = 0;
	inline virtual void AddYaw(float dt, float value) = 0;
	inline virtual void subtractYaw(float dt, float value) = 0;

	inline virtual float GetPitch() = 0;
	inline virtual void SetPitch(float arg) = 0;
	inline virtual void AddPitch(float dt, float value) = 0;
	inline virtual void subtractPitch(float dt, float value) = 0;

	inline virtual float GetRoll() = 0;
	inline virtual void SetRoll(float arg) = 0;
	inline virtual void AddRoll(float dt, float value) = 0;
	inline virtual void subtractRoll(float dt, float value) = 0;
	
	inline virtual gef::Vector4 GetPositionVector() = 0;
	inline virtual gef::Vector4 GetForawrdVector() = 0;
	inline virtual gef::Vector4 GetLookAtVector() = 0;
	inline virtual gef::Vector4 GetSideVector() = 0;
	inline virtual gef::Vector4 GetUpVector() = 0;

	//inline virtual void updateYaw(int width, int mouseX, int speed) = 0;
	//inline virtual void updatePitch(int height, int mouseY, int speed) = 0;
	//virtual void cameraControll(float dt, int width, int height) const = 0;
};

#endif // _CAMERA_H