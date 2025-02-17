/******************************************************************************
Author:Rich Davison
Description: Some random variables and functions, for lack of a better place 
to put them.

-_-_-_-_-_-_-_,------,   
_-_-_-_-_-_-_-|   /\_/\   NYANYANYAN
-_-_-_-_-_-_-~|__( ^ .^) /
_-_-_-_-_-_-_-""  ""   

*//////////////////////////////////////////////////////////////////////////////

#pragma once

#define WEEK_2_CODE
#define USE_MD5MESH
#define WEEK_3_CODE

//It's pi(ish)...
static const float		PI = 3.14159265358979323846f;	

//It's pi...divided by 360.0f!
static const float		PI_OVER_360 = PI / 360.0f;

//Radians to degrees
static inline double RadToDeg(const double deg)	{
	return deg * 180.0 / PI;
};

//Degrees to radians
static inline double DegToRad(const double rad)	{
	return rad * PI / 180.0;
};

//I blame Microsoft...
#define nclgl_max(a,b)    (((a) > (b)) ? (a) : (b))
#define nclgl_min(a,b)    (((a) < (b)) ? (a) : (b))

#define SHADERDIR	"../../Shaders/"
#define MESHDIR		"../../Meshes/"
#define TEXTUREDIR  "../../Textures/"
#define SOUNDSDIR	"../../Sounds/"