#ifndef _FREE_CAMERA_H
#define _FREE_CAMERA_H

#include "camera.h"

class FreeCamera : public Camera
{
public:
	FreeCamera();
	//inline virtual ~FreeCamera() {}

	virtual void CameraControll(const float frame_time, const gef::SonyController* controller);
};

#endif