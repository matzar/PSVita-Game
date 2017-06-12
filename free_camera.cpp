#include "free_camera.h"
#include <math.h>

free_camera::free_camera() {
	update();
	setYaw(60.0f);
	setPitch(-10.0f);
	setRoll(0.0f);
	position_.set_value(-5.0f, 4.0f, 8.0f);
	forward_.set_value(0.0f, 0.0f, 0.0f);
	look_at_.set_value(0.0f, 0.0f, 0.0f);
	side_.set_value(0.0f, 0.0f, 0.0f);
	up_.set_value(0.0f, 0.0f, 0.0f);
}

void free_camera::update() {
	float cosR, cosP, cosY;	//temp values for sin/cos from 
	float sinR, sinP, sinY;
	// Roll, Pitch and Yall are variables stored by the free_camera
	// handle rotation
	// Only want to calculate these values once, when rotation changes, not every frame. 
	cosY = cosf((yaw_ * 3.1415f) / 180.0f);
	cosP = cosf((pitch_ * 3.1415f) / 180.0f);
	cosR = cosf((roll_ * 3.1415f) / 180.0f);
	sinY = sinf((yaw_  *3.1415f) / 180.0f);
	sinP = sinf((pitch_  *3.1415f) / 180.0f);
	sinR = sinf((roll_  *3.1415f) / 180.0f);
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
