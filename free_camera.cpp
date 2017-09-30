#include "free_camera.h"
// gef headers
#include "maths/vector4.h"
#include "input/sony_controller_input_manager.h"

// only on windows platforms
#ifdef _WIN32
#include <input/keyboard.h>  
#include <platform/d3d11/input/touch_input_manager_d3d11.h>
#endif // _WIN32

FreeCamera::FreeCamera()
{
	Update();

	// first view I tried
	//SetCameraPosition(60.0f, -10.0f, 0.0f, gef::Vector4(-5.0f, 4.0f, 8.0f));

	// Right side view
	//SetCameraPosition(80.0f, -15.0f, 0.0f, gef::Vector4(-6.4f, 3.8f, 3.9f));

	// TPP player position
	SetCameraPosition(90.0f, -12.0f, 0.0f, gef::Vector4(-8.0f, 6.0f, 0.0f));
}

void FreeCamera::CameraController(const float frame_time, const gef::SonyController* controller)
{
	if (controller)
	{
		float camera_speed = 10.0f;

		float left_horizontal_input = controller->left_stick_x_axis();
		float left_vertical_input = controller->left_stick_y_axis();

		float right_horizontal_input = controller->right_stick_x_axis();
		float right_vertical_input = controller->right_stick_y_axis();
		// left stick - pan controll
		if (controller->left_stick_x_axis() < 0)
		{
			MoveSideLeft(frame_time * camera_speed);
		}
		if (controller->left_stick_x_axis() > 0)
		{
			MoveSideRight(frame_time * camera_speed);
		}
		if (controller->left_stick_y_axis() < 0)
		{
			MoveForward(frame_time * camera_speed);
		}
		if (controller->left_stick_y_axis() > 0)
		{
			MoveBackwards(frame_time * camera_speed);
		}
		// right stick - yaw and pitch controll
		if (controller->right_stick_x_axis() < 0)
		{
			subtractYaw(camera_speed * camera_speed, frame_time);
		}
		if (controller->right_stick_x_axis() > 0)
		{
			AddYaw(camera_speed * camera_speed, frame_time);

		}
		if (controller->right_stick_y_axis() < 0)
		{
			AddPitch(camera_speed * camera_speed, frame_time);
		}
		if (controller->right_stick_y_axis() > 0)
		{
			subtractPitch(camera_speed * camera_speed, frame_time);
		}
		// buttons
		// handle input
		if (controller->buttons_down() & gef_SONY_CTRL_R2)
		{
			MoveUp(frame_time * camera_speed);
		}
		if (controller->buttons_down() & gef_SONY_CTRL_L2)
		{
			MoveDown(frame_time * camera_speed);
		}
		/* dev function 
		if (controller->buttons_pressed() & gef_SONY_CTRL_TOUCH_PAD)
		{
			DisplayCameraPosition();
		}
		*/
	}
}

void FreeCamera::CameraController(const float frame_time, const gef::TouchInputManager* touch_input, const gef::Keyboard * keyboard)
{
	//960, 544
	//const gef::TouchInputManager* touch_input = input_manager_->touch_manager();

	// initialise the mouse position
	gef::Vector2 mouse_position(0.0f, 0.0f);


#ifdef _WIN32 // Only on windows platforms
	// get a pointer to the d3d11 implementation of the TouchInputManager
	const gef::TouchInputManagerD3D11* touch_input_d3d11 = (const gef::TouchInputManagerD3D11*)touch_input;

	// get the mouse position
	mouse_position = touch_input_d3d11->mouse_position();
#endif

	if (keyboard)
	{
		float camera_speed = 10.0f;

		// left stick - pan controll
		if (keyboard->IsKeyDown(gef::Keyboard::KC_A))
		{
			MoveSideLeft(frame_time * camera_speed);
		}
		if (keyboard->IsKeyDown(gef::Keyboard::KC_D))
		{
			MoveSideRight(frame_time * camera_speed);
		}
		if (keyboard->IsKeyDown(gef::Keyboard::KC_W))
		{
			MoveForward(frame_time * camera_speed);
		}
		if (keyboard->IsKeyDown(gef::Keyboard::KC_S))
		{
			MoveBackwards(frame_time * camera_speed);
		}
		// right stick - yaw and pitch controll
		if (keyboard->IsKeyDown(gef::Keyboard::KC_LEFT))
		{
			subtractYaw(camera_speed * camera_speed, frame_time);
		}
		if (keyboard->IsKeyDown(gef::Keyboard::KC_RIGHT))
		{
			AddYaw(camera_speed * camera_speed, frame_time);

		}
		if (keyboard->IsKeyDown(gef::Keyboard::KC_DOWN))
		{
			AddPitch(camera_speed * camera_speed, frame_time);
		}
		if (keyboard->IsKeyDown(gef::Keyboard::KC_UP))
		{
			subtractPitch(camera_speed * camera_speed, frame_time);
		}
		// buttons
		// handle input
		if (keyboard->IsKeyDown(gef::Keyboard::KC_E))
		{
			MoveUp(frame_time * camera_speed);
		}
		if (keyboard->IsKeyDown(gef::Keyboard::KC_Q))
		{
			MoveDown(frame_time * camera_speed);
		}
		/* dev function
		if (controller->buttons_pressed() & gef_SONY_CTRL_TOUCH_PAD)
		{
		DisplayCameraPosition();
		}
		*/
	}
}
