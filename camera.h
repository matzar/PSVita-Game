#ifndef _CAMERA_H
#define _CAMERA_H

#include "maths/vector4.h"

class Camera
{
public:
	Camera();
	~Camera();

	void Update();
	void SetCameraPosition(const float& yaw, const float& pitch, const float& roll, const gef::Vector4& position);
	void DisplayCameraPosition() const;

	inline void MoveForward(const float dt) { position_ += forward_ * dt; Update(); }
	inline void MoveBackwards(const float dt) { position_ -= forward_ * dt; Update(); }

	inline void MoveUp(const float dt) { position_ += up_ * dt; Update(); }
	inline void MoveDown(const float dt) { position_ -= up_ * dt; Update(); }

	inline void MoveSideLeft(const float dt) { position_ -= side_ * dt; Update(); }
	inline void MoveSideRight(const float dt) { position_ += side_ * dt; Update(); }

	inline float GetYaw() const { return yaw_; }
	inline void SetYaw(const float arg) { yaw_ = arg; Update(); }
	inline void AddYaw(const float dt, const float value) { yaw_ += value * dt; Update(); }
	inline void subtractYaw(const float dt, const float value) { yaw_ -= value * dt; Update(); }

	inline float GetPitch() const { return pitch_; }
	inline void SetPitch(const float arg) { pitch_ = arg; Update(); }
	inline void AddPitch(const float dt, const float value) { pitch_ += value * dt; Update(); }
	inline void subtractPitch(const float dt, const float value) { pitch_ -= value * dt; Update(); }

	inline float GetRoll() const { return roll_; }
	inline void SetRoll(const float arg) { roll_ = arg; Update(); }
	inline void AddRoll(const float dt, const float value) { roll_ += value * dt; Update(); }
	inline void subtractRoll(const float dt, const float value) { roll_ -= value * dt; Update(); }

	inline gef::Vector4 GetPositionVector() const { return position_; }
	inline gef::Vector4 GetForawrdVector() const { return forward_; }
	inline gef::Vector4 GetLookAtVector() const { return look_at_; }
	inline gef::Vector4 GetSideVector() const { return side_; }
	inline gef::Vector4 GetUpVector() const { return up_; }
protected:
	gef::Vector4 position_;
	gef::Vector4 forward_;
	gef::Vector4 look_at_;
	gef::Vector4 side_;
	gef::Vector4 up_;

	float yaw_;
	float pitch_;
	float roll_;
};

#endif // _CAMERA_H