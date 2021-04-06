// pbrendel (c) 2021

#pragma once

#include "vector.h"


template< typename T >
union mat4x4
{
	typedef vec4<T> RowType;

	mat4x4()
	{}

	struct
	{
		RowType x;
		RowType y;
		RowType z;
		RowType w;
	};
	struct
	{
		float _m00, _m01, _m02, _m03;
		float _m10, _m11, _m12, _m13;
		float _m20, _m21, _m22, _m23;
		float _m30, _m31, _m32, _m33;
	};
	RowType rows[4];
	T m[16];

	inline RowType &operator[]( uint index )
	{
		return rows[index];
	}
};


template< typename T >
union mat3x3
{
	typedef vec3<T> RowType;

	mat3x3()
	{}

	struct
	{
		RowType x;
		RowType y;
		RowType z;
	};
	struct
	{
		float _m00, _m01, _m02;
		float _m10, _m11, _m12;
		float _m20, _m21, _m22;
	};
	RowType rows[3];
	T m[9];

	inline RowType &operator[]( uint index )
	{
		return rows[index];
	}
};


template< typename T >
union mat2x2
{
	typedef vec2<T> RowType;

	mat2x2()
	{}

	struct
	{
		RowType x;
		RowType y;
	};
	struct
	{
		float _m00, _m01;
		float _m10, _m11;
	};
	RowType rows[2];
	T m[4];

	inline RowType &operator[]( uint index )
	{
		return rows[index];
	}
};


typedef mat4x4<float> float4x4;
typedef mat3x3<float> float3x3;
typedef mat2x2<float> float2x2;

typedef mat4x4<uint> uint4x4;
typedef mat3x3<uint> uint3x3;
typedef mat2x2<uint> uint2x2;


template< typename T >
vec4<T> mul( const vec4<T> &v, const mat4x4<T> &m )
{
	return m.x * v.x + m.y * v.y + m.z * v.z + m.w * v.w;
}


template< typename T >
vec3<T> mul( const vec3<T> &v, const mat3x3<T> &m )
{
	return m.x * v.x + m.y * v.y + m.z * v.z;
}


template< typename T >
vec2<T> mul( const vec2<T> &v, const mat2x2<T> &m )
{
	return m.x * v.x + m.y * v.y;
}


void IdentityMatrix( float4x4 &outMatrix );
void IdentityMatrix( float3x3 &outMatrix );
void IdentityMatrix( float2x2 &outMatrix );
void TranslationMatrix( const float3 &t, float4x4 &outMatrix );
void InfiniteProjectionMatrix( float tanHalfFovX, float tanHalfFovY, float nearZ, float4x4 &outMatrix );
void ClipToScreenMatrix( float screenWidth, float screenHeight, float3x3 &outMatrix );
