#ifndef _FREE_CAMERA_H
#define _FREE_CAMERA_H

// my headers
#include "camera.h"

// forward gef declarations
namespace gef
{
	class SonyController;
	class TouchInputManager;
	class Keyboard;
}

class FreeCamera : public Camera
{
public:
	FreeCamera();

	virtual void CameraController(const float frame_time, const gef::SonyController* controller);
	virtual void CameraController(const float frame_time, const gef::TouchInputManager* touch_input, const gef::Keyboard* keyboard);
};

#endif