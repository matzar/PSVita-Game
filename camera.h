#ifndef _CAMERA_H
#define _CAMERA_H

class camera
{
public:
	virtual ~camera() {}
	virtual void update() = 0;
	virtual void moveForward(float dt) = 0;
	virtual void moveBackwards(float dt) = 0;
	virtual void moveUp(float dt) = 0;
	virtual void moveDown(float dt) = 0;
	virtual void moveSideLeft(float dt) = 0;
	virtual void moveSideRight(float dt) = 0;

	virtual void addYaw(float dt, float value) = 0;
	virtual void subtractYaw(float dt, float value) = 0;
	virtual void addPitch(float dt, float value) = 0;
	virtual void subtractPitch(float dt, float value) = 0;
	virtual void addRoll(float dt, float value) = 0;
	virtual void subtractRoll(float dt, float value) = 0;

	virtual float getPositionX() = 0;
	virtual float getPositionY() = 0;
	virtual float getPositionZ() = 0;

	virtual float getForwardX() = 0;
	virtual float getForwardY() = 0;
	virtual float getForwardZ() = 0;

	virtual float getLookAtX() = 0;
	virtual float getLookAtY() = 0;
	virtual float getLookAtZ() = 0;

	virtual float getUpX() = 0;
	virtual float getUpY() = 0;
	virtual float getUpZ() = 0;

	virtual float getSideX() = 0;
	virtual float getSideY() = 0;
	virtual float getSideZ() = 0;

	virtual float getYaw() = 0;
	virtual float getPitch() = 0;
	virtual float getRoll() = 0;

	virtual void setYaw(float arg) = 0;
	virtual void setPitch(float arg) = 0;
	virtual void setRoll(float arg) = 0;

	virtual void updateYaw(int width, int mouseX, int speed) = 0;
	virtual void updatePitch(int height, int mouseY, int speed) = 0;
	//virtual void cameraControll(float dt, int width, int height) = 0;
};

#endif // _CAMERA_H