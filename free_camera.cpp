#include "free_camera.h"
#include <system/debug_log.h>
#define _USE_MATH_DEFINES
#include <math.h>

free_camera::free_camera() {
	Update();
	/*SetYaw(60.0f);
	SetPitch(-10.0f);
	SetRoll(0.0f);
	position_.set_value(-5.0f, 4.0f, 8.0f);*/
	
	SetYaw(83.363815f);
	SetPitch(-13.393826f);
	SetRoll(0.000000f);
	position_.set_value(-7.855141f, 4.768086f, 4.618602f);
	/*forward_.set_value(0.0f, 0.0f, 0.0f);
	look_at_.set_value(0.0f, 0.0f, 0.0f);
	side_.set_value(0.0f, 0.0f, 0.0f);
	up_.set_value(0.0f, 0.0f, 0.0f);*/
}

void free_camera::Update() {
	//temp values for sin/cos
	float cosR, cosP, cosY;	
	float sinR, sinP, sinY;

	// roll_, pitch_ and yaw_ are variables stored by the free_camera
	// rotation handling - these values are calculated only once, when rotation changes, not every frame. 
	cosY = cosf((yaw_ * M_PI) / 180.0f);
	cosP = cosf((pitch_ * M_PI) / 180.0f);
	cosR = cosf((roll_ * M_PI) / 180.0f);
	sinY = sinf((yaw_  * M_PI) / 180.0f);
	sinP = sinf((pitch_  * M_PI) / 180.0f);
	sinR = sinf((roll_  * M_PI) / 180.0f);
	// Calculate forward vector
	forward_.set_x(sinY * cosP);
	forward_.set_y(sinP);
	forward_.set_z(cosP * -cosY);
	// Calculate lookAt vector
	look_at_.set_x(position_.x() + forward_.x());
	look_at_.set_y(position_.y() + forward_.y());
	look_at_.set_z(position_.z() + forward_.z());
	// Calculate up vector
	up_.set_x(-cosY * sinR - sinY * sinP * cosR);
	up_.set_y(cosP * cosR);
	up_.set_z(-sinY * sinR - sinP * cosR * -cosY);
	// Calculate side Vector (right)
	side_ = forward_.CrossProduct(up_); // this is a cross product between the forward and up vector
}

void free_camera::DisplayCameraPosition()
{
	gef::DebugOut("\nSetYaw(%ff);\n", yaw_);
	gef::DebugOut("SetPitch(%ff);\n", pitch_);
	gef::DebugOut("SetRoll(%ff);\n", roll_);
	gef::DebugOut("position_.set_value(%ff, %ff, %ff);\n", position_.x(), position_.y(), position_.z());
	/*gef::DebugOut("ForwardX %f\n", forward_.x());
	gef::DebugOut("ForwardY %f\n", forward_.y());
	gef::DebugOut("ForwardZ %f\n", forward_.z());
	gef::DebugOut("LookAtX %f\n", look_at_.x());
	gef::DebugOut("LookAtY %f\n", look_at_.y());
	gef::DebugOut("LookAtZ %f\n", look_at_.z());
	gef::DebugOut("SideX %f\n", side_.x());
	gef::DebugOut("SideY %f\n", side_.y());
	gef::DebugOut("SideZ %f\n", side_.z());
	gef::DebugOut("UpX %f\n", up_.x());
	gef::DebugOut("UpY %f\n", up_.y());
	gef::DebugOut("UpZ %f\n", up_.z());*/
}

//void free_camera::cameraControll(float dt, int width, int height, Input *input) {
//	// move camera left
//	if (input->isKeyDown('a') || input->isKeyDown('A')) {
//		moveSideLeft(dt);
//	}
//	// move camera right
//	if (input->isKeyDown('d') || input->isKeyDown('D')) {
//		moveSideRight(dt);
//	}
//	// move camera up
//	if (input->isKeyDown('w') || input->isKeyDown('W')) {
//		moveUp(dt);
//	}
//	// move camera down
//	if (input->isKeyDown('s') || input->isKeyDown('S')) {
//		moveDown(dt);
//	}
//}
