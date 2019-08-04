#pragma once
#include <string>
#include <math.h> 

#define EPSILON 0.000001f
#define Angle_To_Rad 0.017453f
#define ANGLE90 1.570796f
#define ANGLE180 3.141592f
#define ANGLE360 (ANGLE180*2.0f)

class Matrix
{
public:

	Matrix()
	{
		data[0][0] = 0;	data[0][1] = 0;	data[0][2] = 0;	data[0][3] = 0;
		data[1][0] = 0;	data[1][1] = 0;	data[1][2] = 0;	data[1][3] = 0;
		data[2][0] = 0;	data[2][1] = 0;	data[2][2] = 0;	data[2][3] = 0;
		data[3][0] = 0;	data[3][1] = 0;	data[3][2] = 0;	data[3][3] = 0;
	}
	Matrix(
		float m11, float m12, float m13, float m14,
		float m21, float m22, float m23, float m24,
		float m31, float m32, float m33, float m34,
		float m41, float m42, float m43, float m44)
	{
		data[0][0] = m11;	data[0][1] = m12;	data[0][2] = m13;	data[0][3] = m14;
		data[1][0] = m21;	data[1][1] = m22;	data[1][2] = m23;	data[1][3] = m24;
		data[2][0] = m31;	data[2][1] = m32;	data[2][2] = m33;	data[2][3] = m34;
		data[3][0] = m41;	data[3][1] = m42;	data[3][2] = m43;	data[3][3] = m44;
	}

	static Matrix Identity()
	{
		return Matrix(
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1);
	}
	static Matrix Scale(float x, float y, float z) {
		return Matrix(
			x, 0, 0, 0,
			0, y, 0, 0,
			0, 0, z, 0,
			0, 0, 0, 1);
	}
	static Matrix RotationX(float rad)
	{
		return Matrix(
			1, 0, 0, 0,
			0, cos(rad), -sin(rad), 0,
			0, sin(rad), cos(rad), 0,
			0, 0, 0, 1);
	}
	static Matrix RotationY(float rad)
	{
		return Matrix(
			cos(rad), 0, sin(rad), 0,
			0, 1, 0, 0,
			-sin(rad), 0, cos(rad), 0,
			0, 0, 0, 1);
	}
	static Matrix RotationZ(float rad)
	{
		return Matrix(
			cos(rad), -sin(rad), 0, 0,
			sin(rad), cos(rad), 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1);
	}
	static Matrix Translate(float x, float y, float z) {
		return Matrix(
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			x, y, z, 1);
	}

	void Inverse()
	{
		for (int row = 0; row < 3; ++row) {

			for (int col = row + 1; col < 4; ++col) {

				float temp = data[row][col];
				data[row][col] = data[col][row];
				data[col][row] = temp;
			}
		}
	}

	Matrix operator* (Matrix mat) {

		return Matrix(
			data[0][0] * mat.data[0][0] + data[0][1] * mat.data[1][0] + data[0][2] * mat.data[2][0] + data[0][3] * mat.data[3][0],
			data[0][0] * mat.data[0][1] + data[0][1] * mat.data[1][1] + data[0][2] * mat.data[2][1] + data[0][3] * mat.data[3][1],
			data[0][0] * mat.data[0][2] + data[0][1] * mat.data[1][2] + data[0][2] * mat.data[2][2] + data[0][3] * mat.data[3][2],
			data[0][0] * mat.data[0][3] + data[0][1] * mat.data[1][3] + data[0][2] * mat.data[2][3] + data[0][3] * mat.data[3][3],

			data[1][0] * mat.data[0][0] + data[1][1] * mat.data[1][0] + data[1][2] * mat.data[2][0] + data[1][3] * mat.data[3][0],
			data[1][0] * mat.data[0][1] + data[1][1] * mat.data[1][1] + data[1][2] * mat.data[2][1] + data[1][3] * mat.data[3][1],
			data[1][0] * mat.data[0][2] + data[1][1] * mat.data[1][2] + data[1][2] * mat.data[2][2] + data[1][3] * mat.data[3][2],
			data[1][0] * mat.data[0][3] + data[1][1] * mat.data[1][3] + data[1][2] * mat.data[2][3] + data[1][3] * mat.data[3][3],

			data[2][0] * mat.data[0][0] + data[2][1] * mat.data[1][0] + data[2][2] * mat.data[2][0] + data[2][3] * mat.data[3][0],
			data[2][0] * mat.data[0][1] + data[2][1] * mat.data[1][1] + data[2][2] * mat.data[2][1] + data[2][3] * mat.data[3][1],
			data[2][0] * mat.data[0][2] + data[2][1] * mat.data[1][2] + data[2][2] * mat.data[2][2] + data[2][3] * mat.data[3][2],
			data[2][0] * mat.data[0][3] + data[2][1] * mat.data[1][3] + data[2][2] * mat.data[2][3] + data[2][3] * mat.data[3][3],

			data[3][0] * mat.data[0][0] + data[3][1] * mat.data[1][0] + data[3][2] * mat.data[2][0] + data[3][3] * mat.data[3][0],
			data[3][0] * mat.data[0][1] + data[3][1] * mat.data[1][1] + data[3][2] * mat.data[2][1] + data[3][3] * mat.data[3][1],
			data[3][0] * mat.data[0][2] + data[3][1] * mat.data[1][2] + data[3][2] * mat.data[2][2] + data[3][3] * mat.data[3][2],
			data[3][0] * mat.data[0][3] + data[3][1] * mat.data[1][3] + data[3][2] * mat.data[2][3] + data[3][3] * mat.data[3][3]);
	}
	float Get(int row, int column) {
		return data[row][column];
	}

