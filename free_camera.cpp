#include "free_camera.h"
#include <math.h>

free_camera::free_camera() {
	update();
	setYaw(60.0f);
	setPitch(-10.0f);
	setRoll(0.0f);
	position.set_value(-5.0f, 4.0f, 8.0f);
	forward.set_value(0.0f, 0.0f, 0.0f);
	lookAt.set_value(0.0f, 0.0f, 0.0f);
	side.set_value(0.0f, 0.0f, 0.0f);
	up.set_value(0.0f, 0.0f, 0.0f);
}

free_camera::~free_camera() {}

void free_camera::update() {
	float cosR, cosP, cosY;	//temp values for sin/cos from 
	float sinR, sinP, sinY;
	// Roll, Pitch and Yall are variables stored by the free_camera
	// handle rotation
	// Only want to calculate these values once, when rotation changes, not every frame. 
	cosY = cosf((Yaw * 3.1415f) / 180.0f);
	cosP = cosf((Pitch * 3.1415f) / 180.0f);
	cosR = cosf((Roll * 3.1415f) / 180.0f);
	sinY = sinf((Yaw  *3.1415f) / 180.0f);
	sinP = sinf((Pitch  *3.1415f) / 180.0f);
	sinR = sinf((Roll  *3.1415f) / 180.0f);
	// Calculate forward vector
	forward.set_x(sinY * cosP);
	forward.set_y(sinP);
	forward.set_z(cosP * -cosY);
	// Calculate lookAt vector
	lookAt.set_x(position.x() + forward.x());
	lookAt.set_y(position.y() + forward.y());
	lookAt.set_z(position.z() + forward.z());
	// Calculate up vector
	up.set_x(-cosY * sinR - sinY * sinP * cosR);
	up.set_y(cosP * cosR);
	up.set_z(-sinY * sinR - sinP * cosR * -cosY);
	// Calculate side Vector (right)
	side = forward.CrossProduct(up); // this is a cross product between the forward and up vector
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
