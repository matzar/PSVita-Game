#ifndef _FREE_CAMERA_H
#define _FREE_CAMERA_H

#include "camera.h"
#include "maths\vector4.h"

class free_camera : public camera {
public:
	free_camera();
	~free_camera();

	virtual void update();
	virtual void moveForward(float dt);
	virtual void moveBackwards(float dt);
	virtual void moveUp(float dt);
	virtual void moveDown(float dt);
	virtual void moveSideLeft(float dt);
	virtual void moveSideRight(float dt);

	virtual void addYaw(float dt, float value);
	virtual void subtractYaw(float dt, float value);
	virtual void addPitch(float dt, float value);
	virtual void subtractPitch(float dt, float value);
	virtual void addRoll(float dt, float value);
	virtual void subtractRoll(float dt, float value);

	virtual float getPositionX();
	virtual float getPositionY();
	virtual float getPositionZ();

	virtual float getForwardX();
	virtual float getForwardY();
	virtual float getForwardZ();

	virtual float getLookAtX();
	virtual float getLookAtY();
	virtual float getLookAtZ();

	virtual float getUpX();
	virtual float getUpY();
	virtual float getUpZ();

	virtual float getSideX();
	virtual float getSideY();
	virtual float getSideZ();

	virtual float getYaw();
	virtual float getPitch();
	virtual float getRoll();

	virtual void setLookAtX(float x);
	virtual void setLookAtY(float y);
	virtual void setLookAtZ(float z);

	virtual void setYaw(float arg) ;
	virtual void setPitch(float arg);
	virtual void setRoll(float arg);

	virtual void updateYaw(int width, int mouseX, int speed);
	virtual void updatePitch(int height, int mouseY, int speed);

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