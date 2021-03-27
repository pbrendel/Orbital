// pbrendel (c) 2019-21

#pragma once

#include "types.h"


template< typename T >
struct vec2
{
	T x;
	T y;

	inline explicit vec2( T _x = T( 0 ), T _y = T( 0 ) )
		: x( _x )
		, y( _y )
	{}

	template< typename U >
	inline vec2( const vec2<U> &v )
	{
		x = static_cast<T>( v.x );
		y = static_cast<T>( v.y );
	}

	inline T &operator[]( uint index )
	{
		return *( &this->x + index );
	}
};


template< typename T >
union vec3
{
	struct
	{
		T x;
		T y;
		T z;
	};
	struct
	{
		vec2<T> xy;
		T z1;
	};

	inline explicit vec3( T _x = T( 0 ), T _y = T( 0 ), T _z = T( 0 ) )
		: x( _x )
		, y( _y )
		, z( _z )
	{}

	inline vec3( const vec2<T> &_xy, T _z )
		: xy( _xy )
		, z1( _z )
	{}

	inline T &operator[]( uint index )
	{
		return *( &this->x + index );
	}
};


template< typename T >
union vec4
{
	struct
	{
		T x;
		T y;
		T z;
		T w;
	};
	struct
	{
		vec3<T> xyz;
		T w1;
	};
	struct
	{
		vec2<T> xy;
		vec2<T> zw;
	};

	inline explicit vec4( T _x = T( 0 ), T _y = T( 0 ), T _z = T( 0 ), T _w = T( 0 ) )
		: x( _x )
		, y( _y )
		, z( _z )
		, w( _w )
	{}

	inline vec4( const vec3<T> &_xyz, T _w )
		: xyz( _xyz )
		, w1( _w )
	{}

	inline vec4( const vec2<T> &_xy, const vec2<T> &_zw )
		: xy( _xy )
		, zw( _zw )
	{}

	inline vec4( const vec2<T> &_xy, float _z, float _w )
		: xy( _xy )
		, zw( vec2<T>( _z, _w ) )
	{}

	inline T &operator[]( uint index )
	{
		return *( &this->x + index );
	}
};


typedef vec2<uint> uint2;
typedef vec3<uint> uint3;
typedef vec4<uint> uint4;
typedef vec2<float> float2;
typedef vec3<float> float3;
typedef vec4<float> float4;
typedef vec2<byte> bool2;
typedef vec3<byte> bool3;
typedef vec4<byte> bool4;


template< typename T >
inline bool operator==( const vec4<T> &a, const vec4<T> &b )
{
	return a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w;
}


template< typename T >
inline bool operator==( const vec3<T> &a, const vec3<T> &b )
{
	return a.x == b.x && a.y == b.y && a.z == b.z;
}


template< typename T >
inline bool operator==( const vec2<T> &a, const vec2<T> &b )
{
	return a.x == b.x && a.y == b.y;
}


template< typename T >
inline bool operator!=( const vec4<T> &a, const vec4<T> &b )
{
	return !( a == b );
}


template< typename T >
inline bool operator!=( const vec3<T> &a, const vec3<T> &b )
{
	return !( a == b );
}


template< typename T >
inline bool operator!=( const vec2<T> &a, const vec2<T> &b )
{
	return !( a == b );
}


#define IMPLEMENT_VEC_VEC_OPERATOR_4( type, op ) inline type operator##op( const type &a, const type &b ) { type ret; ret.x = a.x op b.x; ret.y = a.y op b.y; ret.z = a.z op b.z; ret.w = a.w op b.w; return ret; }
#define IMPLEMENT_VEC_VEC_OPERATOR_3( type, op ) inline type operator##op( const type &a, const type &b ) { type ret; ret.x = a.x op b.x; ret.y = a.y op b.y; ret.z = a.z op b.z; return ret; }
#define IMPLEMENT_VEC_VEC_OPERATOR_2( type, op ) inline type operator##op( const type &a, const type &b ) { type ret; ret.x = a.x op b.x; ret.y = a.y op b.y; return ret; }
#define IMPLEMENT_VEC_OPERATOR_4( type, op ) inline type& operator##op( type &a, const type &b ) { a.x op b.x; a.y op b.y; a.z op b.z; a.w op b.w; return a; }
#define IMPLEMENT_VEC_OPERATOR_3( type, op ) inline type& operator##op( type &a, const type &b ) { a.x op b.x; a.y op b.y; a.z op b.z; return a; }
#define IMPLEMENT_VEC_OPERATOR_2( type, op ) inline type& operator##op( type &a, const type &b ) { a.x op b.x; a.y op b.y; return a; }
#define IMPLEMENT_VEC_SCAL_OPERATOR_4( type, scal, op ) inline type operator##op( const type &v, scal f ) { type ret; ret.x = v.x op f; ret.y = v.y op f; ret.z = v.z op f; ret.w = v.w op f; return ret; } \
                                                        inline type operator##op( scal f, const type &v ) { type ret; ret.x = v.x op f; ret.y = v.y op f; ret.z = v.z op f; ret.w = v.w op f; return ret; }
