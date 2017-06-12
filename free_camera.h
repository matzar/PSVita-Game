#ifndef _FREE_CAMERA_H
#define _FREE_CAMERA_H

#include "camera.h"
#include "maths\vector4.h"

class free_camera : public camera {
public:
	free_camera();
	inline virtual ~free_camera() {}

	virtual void Update();

	inline virtual void MoveForward(float dt) { position_ += forward_ * dt; Update(); }
	inline virtual void MoveBackwards(float dt) { position_ -= forward_ * dt; Update(); }

	inline virtual void MoveUp(float dt) { position_ += up_ * dt; Update(); }
	inline virtual void MoveDown(float dt) { position_ -= up_ * dt; Update(); } 

	inline virtual void MoveSideLeft(float dt) { position_ -= side_ * dt; Update(); }
	inline virtual void MoveSideRight(float dt) { position_ += side_ * dt; Update(); }
		   
	inline virtual float GetYaw() { return yaw_; Update(); }
	inline virtual void SetYaw(float arg) { yaw_ = arg; Update(); }
	inline virtual void AddYaw(float dt, float value) { yaw_ += value * dt; Update(); }
	inline virtual void subtractYaw(float dt, float value) { yaw_ -= value * dt; Update(); } 

	inline virtual float GetPitch() { return pitch_; Update(); }
	inline virtual void SetPitch(float arg) { pitch_ = arg; Update(); }
	inline virtual void AddPitch(float dt, float value) { pitch_ += value * dt; Update(); } 
	inline virtual void subtractPitch(float dt, float value) { pitch_ -= value * dt; Update(); } 

	inline virtual float GetRoll() { return roll_; Update(); }
	inline virtual void SetRoll(float arg) { roll_ = arg; Update(); }
	inline virtual void AddRoll(float dt, float value) { roll_ += value * dt; Update(); } 
	inline virtual void subtractRoll(float dt, float value) { roll_ -= value * dt; Update(); } 

	inline virtual gef::Vector4 GetPositionVector() { return position_; }
	inline virtual gef::Vector4 GetForawrdVector() { return forward_; }
	inline virtual gef::Vector4 GetLookAtVector() { return look_at_; }
	inline virtual gef::Vector4 GetSideVector() { return side_; }
	inline virtual gef::Vector4 GetUpVector() { return up_; }

	//inline virtual void up_dateYaw(int width, int mouseX, int speed) { yaw_ += static_cast<float>((mouseX - (width / 2)) / speed); } 
	//inline virtual void up_datePitch(int height, int mouseY, int speed) { pitch_ -= static_cast<float>((mouseY - (height / 2)) / speed); } 
	//virtual void cameraControll(float dt, int width, int height, Input *input);
private:
	gef::Vector4 position_;
	gef::Vector4 forward_;
	gef::Vector4 look_at_;
	gef::Vector4 side_;
	gef::Vector4 up_;
	float yaw_, pitch_, roll_;
};

#endif