#ifndef _CAMERA_H
#define _CAMERA_H

class camera
{
public:
	inline virtual ~camera() {}
	virtual void update() const = 0;
	inline virtual void moveForward(float dt) const = 0;
	inline virtual void moveBackwards(float dt) const = 0;
	inline virtual void moveUp(float dt) const = 0;
	inline virtual void moveDown(float dt) const = 0;
	inline virtual void moveSideLeft(float dt) const = 0;
	inline virtual void moveSideRight(float dt) const = 0;

	inline virtual void addYaw(float dt, float value) const = 0;
	inline virtual void subtractYaw(float dt, float value) const = 0;
	inline virtual void addPitch(float dt, float value) const = 0;
	inline virtual void subtractPitch(float dt, float value) const = 0;
	inline virtual void addRoll(float dt, float value) const = 0;
	inline virtual void subtractRoll(float dt, float value) const = 0;

	inline virtual float getPositionX() const = 0;
	inline virtual float getPositionY() const = 0;
	inline virtual float getPositionZ() const = 0;
	
	inline virtual float getForwardX() const = 0;
	inline virtual float getForwardY() const = 0;
	inline virtual float getForwardZ() const = 0;
	
	inline virtual float getLookAtX() const = 0;
	inline virtual float getLookAtY() const = 0;
	inline virtual float getLookAtZ() const = 0;
	
	inline virtual float getUpX() const = 0;
	inline virtual float getUpY() const = 0;
	inline virtual float getUpZ() const = 0;
	
	inline virtual float getSideX() const = 0;
	inline virtual float getSideY() const = 0;
	inline virtual float getSideZ() const = 0;
	
	inline virtual float getYaw() const = 0;
	inline virtual float getPitch() const = 0;
	inline virtual float getRoll() const = 0;

	inline virtual void setYaw(float arg) const = 0;
	inline virtual void setPitch(float arg) const = 0;
	inline virtual void setRoll(float arg) const = 0;

	inline virtual void updateYaw(int width, int mouseX, int speed) = 0;
	inline virtual void updatePitch(int height, int mouseY, int speed) = 0;
	//virtual void cameraControll(float dt, int width, int height) const = 0;
};

#endif // _CAMERA_H