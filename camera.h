#ifndef _CAMERA_H
#define _CAMERA_H

class camera
{
public:
	inline virtual ~camera() {}
	virtual void update() = 0;
	inline const virtual void moveForward(float dt) = 0;
	inline const virtual void moveBackwards(float dt) = 0;
	inline const virtual void moveUp(float dt) = 0;
	inline const virtual void moveDown(float dt) = 0;
	inline const virtual void moveSideLeft(float dt) = 0;
	inline const virtual void moveSideRight(float dt) = 0;

	inline const virtual void addYaw(float dt, float value) = 0;
	inline const virtual void subtractYaw(float dt, float value) = 0;
	inline const virtual void addPitch(float dt, float value) = 0;
	inline const virtual void subtractPitch(float dt, float value) = 0;
	inline const virtual void addRoll(float dt, float value) = 0;
	inline const virtual void subtractRoll(float dt, float value) = 0;

	inline const virtual float getPositionX() = 0;
	inline const virtual float getPositionY() = 0;
	inline const virtual float getPositionZ() = 0;
	
	inline const virtual float getForwardX() = 0;
	inline const virtual float getForwardY() = 0;
	inline const virtual float getForwardZ() = 0;
	
	inline const virtual float getLookAtX() = 0;
	inline const virtual float getLookAtY() = 0;
	inline const virtual float getLookAtZ() = 0;
	
	inline const virtual float getUpX() = 0;
	inline const virtual float getUpY() = 0;
	inline const virtual float getUpZ() = 0;
	
	inline const virtual float getSideX() = 0;
	inline const virtual float getSideY() = 0;
	inline const virtual float getSideZ() = 0;
	
	inline const virtual float getYaw() = 0;
	inline const virtual float getPitch() = 0;
	inline const virtual float getRoll() = 0;

	inline const virtual void setYaw(float arg) = 0;
	inline const virtual void setPitch(float arg) = 0;
	inline const virtual void setRoll(float arg) = 0;

	inline const virtual void updateYaw(int width, int mouseX, int speed) = 0;
	inline const virtual void updatePitch(int height, int mouseY, int speed) = 0;
	//virtual void cameraControll(float dt, int width, int height) = 0;
};

#endif // _CAMERA_H