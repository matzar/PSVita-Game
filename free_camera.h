#ifndef _FREE_CAMERA_H
#define _FREE_CAMERA_H

#include "camera.h"
#include "maths\vector4.h"

class free_camera : public camera {
public:
	free_camera();
	inline virtual ~free_camera();

	virtual void update();
	inline virtual void moveForward(float dt) { position += forward * dt; }
	inline virtual void moveBackwards(float dt) { position -= forward * dt; }

	inline virtual void moveUp(float dt) { position += up * dt; }
	inline virtual void moveDown(float dt) { position -= up * dt; } 

	inline virtual void moveSideLeft(float dt) { position -= side * dt; }
	inline virtual void moveSideRight(float dt) { position += side * dt; }
		   
	inline virtual float getYaw() { return Yaw; }
	inline virtual void setYaw(float arg) { Yaw = arg; }
	inline virtual void addYaw(float dt, float value) { Yaw += value * dt; }
	inline virtual void subtractYaw(float dt, float value) { Yaw -= value * dt; } 

	inline virtual float getRoll() { return Roll; }
	inline virtual void setPitch(float arg) { Pitch = arg; }
	inline virtual void addPitch(float dt, float value) { Pitch += value * dt; } 
	inline virtual void subtractPitch(float dt, float value) { Pitch -= value * dt; } 

	inline virtual float getPitch() { return Pitch; }
	inline virtual void setRoll(float arg) { Roll = arg; }
	inline virtual void addRoll(float dt, float value) { Roll += value * dt; } 
	inline virtual void subtractRoll(float dt, float value) { Roll -= value * dt; } 

	inline virtual float getPositionX() {	return position.x(); } 
	inline virtual float getPositionY() { return position.y(); } 
	inline virtual float getPositionZ() { return position.z(); } 
	
	inline virtual float getForwardX() { return forward.x(); } 
	inline virtual float getForwardY() { return forward.y(); } 
	inline virtual float getForwardZ() { return forward.z(); } 
	 
	inline virtual float getLookAtX() { return lookAt.x(); } 
	inline virtual float getLookAtY() { return lookAt.y(); } 
	inline virtual float getLookAtZ() { return lookAt.z(); } 
	
	inline virtual float getSideX() { return side.x(); } 
	inline virtual float getSideY() { return side.y(); } 
	inline virtual float getSideZ() { return side.z(); } 

	inline virtual float getUpX() { return up.x(); } 
	inline virtual float getUpY() { return up.y(); } 
	inline virtual float getUpZ() { return up.z(); }  

	inline virtual void updateYaw(int width, int mouseX, int speed) { Yaw += static_cast<float>((mouseX - (width / 2)) / speed); } 
	inline virtual void updatePitch(int height, int mouseY, int speed) { Pitch -= static_cast<float>((mouseY - (height / 2)) / speed); } 
	//virtual void cameraControll(float dt, int width, int height, Input *input);
private:
	gef::Vector4 position;
	gef::Vector4 lookAt;
	gef::Vector4 forward;
	gef::Vector4 up;
	gef::Vector4 side;
	float Yaw, Pitch, Roll;
};

#endif