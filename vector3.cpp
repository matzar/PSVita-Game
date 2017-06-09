#include "Vector3.h"

Vector3::Vector3(float x, float y, float z) {
	this->x = x;
	this->y = y;
	this->z = z;
}

Vector3::Vector3(float construct[3]) {
	this->x = construct[0];
	this->y = construct[1];
	this->z = construct[2];
}

Vector3 Vector3::copy() {
	Vector3 copy(this->x,
				 this->y,
				 this->z);

	return copy;
}

bool Vector3::equals(const Vector3& v2, float epsilon) {
	return ((fabsf(this->x - v2.x) < epsilon) &&
			(fabsf(this->y - v2.y) < epsilon) &&
			(fabsf(this->z - v2.z) < epsilon));
}

bool Vector3::equals(const Vector3& v2) {
	return equals(v2, 0.00001f);
}


float Vector3::length() {
	return static_cast<float>(sqrt(this->lengthSquared()));
}

float Vector3::lengthSquared() {
	return (
		this->x*this->x +
		this->y*this->y +
		this->z*this->z
		);
}

void Vector3::normalise() {
	float mag = this->length();
	if (mag) {
		float multiplier = 1.0f / mag;
		this->x *= multiplier;
		this->y *= multiplier;
		this->z *= multiplier;
	}
}

Vector3 Vector3::normalised() {
	Vector3 norm(x, y, z);
	norm.normalise();
	return norm;
}

Vector3 Vector3::cross(const Vector3& v2) {
	Vector3 cross(
		(this->y * v2.z - this->z * v2.y),
		(this->z * v2.x - this->x * v2.z),
		(this->x * v2.y - this->y * v2.x)
		);
	return cross;
}

void Vector3::subtract(const Vector3& v1, float scale) {
	this->x -= (v1.x*scale);
	this->y -= (v1.y*scale);
	this->z -= (v1.z*scale);
}

void Vector3::set(float x, float y, float z) {
	this->x = x;
	this->y = y;
	this->z = z;
}

void Vector3::set(float translate[3]) {
	this->x = translate[0];
	this->y = translate[1];
	this->z = translate[2];
}

void Vector3::setX(float x) {
	this->x = x;
}

void Vector3::setY(float y) {
	this->y = y;
}

void Vector3::setZ(float z) {
	this->z = z;
}

float Vector3::getX() {
	return this->x;
}

float Vector3::getY() {
	return this->y;
}

float Vector3::getZ() {
	return this->z;
}

float Vector3::dot(const Vector3& v2) {
	return (this->x*v2.x +
			this->y*v2.y +
			this->z*v2.z
			);
}

void Vector3::scale(float scale) {
	((this->x) * scale);
	((this->y) * scale);
	((this->z) * scale);
}

void Vector3::add(const Vector3& v1, float scale) {
	this->x += (v1.x*scale);
	this->y += (v1.y*scale);
	this->z += (v1.z*scale);
}

Vector3 Vector3::operator+(const Vector3& v2) {
	return Vector3(this->x + v2.x, this->y + v2.y, this->z + v2.z);
}

Vector3 Vector3::operator-(const Vector3& v2) {
	return Vector3(this->x - v2.x, this->y - v2.y, this->z - v2.z);
}

Vector3& Vector3::operator+=(const Vector3& v2) {
	this->x += v2.x;
	this->y += v2.y;
	this->z += v2.z;
	return *this;
}

Vector3& Vector3::operator-=(const Vector3& v2) {
	this->x -= v2.x;
	this->y -= v2.y;
	this->z -= v2.z;
	return *this;
}

void Vector3::operator=(const Vector3& v2) {
	this->x = v2.x;
	this->y = v2.y;
	this->z = v2.z;
}

void Vector3::operator=(float* array) {
	this->x = array[0];
	this->y = array[1];
	this->z = array[2];
}
