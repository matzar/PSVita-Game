#ifndef _CAMERA_H
#define _CAMERA_H
#include "maths\vector4.h"

class camera
{
public:
	inline virtual ~camera() {}
	virtual void update() = 0;
	inline virtual void moveForward(float dt) = 0;
	inline virtual void moveBackwards(float dt) = 0;

	inline virtual void moveUp(float dt) = 0;
	inline virtual void moveDown(float dt) = 0;

	inline virtual void moveSideLeft(float dt) = 0;
	inline virtual void moveSideRight(float dt) = 0;

	inline virtual float getYaw() = 0;
	inline virtual void setYaw(float arg) = 0;
	inline virtual void addYaw(float dt, float value) = 0;
	inline virtual void subtractYaw(float dt, float value) = 0;

	inline virtual float getPitch() = 0;
	inline virtual void setPitch(float arg) = 0;
	inline virtual void addPitch(float dt, float value) = 0;
	inline virtual void subtractPitch(float dt, float value) = 0;

	inline virtual float getRoll() = 0;
	inline virtual void setRoll(float arg) = 0;
	inline virtual void addRoll(float dt, float value) = 0;
	inline virtual void subtractRoll(float dt, float value) = 0;

	inline virtual float getPositionX() = 0;
	inline virtual float getPositionY() = 0;
	inline virtual float getPositionZ() = 0;
	
	inline virtual float getForwardX() = 0;
	inline virtual float getForwardY() = 0;
	inline virtual float getForwardZ() = 0;
	
	inline virtual float getLookAtX() = 0;
	inline virtual float getLookAtY() = 0;
	inline virtual float getLookAtZ() = 0;
	
	inline virtual float getSideX() = 0;
	inline virtual float getSideY() = 0;
	inline virtual float getSideZ() = 0;

	inline virtual float getUpX() = 0;
	inline virtual float getUpY() = 0;
	inline virtual float getUpZ() = 0;
	
	inline virtual gef::Vector4 getPositionVector() = 0;
	inline virtual gef::Vector4 getForawrdVector() = 0;
	inline virtual gef::Vector4 getLookAtVector() = 0;
	inline virtual gef::Vector4 getSideVector() = 0;
	inline virtual gef::Vector4 getUpVector() = 0;

	inline virtual void updateYaw(int width, int mouseX, int speed) = 0;
	inline virtual void updatePitch(int height, int mouseY, int speed) = 0;
	//virtual void cameraControll(float dt, int width, int height) const = 0;
};

#endif // _CAMERA_H