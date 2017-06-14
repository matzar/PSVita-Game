#include "camera.h"
#include <system/debug_log.h>
#include "maths/vector4.h"
#include <math.h>

const float PI = 3.1415f;

Camera::Camera()
{
	Update();

	//// first view I tried
	////SetCameraPosition(60.0f, -10.0f, 0.0f, gef::Vector4(-5.0f, 4.0f, 8.0f));

	//// Right side view
	////SetCameraPosition(80.0f, -15.0f, 0.0f, gef::Vector4(-6.4f, 3.8f, 3.9f));

	// TPP player position
	SetCameraPosition(90.0f, -12.0f, 0.0f, gef::Vector4(-8.0f, 3.5f, 0.0f));
}

Camera::~Camera()
{
}

void Camera::Update()
{
	//temp values for sin/cos
	float cosR, cosP, cosY;
	float sinR, sinP, sinY;

	// roll_, pitch_ and yaw_ are variables stored by the Camera
	// rotation handling - these values are calculated only once, when rotation changes, not every frame. 
	cosY = cosf((yaw_ * PI) / 180.0f);
	cosP = cosf((pitch_ * PI) / 180.0f);
	cosR = cosf((roll_ * PI) / 180.0f);
	sinY = sinf((yaw_ * PI) / 180.0f);
	sinP = sinf((pitch_ * PI) / 180.0f);
	sinR = sinf((roll_ * PI) / 180.0f);
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

void Camera::SetCameraPosition(const float& yaw, const float& pitch, const float& roll, const gef::Vector4& position)
{
	yaw_ = yaw;
	pitch_ = pitch;
	roll_ = roll;
	position_ = position;
	Update();
}

void Camera::DisplayCameraPosition() const
{
	gef::DebugOut("SetCameraPosition(%.1ff, %.1ff, %.1ff, %.1ff, %.1ff, %.1ff);\n", yaw_, pitch_, roll_, position_.x(), position_.y(), position_.z());
}