#ifndef _FREE_CAMERA_H
#define _FREE_CAMERA_H

#include "camera.h"
#include "maths\vector4.h"

class free_camera : public camera {
public:
	free_camera();
	inline virtual ~free_camera() {}

	virtual void update();

	inline virtual void moveForward(float dt) { position_ += forward_ * dt; }
	inline virtual void moveBackwards(float dt) { position_ -= forward_ * dt; }

	inline virtual void moveUp(float dt) { position_ += up_ * dt; }
	inline virtual void moveDown(float dt) { position_ -= up_ * dt; } 

	inline virtual void moveSideLeft(float dt) { position_ -= side_ * dt; }
	inline virtual void moveSideRight(float dt) { position_ += side_ * dt; }
		   
	inline virtual float getYaw() { return yaw_; }
	inline virtual void setYaw(float arg) { yaw_ = arg; }
	inline virtual void addYaw(float dt, float value) { yaw_ += value * dt; }
	inline virtual void subtractYaw(float dt, float value) { yaw_ -= value * dt; } 

	inline virtual float getPitch() { return pitch_; }
	inline virtual void setPitch(float arg) { pitch_ = arg; }
	inline virtual void addPitch(float dt, float value) { pitch_ += value * dt; } 
	inline virtual void subtractPitch(float dt, float value) { pitch_ -= value * dt; } 

	inline virtual float getRoll() { return roll_; }
	inline virtual void setRoll(float arg) { roll_ = arg; }
	inline virtual void addRoll(float dt, float value) { roll_ += value * dt; } 
	inline virtual void subtractRoll(float dt, float value) { roll_ -= value * dt; } 

	inline virtual gef::Vector4 getPositionVector() { return position_; }
	inline virtual gef::Vector4 getForawrdVector() { return forward_; }
	inline virtual gef::Vector4 getLookAtVector() { return look_at_; }
	inline virtual gef::Vector4 getSideVector() { return side_; }
	inline virtual gef::Vector4 getUpVector() { return up_; }

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