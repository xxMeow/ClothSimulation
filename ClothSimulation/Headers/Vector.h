#pragma once

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include <vector>
#include <algorithm>
#include <iostream>

class vec2
{
public:
	double x;
	double y;
public:
	vec2(void)
	{
		x = y = 0.0;
	}
	vec2(double x0, double y0)
	{
		x = x0;
		y = y0;
	}
	~vec2(void)
	{
	}
public:
 
	double	getX(void) { return x; }
	double	getY(void) { return y; }
public:
	void operator=(vec2 &v)
	{
		x = v.x;
		y = v.y;
	}
	vec2 operator+(vec2 v)
	{
		vec2 res;
		res.x = x + v.x;
		res.y = y + v.y;
		return res;
	}
	vec2 operator-(vec2 v)
	{
		vec2 res;
		res.x = x - v.x;
		res.y = y - v.y;
		return res;
	}
	vec2 operator/(double v)
	{
		vec2 res;
		res.x = x / v;
		res.y = y / v;
		return res;
	}
	vec2 operator*(double v)
	{
		vec2 res;
		res.x = x*v;
		res.y = y*v;
		return res;
	}
	vec2 &operator+=(vec2 v)
	{
		x += v.x;
		y += v.y;
		return *this;
	}
	vec2 &operator-=(vec2 v)
	{
		x -= v.x;
		y -= v.y;
		return *this;
	}

	double length()
	{
		return sqrt(x*x + y*y);
	}
	double dist()
	{
		return sqrt(x*x + y*y);
	}
	double dist(vec2 p)
	{
		return sqrt((x - p.x)*(x - p.x) + (y - p.y)*(y - p.y));
	}
};

class vec3
{

public:
	vec3()
	{
		x = y = z = 0.0f;

	}
	vec3(double x, double y, double z){
		this->x = x;
		this->y = y;
		this->z = z;
	}
	~vec3(){}
public:


	double		x;
	double		y;
	double		z;
	double	getX(void) { return x; }
	double	getY(void) { return y; }
	double	getZ(void) { return z; }
	vec3 operator+(vec3 v)
	{
		vec3 result = (*this);
		result.x += v.x;
		result.y += v.y;
		result.z += v.z;
		return result;
	}
	vec3 operator-(vec3 v)
	{
		vec3 result = (*this);
		result.x -= v.x;
		result.y -= v.y;
		result.z -= v.z;
		return result;
	}

	vec3 &operator+=(vec3 v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}
	vec3 &operator-=(vec3 v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
	}
	vec3 operator*(double val)
	{
		vec3 result = (*this);
		result.x *= val;
		result.y *= val;
		result.z *= val;
		return result;
	}


	vec3 operator/(double val)
	{
		vec3 result = (*this);
		result.x /= val;
		result.y /= val;
		result.z /= val;
		return result;
	}

	friend vec3 operator*(double val, vec3 v)
	{
		v.x *= val;
		v.y *= val;
		v.z *= val;

		return v;
	}
	friend vec3 operator/(double val, vec3 v)
	{
		v.x /= val;
		v.y /= val;
		v.z /= val;

		return v;
	}

	vec3 Cross(vec3 v)
	{
		vec3 result;
		result.x = y*v.z - z*v.y;
		result.y = z*v.x - x*v.z;
		result.z = x*v.y - y*v.x;
		return result;
	}
	double length()
	{
		return sqrt(x*x + y*y + z*z);
	}
	void Normalize()
	{
		double w = length();
		if (w < 0.00001) return;
		x /= w;
		y /= w;
		z /= w;
	}
	double  dot(vec3 v)
	{
		return (x*v.x + y*v.y + z*v.z);
	}
	double dist(vec3 v)
	{
		return sqrt(pow(x - v.x, 2) + pow(y - v.y, 2) + pow(z - v.z, 2));
	}
	bool operator ==(vec3 &v)
	{
		return x == v.x && y == v.y&&z == v.z;
	}
	bool operator!=(vec3 &v)
	{
		return x != v.x || y != v.y || z != v.z;
	}
	void setZeroVector(){
		x = y = z = 0.0f;
	}

};