#define IMPLEMENT_VEC_SCAL_OPERATOR_3( type, scal, op ) inline type operator##op( const type &v, scal f ) { type ret; ret.x = v.x op f; ret.y = v.y op f; ret.z = v.z op f; return ret; } \
                                                        inline type operator##op( scal f, const type &v ) { type ret; ret.x = v.x op f; ret.y = v.y op f; ret.z = v.z op f; return ret; }
#define IMPLEMENT_VEC_SCAL_OPERATOR_2( type, scal, op ) inline type operator##op( const type &v, scal f ) { type ret; ret.x = v.x op f; ret.y = v.y op f; return ret; } \
                                                        inline type operator##op( scal f, const type &v ) { type ret; ret.x = v.x op f; ret.y = v.y op f; return ret; }
#define IMPLEMENT_SCAL_OPERATOR_4( type, scal, op ) inline type &operator##op( type &v, scal f ) { v.x op f; v.y op f; v.z op f; v.w op f; return v; }
#define IMPLEMENT_SCAL_OPERATOR_3( type, scal, op ) inline type &operator##op( type &v, scal f ) { v.x op f; v.y op f; v.z op f; return v; }
#define IMPLEMENT_SCAL_OPERATOR_2( type, scal, op ) inline type &operator##op( type &v, scal f ) { v.x op f; v.y op f; return v; }
#define IMPLEMENT_VEC_UNARY_OPERATOR_4( type, op ) inline type operator##op( const type &v ) { return type( op v.x, op v.y, op v.z, op v.w ); }
#define IMPLEMENT_VEC_UNARY_OPERATOR_3( type, op ) inline type operator##op( const type &v ) { return type( op v.x, op v.y, op v.z ); }
#define IMPLEMENT_VEC_UNARY_OPERATOR_2( type, op ) inline type operator##op( const type &v ) { return type( op v.x, op v.y ); }
#define IMPLEMENT_VEC_REL_OPERATOR_4( type, op ) inline bool4 operator##op( const type &a, const type &b ) { return bool4( a.x op b.x ? 1 : 0, a.y op b.y ? 1 : 0, a.z op b.z ? 1 : 0, a.w op b.w ? 1 : 0 ); }
#define IMPLEMENT_VEC_REL_OPERATOR_3( type, op ) inline bool3 operator##op( const type &a, const type &b ) { return bool3( a.x op b.x ? 1 : 0, a.y op b.y ? 1 : 0, a.z op b.z ? 1 : 0 ); }
#define IMPLEMENT_VEC_REL_OPERATOR_2( type, op ) inline bool2 operator##op( const type &a, const type &b ) { return bool2( a.x op b.x ? 1 : 0, a.y op b.y ? 1 : 0 ); }

