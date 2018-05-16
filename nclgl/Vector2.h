#pragma once
/*
Class:Vector2
Implements:
Author:Rich Davison
Description:VERY simple Vector2 class. Students are encouraged to modify this as necessary!

-_-_-_-_-_-_-_,------,   
_-_-_-_-_-_-_-|   /\_/\   NYANYANYAN
-_-_-_-_-_-_-~|__( ^ .^) /
_-_-_-_-_-_-_-""  ""   

*/
#include <iostream>

class Vector2 {
public:
	Vector2(void) {
		ToZero();
	}

	Vector2(const float x, const float y) {
		this->x = x;
		this->y = y;
	}

	~Vector2(void) {}

	float x;
	float y;

	void ToZero() {
		x = 0.0f;
		y = 0.0f;
	}

	inline friend std::ostream& operator<<(std::ostream& o, const Vector2& v) {
		o << "Vector2(" << v.x << "," << v.y << ")" << std::endl;
		return o;
	}

	inline Vector2  operator-(const Vector2  &a) const {
		return Vector2(x - a.x, y - a.y);
	}

	inline Vector2  operator+(const Vector2  &a) const {
		return Vector2(x + a.x, y + a.y);
	}

	inline void Normalise() {
		float length = Length();

		if (length != 0.0f) {
			length = 1.0f / length;
			x = x * length;
			y = y * length;
		}
	}

	static Vector2 Normalise(Vector2 vec) {
		vec.Normalise();
		return vec;
	}

	static float Distance(Vector2 vec1, Vector2 vec2) {
		return (vec2 - vec1).Length();
	}


	inline float Length() const {
		return sqrt((x*x) + (y*y));
	}

	inline void	Invert() {
		x = -x;
		y = -y;
	}

	Vector2	Inverse() const {
		return Vector2(-x, -y);
	}

	static float	Dot(const Vector2 &a, const Vector2 &b) {
		return (a.x*b.x) + (a.y*b.y);
	}

	//static Vector3	Cross(const Vector3 &a, const Vector3 &b) {
	//	float bz = 0;
	//	float az = 0;
	//	return Vector3((a.y*bz) - (az*b.y), (az*b.x) - (a.x*bz), (a.x*b.y) - (a.y*b.x));
	//}



	inline Vector2  operator-() const {
		return Vector2(-x, -y);
	}

	inline void operator+=(const Vector2  &a) {
		x += a.x;
		y += a.y;
	}

	inline void operator-=(const Vector2  &a) {
		x -= a.x;
		y -= a.y;
	}

	inline Vector2  operator*(const float a) const {
		return Vector2(x * a, y * a);
	}

	inline Vector2  operator*(const Vector2  &a) const {
		return Vector2(x * a.x, y * a.y);
	}

	inline Vector2  operator/(const Vector2  &a) const {
		return Vector2(x / a.x, y / a.y);
	};

	inline Vector2  operator/(const float v) const {
		return Vector2(x / v, y / v);
	};

	inline bool	operator==(const Vector2 &A)const { return (A.x == x && A.y == y) ? true : false; };
	inline bool	operator!=(const Vector2 &A)const { return (A.x == x && A.y == y) ? false : true; };
};