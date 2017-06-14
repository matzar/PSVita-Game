#include <input/sony_controller_input_manager.h>
#include "free_camera.h"
#include "maths\vector4.h"

#ifdef _WIN32
// only on windows platforms
#include <Windows.h>
#include <platform/d3d11/input/keyboard_d3d11.h>
#include <platform/d3d11/input/touch_input_manager_d3d11.h>
//#include <freeglut.h>
#endif 

FreeCamera::FreeCamera() : 
	input_manager_(nullptr)
{
	Update();

	// first view I tried
	//SetCameraPosition(60.0f, -10.0f, 0.0f, gef::Vector4(-5.0f, 4.0f, 8.0f));

	// Right side view
	//SetCameraPosition(80.0f, -15.0f, 0.0f, gef::Vector4(-6.4f, 3.8f, 3.9f));

	// TPP player position
	SetCameraPosition(90.0f, -12.0f, 0.0f, gef::Vector4(-8.0f, 3.5f, 0.0f));
}

void FreeCamera::CameraControll(const float frame_time, gef::SonyController* controller)
{
	float camera_speed = 10.0f;

	float left_horizontal_input = controller->left_stick_x_axis();
	float left_vertical_input = controller->left_stick_y_axis();

	float right_horizontal_input = controller->right_stick_x_axis();
	float right_vertical_input = controller->right_stick_y_axis();
	// left stick - pan controll
	if (controller->left_stick_x_axis() < 0)
	{
		camera_->MoveSideLeft(frame_time * camera_speed);
	}
	if (controller->left_stick_x_axis() > 0)
	{
		camera_->MoveSideRight(frame_time * camera_speed);
	}
	if (controller->left_stick_y_axis() < 0)
	{
		camera_->MoveForward(frame_time * camera_speed);
	}
	if (controller->left_stick_y_axis() > 0)
	{
		camera_->MoveBackwards(frame_time * camera_speed);
	}
	// right stick - yaw and pitch controll
	if (controller->right_stick_x_axis() < 0)
	{
		camera_->subtractYaw(frame_time, camera_speed * camera_speed);
	}
	if (controller->right_stick_x_axis() > 0)
	{
		camera_->AddYaw(frame_time, camera_speed * camera_speed);

	}
	if (controller->right_stick_y_axis() < 0)
	{
		camera_->AddPitch(frame_time, camera_speed * camera_speed);
	}
	if (controller->right_stick_y_axis() > 0)
	{
		camera_->subtractPitch(frame_time, camera_speed * camera_speed);
	}
	// buttons
	// handle input
	if (controller->buttons_down() & gef_SONY_CTRL_R2)
	{
		camera_->MoveUp(timeStep * camera_speed);
	}
	if (controller->buttons_down() & gef_SONY_CTRL_L2)
	{
		camera_->MoveDown(timeStep * camera_speed);
	}
	if (controller->buttons_pressed() & gef_SONY_CTRL_SELECT)
	{
		camera_->DisplayCameraPosition();
	}
	if (controller->buttons_pressed() & gef_SONY_CTRL_CROSS)
	{
		camera_->SetCameraPosition(90.0f, -12.0f, 0.0f, gef::Vector4(-8.0f, 3.5f, 0.0f));
	}

#ifdef _WIN32 // Only on windows platforms
		// if there is a keyboard, check the arrow keys to control the direction of the character
		gef::Keyboard* keyboard = input_manager_->keyboard();
		if (keyboard)
		{
			//const gef::KeyboardD3D11* keyboard_d3d11 = (const gef::KeyboardD3D11*)keyboard;
			float camera_speed = 10.0f;

			// keyboard input
			if (keyboard->IsKeyDown(gef::Keyboard::KC_W))
				camera_->MoveForward(frame_time * camera_speed);
			if (keyboard->IsKeyDown(gef::Keyboard::KC_S))
				camera_->MoveBackwards(frame_time * camera_speed);
			if (keyboard->IsKeyDown(gef::Keyboard::KC_A))
				camera_->MoveSideLeft(frame_time * camera_speed);
			if (keyboard->IsKeyDown(gef::Keyboard::KC_D))
				camera_->MoveSideRight(frame_time * camera_speed);
			if (keyboard->IsKeyDown(gef::Keyboard::KC_UP))
				camera_->AddPitch(frame_time, camera_speed * camera_speed);
			if (keyboard->IsKeyDown(gef::Keyboard::KC_DOWN))
				camera_->subtractPitch(frame_time, camera_speed * camera_speed);
			if (keyboard->IsKeyDown(gef::Keyboard::KC_LEFT))
				camera_->subtractYaw(frame_time, camera_speed * camera_speed);
			if (keyboard->IsKeyDown(gef::Keyboard::KC_RIGHT))
				camera_->AddYaw(frame_time, camera_speed * camera_speed);
			if (keyboard->IsKeyDown(gef::Keyboard::KC_R) || keyboard->IsKeyDown(gef::Keyboard::KC_NUMPAD8))
				camera_->MoveUp(frame_time * camera_speed);
			if (keyboard->IsKeyDown(gef::Keyboard::KC_F) || keyboard->IsKeyDown(gef::Keyboard::KC_NUMPAD2))
				camera_->MoveDown(frame_time * camera_speed);
		} // keyboard

			// mouse input
		const gef::TouchInputManager* touch_input = input_manager_->touch_manager();
		if (touch_input)
		{
			// initialise the mouse position
			gef::Vector2 mouse_position(0.0f, 0.0f); // left upper corner of the window

														// get a pointer to the d3d11 implementation of the TouchInputManager
			const gef::TouchInputManagerD3D11* touch_input_d3d11 = (const gef::TouchInputManagerD3D11*)touch_input;

			// get the mouse position
			mouse_position = touch_input_d3d11->mouse_position();
			// camera's Yaw mouse controll, last variable controlls speed
			//camera_->updateYaw(960, mouse_position.x, 2);
			// camera's Pitch mouse controll, last variable controlls speed
			//camera_->updatePitch(544, mouse_position.y, 2);

			if (touch_input_d3d11->is_button_down(0))
			{
				//SetCursorPos(480, 272);	
			}

			//gef::DebugOut("Mouse position x, y: %f %f\n", mouse_position.x, mouse_position.y);
		} // touch_input (mouse)
#endif // !_WIN32
	
	//camera_->moveSideLeft(frame_time);
	//camera_->moveUp(frame_time);
	//camera_->update();
}