IMPLEMENT_VEC_VEC_OPERATOR_4( uint4, + );
IMPLEMENT_VEC_VEC_OPERATOR_4( uint4, - );
IMPLEMENT_VEC_VEC_OPERATOR_4( uint4, * );
IMPLEMENT_VEC_VEC_OPERATOR_4( uint4, / );
IMPLEMENT_VEC_VEC_OPERATOR_4( uint4, & );
IMPLEMENT_VEC_VEC_OPERATOR_4( uint4, | );
IMPLEMENT_VEC_OPERATOR_4( uint4, += );
IMPLEMENT_VEC_OPERATOR_4( uint4, -= );
IMPLEMENT_VEC_OPERATOR_4( uint4, *= );
IMPLEMENT_VEC_OPERATOR_4( uint4, /= );
IMPLEMENT_VEC_SCAL_OPERATOR_4( uint4, uint, * );
IMPLEMENT_VEC_SCAL_OPERATOR_4( uint4, uint, / );
IMPLEMENT_VEC_SCAL_OPERATOR_4( uint4, uint, >> );
IMPLEMENT_VEC_SCAL_OPERATOR_4( uint4, uint, << );
IMPLEMENT_SCAL_OPERATOR_4( uint4, uint, *= );
IMPLEMENT_SCAL_OPERATOR_4( uint4, uint, /= );
IMPLEMENT_VEC_UNARY_OPERATOR_4( uint4, ~ );
IMPLEMENT_VEC_REL_OPERATOR_4( uint4, < );
IMPLEMENT_VEC_REL_OPERATOR_4( uint4, <= );
IMPLEMENT_VEC_REL_OPERATOR_4( uint4, > );
IMPLEMENT_VEC_REL_OPERATOR_4( uint4, >= );
IMPLEMENT_VEC_VEC_OPERATOR_3( uint3, + );
IMPLEMENT_VEC_VEC_OPERATOR_3( uint3, - );
IMPLEMENT_VEC_VEC_OPERATOR_3( uint3, * );
IMPLEMENT_VEC_VEC_OPERATOR_3( uint3, / );
IMPLEMENT_VEC_VEC_OPERATOR_3( uint3, & );
IMPLEMENT_VEC_VEC_OPERATOR_3( uint3, | );
IMPLEMENT_VEC_OPERATOR_3( uint3, += );
IMPLEMENT_VEC_OPERATOR_3( uint3, -= );
IMPLEMENT_VEC_OPERATOR_3( uint3, *= );
IMPLEMENT_VEC_OPERATOR_3( uint3, /= );
IMPLEMENT_VEC_SCAL_OPERATOR_3( uint3, uint, * );
IMPLEMENT_VEC_SCAL_OPERATOR_3( uint3, uint, / );
IMPLEMENT_VEC_SCAL_OPERATOR_3( uint3, uint, >> );
IMPLEMENT_VEC_SCAL_OPERATOR_3( uint3, uint, << );
IMPLEMENT_SCAL_OPERATOR_3( uint3, uint, *= );
IMPLEMENT_SCAL_OPERATOR_3( uint3, uint, /= );
IMPLEMENT_VEC_UNARY_OPERATOR_3( uint3, ~ );
IMPLEMENT_VEC_REL_OPERATOR_3( uint3, < );
IMPLEMENT_VEC_REL_OPERATOR_3( uint3, <= );
IMPLEMENT_VEC_REL_OPERATOR_3( uint3, > );
IMPLEMENT_VEC_REL_OPERATOR_3( uint3, >= );
IMPLEMENT_VEC_VEC_OPERATOR_2( uint2, + );
IMPLEMENT_VEC_VEC_OPERATOR_2( uint2, - );
IMPLEMENT_VEC_VEC_OPERATOR_2( uint2, * );
IMPLEMENT_VEC_VEC_OPERATOR_2( uint2, / );
IMPLEMENT_VEC_VEC_OPERATOR_2( uint2, & );
IMPLEMENT_VEC_VEC_OPERATOR_2( uint2, | );
IMPLEMENT_VEC_OPERATOR_2( uint2, += );
IMPLEMENT_VEC_OPERATOR_2( uint2, -= );
IMPLEMENT_VEC_OPERATOR_2( uint2, *= );
IMPLEMENT_VEC_OPERATOR_2( uint2, /= );
IMPLEMENT_VEC_SCAL_OPERATOR_2( uint2, uint, * );
IMPLEMENT_VEC_SCAL_OPERATOR_2( uint2, uint, / );
IMPLEMENT_VEC_SCAL_OPERATOR_2( uint2, uint, >> );
IMPLEMENT_VEC_SCAL_OPERATOR_2( uint2, uint, << );
IMPLEMENT_SCAL_OPERATOR_2( uint2, uint, *= );
IMPLEMENT_SCAL_OPERATOR_2( uint2, uint, /= );
IMPLEMENT_VEC_UNARY_OPERATOR_2( uint2, ~ );
IMPLEMENT_VEC_REL_OPERATOR_2( uint2, < );
IMPLEMENT_VEC_REL_OPERATOR_2( uint2, <= );
IMPLEMENT_VEC_REL_OPERATOR_2( uint2, > );
IMPLEMENT_VEC_REL_OPERATOR_2( uint2, >= );

