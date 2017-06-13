#include "free_camera.h"
#include <system/debug_log.h>
#include <math.h>

const float PI = 3.1415f;

free_camera::free_camera() {
	Update();
	// first view I tried
	/*SetYaw(60.0f);
	SetPitch(-10.0f);
	SetRoll(0.0f);
	position_.set_value(-5.0f, 4.0f, 8.0f);*/

	// Right side view
	/*SetYaw(80.0f);
	SetPitch(-15.0f);
	SetRoll(0.0f);
	position_.set_value(-6.4f, 3.8f, 3.9f);*/

	// TPP player position
	SetYaw(90.0f);
	SetPitch(-12.0f);
	SetRoll(0.0f);
	position_.set_value(-8.0f, 3.5f, 0.0f);
}

void free_camera::Update() {
	//temp values for sin/cos
	float cosR, cosP, cosY;	
	float sinR, sinP, sinY;

	// roll_, pitch_ and yaw_ are variables stored by the free_camera
	// rotation handling - these values are calculated only once, when rotation changes, not every frame. 
	cosY = cosf((yaw_ * PI) / 180.0f);
	cosP = cosf((pitch_ * PI) / 180.0f);
	cosR = cosf((roll_ * PI) / 180.0f);
	sinY = sinf((yaw_  * PI) / 180.0f);
	sinP = sinf((pitch_  * PI) / 180.0f);
	sinR = sinf((roll_  * PI) / 180.0f);
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

void free_camera::DisplayCameraPosition() const
{
	gef::DebugOut("\nSetYaw(%.1ff);\n", yaw_);
	gef::DebugOut("SetPitch(%.1ff);\n", pitch_);
	gef::DebugOut("SetRoll(%.1ff);\n", roll_);
	gef::DebugOut("position_.set_value(%.1ff, %.1ff, %.1ff);\n", position_.x(), position_.y(), position_.z());
}