	std::string ToString()
	{
		std::string printS;

		for (int row = 0; row < 4; ++row) {

			for (int col = 0; col < 4; ++col) {

				printS += std::to_string(data[row][col]) + "  ";
			}

			printS += "\n";
		}

		return printS;
	}

private:

	float data[4][4];
};

class Vector2 {

public:
	float x, y;

	Vector2() {
		x = 0;
		y = 0;
	}
	Vector2(float f) {
		x = f;
		y = f;
	}
	Vector2(float _x, float _y) {
		x = _x;
		y = _y;
	}

	void operator= (float f) {
		x = f;
		y = f;
	}
	Vector2 operator* (float f) {
		return Vector2(x*f, y*f);
	}
	Vector2 operator/ (float f) {
		return Vector2(x/f, y/f);
	}
	void operator*= (float f) {
		x *= f;
		y *= f;
	}
	Vector2 operator+ (Vector2 v) {
		return Vector2(x + v.x, y + v.y);
	}
	Vector2 operator- (Vector2 v) {
		return Vector2(x - v.x, y - v.y);
	}
	void operator+= (Vector2 v) {
		x += v.x;
		y += v.y;
	}
	void operator-= (Vector2 v) {
		x -= v.x;
		y -= v.y;
	}
	Vector2 operator* (Matrix m) {

		return Vector2(
			m.Get(0,0) * x + m.Get(0,1) * y + m.Get(3,0),
			m.Get(1,0) * x + m.Get(1,1) * y + m.Get(3,1));
	}
	bool operator==(Vector2 v) {
		return (x == v.x && y == v.y);
	}
	
	static Vector2 Cross(Vector2 v) {

		return Vector2(cos(ANGLE90)*v.x + sin(ANGLE90)*v.y, -sin(ANGLE90)*v.x + cos(ANGLE90)*v.y);
	}
	static float Dot(Vector2 v1, Vector2 v2) {

		return v1.x * v2.x + v1.y*v2.y;
	}
	static float Length(Vector2 v) {

		return sqrt(v.x*v.x + v.y*v.y);
	}
	static float SLength(Vector2 v) {

		return (v.x*v.x + v.y*v.y);
	}
	static Vector2 Normalize(Vector2 v) {

		if (v.x == 0.0f && v.y == 0.0f)
			return Vector2(0.0f, 0.0f);

		float length = Length(v);
		return Vector2(v.x / length, v.y / length);
	}
	static float DirToRad(Vector2 std, Vector2 v) {

		float dotAngle = Vector2::Dot(v, std);
		float dotDir = Vector2::Dot(v, Vector2::Cross(std));

		if (dotDir >= 0) {

			return acos(dotAngle);
		}
		else {
			return ANGLE360 - acos(dotAngle);
		}
	}
};

class Vector3 {
public:
	float x, y, z;

	Vector3() {
		x = 0;
		y = 0;
		z = 0;
	}
	Vector3(float f) {
		x = f;
		y = f;
		z = f;
	}
	Vector3(float _x, float _y, float _z) {
		x = _x;
		y = _y;
		z = _z;
	}
};

class Vector4 {
public:
	float x, y, z, w;

	Vector4() {
		x = 0;
		y = 0;
		z = 0;
		w = 0;
	}
	Vector4(float _x, float _y, float _z, float _w) {
		x = _x;
		y = _y;
		z = _z;
		w = _w;
	}
	Vector4(Vector2 v1, Vector2 v2) {
		x = v1.x;
		y = v1.y;
		z = v2.x;
		w = v2.y;
	}
};

class IVector2 {
public:
	int x, y;

	IVector2() {
		x = 0;
		y = 0;
	}
	IVector2(int _x, int _y) {
		x = _x;
		y = _y;
	}
};

class IVector4 {
public:
	int x, y, z, w;

	IVector4(){
		x = 0;
		y = 0;
		z = 0;
		w = 0;
	}
	IVector4(int _x, int _y, int _z, int _w) {
		x = _x;
		y = _y;
		z = _z;
		w = _w;
	}
};