IMPLEMENT_VEC_VEC_OPERATOR_4( float4, + );
IMPLEMENT_VEC_VEC_OPERATOR_4( float4, - );
IMPLEMENT_VEC_VEC_OPERATOR_4( float4, * );
IMPLEMENT_VEC_VEC_OPERATOR_4( float4, / );
IMPLEMENT_VEC_OPERATOR_4( float4, += );
IMPLEMENT_VEC_OPERATOR_4( float4, -= );
IMPLEMENT_VEC_OPERATOR_4( float4, *= );
IMPLEMENT_VEC_OPERATOR_4( float4, /= );
IMPLEMENT_VEC_SCAL_OPERATOR_4( float4, float, * );
IMPLEMENT_VEC_SCAL_OPERATOR_4( float4, float, / );
IMPLEMENT_SCAL_OPERATOR_4( float4, float, *= );
IMPLEMENT_SCAL_OPERATOR_4( float4, float, /= );
IMPLEMENT_VEC_UNARY_OPERATOR_4( float4, - );
IMPLEMENT_VEC_REL_OPERATOR_4( float4, < );
IMPLEMENT_VEC_REL_OPERATOR_4( float4, <= );
IMPLEMENT_VEC_REL_OPERATOR_4( float4, > );
IMPLEMENT_VEC_REL_OPERATOR_4( float4, >= );
IMPLEMENT_VEC_VEC_OPERATOR_3( float3, + );
IMPLEMENT_VEC_VEC_OPERATOR_3( float3, - );
IMPLEMENT_VEC_VEC_OPERATOR_3( float3, * );
IMPLEMENT_VEC_VEC_OPERATOR_3( float3, / );
IMPLEMENT_VEC_OPERATOR_3( float3, += );
IMPLEMENT_VEC_OPERATOR_3( float3, -= );
IMPLEMENT_VEC_OPERATOR_3( float3, *= );
IMPLEMENT_VEC_OPERATOR_3( float3, /= );
IMPLEMENT_VEC_SCAL_OPERATOR_3( float3, float, * );
IMPLEMENT_VEC_SCAL_OPERATOR_3( float3, float, / );
IMPLEMENT_SCAL_OPERATOR_3( float3, float, *= );
IMPLEMENT_SCAL_OPERATOR_3( float3, float, /= );
IMPLEMENT_VEC_UNARY_OPERATOR_3( float3, - );
IMPLEMENT_VEC_REL_OPERATOR_3( float3, < );
IMPLEMENT_VEC_REL_OPERATOR_3( float3, <= );
IMPLEMENT_VEC_REL_OPERATOR_3( float3, > );
IMPLEMENT_VEC_REL_OPERATOR_3( float3, >= );
IMPLEMENT_VEC_VEC_OPERATOR_2( float2, + );
IMPLEMENT_VEC_VEC_OPERATOR_2( float2, - );
IMPLEMENT_VEC_VEC_OPERATOR_2( float2, * );
IMPLEMENT_VEC_VEC_OPERATOR_2( float2, / );
IMPLEMENT_VEC_OPERATOR_2( float2, += );
IMPLEMENT_VEC_OPERATOR_2( float2, -= );
IMPLEMENT_VEC_OPERATOR_2( float2, *= );
IMPLEMENT_VEC_OPERATOR_2( float2, /= );
IMPLEMENT_VEC_SCAL_OPERATOR_2( float2, float, * );
IMPLEMENT_VEC_SCAL_OPERATOR_2( float2, float, / );
IMPLEMENT_SCAL_OPERATOR_2( float2, float, *= );
IMPLEMENT_SCAL_OPERATOR_2( float2, float, /= );
IMPLEMENT_VEC_UNARY_OPERATOR_2( float2, - );
IMPLEMENT_VEC_REL_OPERATOR_2( float2, < );
IMPLEMENT_VEC_REL_OPERATOR_2( float2, <= );
IMPLEMENT_VEC_REL_OPERATOR_2( float2, > );
IMPLEMENT_VEC_REL_OPERATOR_2( float2, >= );
