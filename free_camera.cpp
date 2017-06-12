#include "free_camera.h"
#include <math.h>

free_camera::free_camera() {
	update();
	setYaw(60.0f);
	setPitch(-10.0f);
	setRoll(0.0f);
	position.set_value(	-5.0f,  4.0f,  8.0f);
	forward.set_value(	 0.0f,  0.0f,  0.0f);
	lookAt.set_value(	 0.0f,  0.0f,  0.0f);
	side.set_value(		 0.0f,  0.0f,  0.0f);
	up.set_value(		 0.0f,  0.0f,  0.0f);
}

free_camera::~free_camera() {}

void free_camera::update() {
	float cosR, cosP, cosY;	//temp values for sin/cos from 
	float sinR, sinP, sinY;
	// Roll, Pitch and Yall are variables stored by the free_camera
	// handle rotation
	// Only want to calculate these values once, when rotation changes, not every frame. 
	cosY = cosf( (Yaw * 3.1415f) / 180.0f);
	cosP = cosf( (Pitch * 3.1415f) / 180.0f);
	cosR = cosf( (Roll * 3.1415f) / 180.0f);
	sinY = sinf( (Yaw  *3.1415f) / 180.0f);
	sinP = sinf( (Pitch  *3.1415f) / 180.0f);
	sinR = sinf( (Roll  *3.1415f) / 180.0f);
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

void free_camera::moveForward(float dt) {
	position += forward * dt;
}

void free_camera::moveBackwards(float dt) {
	position -= forward * dt;
}
void free_camera::moveUp(float dt) {
	position += up * dt;
}
void free_camera::moveDown(float dt) {
	position -= up * dt;
}
void free_camera::moveSideLeft(float dt) {
	position -= side * dt;
}
void free_camera::moveSideRight(float dt) {
	position += side * dt;
}

float free_camera::getPositionX() {
	return position.x();
}
float free_camera::getPositionY() {
	return position.y();
}
float free_camera::getPositionZ() {
	return position.z();
}

float free_camera::getForwardX() {
	return forward.x();
}
float free_camera::getForwardY() {
	return forward.y();
}
float free_camera::getForwardZ() {
	return forward.z();
}

float free_camera::getLookAtX() {
	return lookAt.x();
}
float free_camera::getLookAtY() {
	return lookAt.y();
}
float free_camera::getLookAtZ() {
	return lookAt.z();
}

float free_camera::getUpX() {
	return up.x();
}
float free_camera::getUpY() {
	return up.y();
}
float free_camera::getUpZ() {
	return up.z();
}

float free_camera::getSideX() {
	return side.z();
}
float free_camera::getSideY() {
	return side.y();
}
float free_camera::getSideZ() {
	return side.x();
}

float free_camera::getYaw() {
	return Yaw;
}
float free_camera::getPitch() {
	return Pitch;
}
float free_camera::getRoll() {
	return Roll;
}

void free_camera::setLookAtX(float x) {
	lookAt.set_x(x);
}
void free_camera::setLookAtY(float y) {
	lookAt.set_y(y);
}
void free_camera::setLookAtZ(float z) {
	lookAt.set_z(z);
}

void free_camera::setYaw(float arg) {
	Yaw = arg;
}
void free_camera::setPitch(float arg) {
	Pitch = arg;
}
void free_camera::setRoll(float arg) {
	Roll = arg;
}




void free_camera::addYaw(float dt, float value) {
	Yaw += value * dt;
}
void free_camera::subtractYaw(float dt, float value) {
	Yaw -= value * dt;
}
void free_camera::addPitch(float dt, float value) {
	Pitch += value * dt;
}
void free_camera::subtractPitch(float dt, float value) {
	Pitch -= value * dt;
}
void free_camera::addRoll(float dt, float value) {
	Roll += value * dt;
}
void free_camera::subtractRoll(float dt, float value) {
	Roll -= value * dt;
}



void free_camera::updateYaw(int width, int mouseX, int speed) {
	Yaw += static_cast<float>((mouseX - (width / 2)) / speed);
}
void free_camera::updatePitch(int height, int mouseY, int speed) {
	Pitch -= static_cast<float>((mouseY - (height / 2)) / speed);
}

void normalise() {

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
