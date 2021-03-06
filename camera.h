#ifndef _CAMERA_H
#define _CAMERA_H

// forward gef declarations
namespace gef
{
	class SonyController;
	class TouchInputManager;
	class Keyboard;
}

#include "maths/vector4.h"

class Camera
{
public:
	Camera();
	virtual ~Camera();
	virtual void CameraController(const float dt, const gef::SonyController* controller) = 0;
	virtual void CameraController(const float frame_time, const gef::TouchInputManager* touch_input, const gef::Keyboard* keyboard) = 0;

	void Update();
	void SetCameraPosition(const float& yaw, const float& pitch, const float& roll, const gef::Vector4& position);
	//void DisplayCameraPosition() const; // dev function

	inline void MoveForward(const float dt) { position_ += forward_ * dt; Update(); }
	inline void MoveBackwards(const float dt) { position_ -= forward_ * dt; Update(); }

	inline void MoveUp(const float dt) { position_ += up_ * dt; Update(); }
	inline void MoveDown(const float dt) { position_ -= up_ * dt; Update(); }

	inline void MoveSideLeft(const float dt) { position_ -= side_ * dt; Update(); }
	inline void MoveSideRight(const float dt) { position_ += side_ * dt; Update(); }

	inline float GetYaw() const { return yaw_; }
	inline void SetYaw(const float arg) { yaw_ = arg; Update(); }
	inline void AddYaw(const float value, const float dt) { yaw_ += value * dt; Update(); }
	inline void subtractYaw(const float value, const float dt) { yaw_ -= value * dt; Update(); }

	inline float GetPitch() const { return pitch_; }
	inline void SetPitch(const float arg) { pitch_ = arg; Update(); }
	inline void AddPitch(const float value, const float dt) { pitch_ += value * dt; Update(); }
	inline void subtractPitch(const float value, const float dt) { pitch_ -= value * dt; Update(); }

	inline float GetRoll() const { return roll_; }
	inline void SetRoll(const float arg) { roll_ = arg; Update(); }
	inline void AddRoll(const float value, const float dt) { roll_ += value * dt; Update(); }
	inline void subtractRoll(const float value, const float dt) { roll_ -= value * dt; Update(); }

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

#endif // !_