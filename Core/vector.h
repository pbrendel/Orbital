// pbrendel (c) 2019-21

#pragma once

#include "types.h"
#include "math.h"

#include <algorithm>

//////////////////////////////////////////////////////////////////////////

template< typename T >
struct vec2
{
	T x;
	T y;

	inline vec2()
	{}

	inline vec2( T _x, T _y )
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

	inline vec3()
	{}

	inline vec3( T _x, T _y, T _z )
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

	inline vec4()
	{}

	inline vec4( T _x, T _y, T _z, T _w )
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

//////////////////////////////////////////////////////////////////////////

typedef vec2<uint> uint2;
typedef vec3<uint> uint3;
typedef vec4<uint> uint4;
typedef vec2<float> float2;
typedef vec3<float> float3;
typedef vec4<float> float4;
typedef vec2<byte> bool2;
typedef vec3<byte> bool3;
typedef vec4<byte> bool4;

//////////////////////////////////////////////////////////////////////////

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
IMPLEMENT_VEC_SCAL_OPERATOR_4( uint4, uint, + );
IMPLEMENT_VEC_SCAL_OPERATOR_4( uint4, uint, - );
IMPLEMENT_VEC_SCAL_OPERATOR_4( uint4, uint, >> );
IMPLEMENT_VEC_SCAL_OPERATOR_4( uint4, uint, << );
IMPLEMENT_SCAL_OPERATOR_4( uint4, uint, *= );
IMPLEMENT_SCAL_OPERATOR_4( uint4, uint, /= );
IMPLEMENT_SCAL_OPERATOR_4( uint4, uint, += );
IMPLEMENT_SCAL_OPERATOR_4( uint4, uint, -= );
IMPLEMENT_VEC_UNARY_OPERATOR_4( uint4, ~ );
IMPLEMENT_VEC_REL_OPERATOR_4( uint4, == );
IMPLEMENT_VEC_REL_OPERATOR_4( uint4, != );
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
IMPLEMENT_VEC_SCAL_OPERATOR_3( uint3, uint, + );
IMPLEMENT_VEC_SCAL_OPERATOR_3( uint3, uint, - );
IMPLEMENT_VEC_SCAL_OPERATOR_3( uint3, uint, >> );
IMPLEMENT_VEC_SCAL_OPERATOR_3( uint3, uint, << );
IMPLEMENT_SCAL_OPERATOR_3( uint3, uint, *= );
IMPLEMENT_SCAL_OPERATOR_3( uint3, uint, /= );
IMPLEMENT_SCAL_OPERATOR_3( uint3, uint, += );
IMPLEMENT_SCAL_OPERATOR_3( uint3, uint, -= );
IMPLEMENT_VEC_UNARY_OPERATOR_3( uint3, ~ );
IMPLEMENT_VEC_REL_OPERATOR_3( uint3, == );
IMPLEMENT_VEC_REL_OPERATOR_3( uint3, != );
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
IMPLEMENT_VEC_SCAL_OPERATOR_2( uint2, uint, + );
IMPLEMENT_VEC_SCAL_OPERATOR_2( uint2, uint, - );
IMPLEMENT_VEC_SCAL_OPERATOR_2( uint2, uint, >> );
IMPLEMENT_VEC_SCAL_OPERATOR_2( uint2, uint, << );
IMPLEMENT_SCAL_OPERATOR_2( uint2, uint, *= );
IMPLEMENT_SCAL_OPERATOR_2( uint2, uint, /= );
IMPLEMENT_SCAL_OPERATOR_2( uint2, uint, += );
IMPLEMENT_SCAL_OPERATOR_2( uint2, uint, -= );
IMPLEMENT_VEC_UNARY_OPERATOR_2( uint2, ~ );
IMPLEMENT_VEC_REL_OPERATOR_2( uint2, == );
IMPLEMENT_VEC_REL_OPERATOR_2( uint2, != );
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
IMPLEMENT_VEC_SCAL_OPERATOR_4( float4, float, + );
IMPLEMENT_VEC_SCAL_OPERATOR_4( float4, float, - );
IMPLEMENT_SCAL_OPERATOR_4( float4, float, *= );
IMPLEMENT_SCAL_OPERATOR_4( float4, float, /= );
IMPLEMENT_SCAL_OPERATOR_4( float4, float, += );
IMPLEMENT_SCAL_OPERATOR_4( float4, float, -= );
IMPLEMENT_VEC_UNARY_OPERATOR_4( float4, - );
IMPLEMENT_VEC_REL_OPERATOR_4( float4, == );
IMPLEMENT_VEC_REL_OPERATOR_4( float4, != );
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
IMPLEMENT_VEC_SCAL_OPERATOR_3( float3, float, + );
IMPLEMENT_VEC_SCAL_OPERATOR_3( float3, float, - );
IMPLEMENT_SCAL_OPERATOR_3( float3, float, *= );
IMPLEMENT_SCAL_OPERATOR_3( float3, float, /= );
IMPLEMENT_SCAL_OPERATOR_3( float3, float, += );
IMPLEMENT_SCAL_OPERATOR_3( float3, float, -= );
IMPLEMENT_VEC_UNARY_OPERATOR_3( float3, - );
IMPLEMENT_VEC_REL_OPERATOR_3( float3, == );
IMPLEMENT_VEC_REL_OPERATOR_3( float3, != );
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
IMPLEMENT_VEC_SCAL_OPERATOR_2( float2, float, + );
IMPLEMENT_VEC_SCAL_OPERATOR_2( float2, float, - );
IMPLEMENT_SCAL_OPERATOR_2( float2, float, *= );
IMPLEMENT_SCAL_OPERATOR_2( float2, float, /= );
IMPLEMENT_SCAL_OPERATOR_2( float2, float, += );
IMPLEMENT_SCAL_OPERATOR_2( float2, float, -= );
IMPLEMENT_VEC_UNARY_OPERATOR_2( float2, - );
IMPLEMENT_VEC_REL_OPERATOR_2( float2, == );
IMPLEMENT_VEC_REL_OPERATOR_2( float2, != );
IMPLEMENT_VEC_REL_OPERATOR_2( float2, < );
IMPLEMENT_VEC_REL_OPERATOR_2( float2, <= );
IMPLEMENT_VEC_REL_OPERATOR_2( float2, > );
IMPLEMENT_VEC_REL_OPERATOR_2( float2, >= );

//////////////////////////////////////////////////////////////////////////

#define IMPLEMENT_VEC_FUNC( type, name, impl )									\
	inline type##2 name( const type##2 &v )										\
	{																			\
		return type##2( impl( v.x ), impl( v.y ) );								\
	}																			\
	inline type##3 name( const type##3 &v )										\
	{																			\
		return type##3( impl( v.x ), impl( v.y ), impl( v.y ) );				\
	}																			\
	inline type##4 name( const type##4 &v )										\
	{																			\
		return type##4( impl( v.x ), impl( v.y ), impl( v.z ), impl( v. w ) );	\
	}																			

#define IMPLEMENT_VEC_VEC_FUNC( type, name, impl )								\
	inline type##2 name( const type##2 &a, const type##2 &b )					\
	{																			\
		return type##2( impl( a.x, b.x ), impl( a.y, b.y ) );					\
	}																			\
	inline type##3 name( const type##3 &a, const type##3 &b )					\
	{																			\
		return type##3( impl( a.x, b.x ), impl( a.y, b.y ), impl( a.z, b.z ) );	\
	}																			\
	inline type##4 name( const type##4 &a, const type##4 &b )					\
	{																			\
		return type##4( impl( a.x, b.x ), impl( a.y, b.y ), impl( a.z, b.z ), impl( a.w, b.w ) );	\
	}																			

#define IMPLEMENT_VEC_VEC_VEC_FUNC( type, name, impl )								\
	inline type##2 name( const type##2 &a, const type##2 &b, const type##2 &c )		\
	{																				\
		return type##2( impl( a.x, b.x, c.x ), impl( a.y, b.y, c.y ) );				\
	}																				\
	inline type##3 name( const type##3 &a, const type##3 &b, const type##3 &c )		\
	{																				\
		return type##3( impl( a.x, b.x, c.x ), impl( a.y, b.y, c.y ), impl( a.z, b.z, c.z ) );	\
	}																				\
	inline type##4 name( const type##4 &a, const type##4 &b, const type##4 &c )		\
	{																				\
		return type##4( impl( a.x, b.x, c.x ), impl( a.y, b.y, c.y ), impl( a.z, b.z, c.z ), impl( a.w, b.w, c.w ) );	\
	}																			


IMPLEMENT_VEC_FUNC( float, abs, fabs );
IMPLEMENT_VEC_FUNC( float, frac, frac );
IMPLEMENT_VEC_FUNC( float, round, roundf );
IMPLEMENT_VEC_FUNC( float, saturate, saturate );
IMPLEMENT_VEC_FUNC( float, sign, sign );
IMPLEMENT_VEC_VEC_FUNC( float, min, ( std::min ) );
IMPLEMENT_VEC_VEC_FUNC( float, max, ( std::max ) );
IMPLEMENT_VEC_VEC_FUNC( float, step, step );
IMPLEMENT_VEC_VEC_VEC_FUNC( float, clamp, clamp );
IMPLEMENT_VEC_VEC_VEC_FUNC( float, min3, min3 );
IMPLEMENT_VEC_VEC_VEC_FUNC( float, max3, max3 );

IMPLEMENT_VEC_VEC_FUNC( uint, min, ( std::min ) );
IMPLEMENT_VEC_VEC_FUNC( uint, max, ( std::max ) );
IMPLEMENT_VEC_VEC_FUNC( uint, step, step );
IMPLEMENT_VEC_VEC_VEC_FUNC( uint, clamp, clamp );
IMPLEMENT_VEC_VEC_VEC_FUNC( uint, min3, min3 );
IMPLEMENT_VEC_VEC_VEC_FUNC( uint, max3, max3 );

float3 cross( const float3 &a, const float3 &b );
float dot( const float2 &a, const float2 &b );
float dot( const float3 &a, const float3 &b );
float dot( const float4 &a, const float4 &b );
float length( const float2 &v );
float length( const float3 &v );
float length( const float4 &v );
float2 normalize( const float2 &v );
float3 normalize( const float3 &v );
float4 normalize( const float4 &v );
