#pragma once
#include <math.h>

class Vector3 {

public:
	Vector3(float x = 0, float y = 0, float z = 0);
	Vector3(float construct[3]);
	Vector3 copy();


	void set(float x, float y, float z);
	void set(float translate[3]);
	void setX(float x);
	void setY(float y);
	void setZ(float z);

	float getX();
	float getY();
	float getZ();

	void add(const Vector3& v1, float scale = 1.0);
	void subtract(const Vector3& v1, float scale = 1.0);
	void scale(float scale);

	float dot(const Vector3& v2);
	Vector3 cross(const Vector3& v2);

	void normalise();
	Vector3 normalised();
	float length();
	float lengthSquared();

	bool equals(const Vector3& v2, float epsilon);
	bool equals(const Vector3& v2);

	Vector3 operator+(const Vector3& v2);
	Vector3 operator-(const Vector3& v2);
	void operator=(const Vector3& v2);
	void operator=(float* array);

	Vector3& operator+=(const Vector3& v2);
	Vector3& operator-=(const Vector3& v2);

	float x;
	float y;
	float z;
};