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

	inline virtual void addYaw(float dt, float value);
	inline virtual void subtractYaw(float dt, float value);
	inline virtual void addPitch(float dt, float value);
	inline virtual void subtractPitch(float dt, float value);
	inline virtual void addRoll(float dt, float value);
	inline virtual void subtractRoll(float dt, float value);

	inline virtual float getPositionX();
	inline virtual float getPositionY();
	inline virtual float getPositionZ();

	inline virtual float getForwardX();
	inline virtual float getForwardY();
	inline virtual float getForwardZ();

	inline virtual float getLookAtX();
	inline virtual float getLookAtY();
	inline virtual float getLookAtZ();

	inline virtual float getUpX();
	inline virtual float getUpY();
	inline virtual float getUpZ();

	inline virtual float getSideX();
	inline virtual float getSideY();
	inline virtual float getSideZ();

	inline virtual float getYaw();
	inline virtual float getPitch();
	inline virtual float getRoll();

	inline virtual void setLookAtX(float x);
	inline virtual void setLookAtY(float y);
	inline virtual void setLookAtZ(float z);

	inline virtual void setYaw(float arg);
	inline virtual void setPitch(float arg);
	inline virtual void setRoll(float arg);

	virtual void updateYaw(int width, int mouseX, int speed);
	virtual void updatePitch(int height, int mouseY, int speed);
	void normalise();
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