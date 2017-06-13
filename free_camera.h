#ifndef _FREE_CAMERA_H
#define _FREE_CAMERA_H

#include "camera.h"
#include "maths\vector4.h"

class free_camera : public camera {
public:
	free_camera();
	inline virtual ~free_camera() {}

	virtual void Update();
	

	inline virtual void MoveForward(const float dt) { position_ += forward_ * dt; Update(); }
	inline virtual void MoveBackwards(const float dt) { position_ -= forward_ * dt; Update(); }

	inline virtual void MoveUp(const float dt) { position_ += up_ * dt; Update(); }
	inline virtual void MoveDown(const float dt) { position_ -= up_ * dt; Update(); } 

	inline virtual void MoveSideLeft(const float dt) { position_ -= side_ * dt; Update(); }
	inline virtual void MoveSideRight(const float dt) { position_ += side_ * dt; Update(); }
		   
	inline virtual float GetYaw() const { return yaw_; }
	inline virtual void SetYaw(const float arg) { yaw_ = arg; Update(); }
	inline virtual void AddYaw(const float dt, float value) { yaw_ += value * dt; Update(); }
	inline virtual void subtractYaw(const float dt, float value) { yaw_ -= value * dt; Update(); } 

	inline virtual float GetPitch() const { return pitch_; }
	inline virtual void SetPitch(const float arg) { pitch_ = arg; Update(); }
	inline virtual void AddPitch(const float dt, float value) { pitch_ += value * dt; Update(); } 
	inline virtual void subtractPitch(const float dt, float value) { pitch_ -= value * dt; Update(); } 

	inline virtual float GetRoll() const { return roll_; }
	inline virtual void SetRoll(const float arg) { roll_ = arg; Update(); }
	inline virtual void AddRoll(const float dt, float value) { roll_ += value * dt; Update(); } 
	inline virtual void subtractRoll(const float dt, float value) { roll_ -= value * dt; Update(); } 

	inline virtual gef::Vector4 GetPositionVector() const { return position_; }
	inline virtual gef::Vector4 GetForawrdVector() const { return forward_; }
	inline virtual gef::Vector4 GetLookAtVector() const { return look_at_; }
	inline virtual gef::Vector4 GetSideVector() const { return side_; }
	inline virtual gef::Vector4 GetUpVector() const { return up_; }

	virtual void DisplayCameraPosition() const;
private:
	gef::Vector4 position_;
	gef::Vector4 forward_;
	gef::Vector4 look_at_;
	gef::Vector4 side_;
	gef::Vector4 up_;
	float yaw_, pitch_, roll_;
};

#endif