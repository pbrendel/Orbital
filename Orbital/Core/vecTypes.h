// pbrendel (c) 2019-21

#pragma once

#include "types.h"


struct uint2
{
	uint x;
	uint y;

	inline explicit uint2( uint _x = 0, uint _y = 0 )
		: x( _x )
		, y( _y )
	{}

	inline uint &operator[]( uint index )
	{
		return *( &this->x + index );
	}
};


union uint3
{
	struct
	{
		uint x;
		uint y;
		uint z;
	};
	struct
	{
		uint2 xy;
		uint z1;
	};

	inline explicit uint3( uint _x = 0, uint _y = 0, uint _z = 0 )
		: x( _x )
		, y( _y )
		, z( _z )
	{}

	inline uint3( uint2 _xy, uint _z )
		: xy( _xy )
		, z1( _z )
	{}

	inline uint &operator[]( uint index )
	{
		return *( &this->x + index );
	}
};


union uint4
{
	struct
	{
		uint x;
		uint y;
		uint z;
		uint w;
	};
	struct
	{
		uint3 xyz;
		uint w1;
	};
	struct
	{
		uint2 xy;
		uint2 zw;
	};

	inline explicit uint4( uint _x = 0, uint _y = 0, uint _z = 0, uint _w = 0 )
		: x( _x )
		, y( _y )
		, z( _z )
		, w( _w )
	{}

	inline uint4( uint3 _xyz, uint _w )
		: xyz( _xyz )
		, w1( _w )
	{}

	inline uint4( uint2 _xy, uint _zw )
		: xy( _xy )
		, zw( _zw )
	{}

	inline uint &operator[]( uint index )
	{
		return *( &this->x + index );
	}
};


struct float2
{
	float x;
	float y;

	inline explicit float2( float _x = 0.0f, float _y = 0.0f )
		: x( _x )
		, y( _y )
	{}

	inline float2( uint2 i )
		: x( static_cast<float>( i.x ) )
		, y( static_cast<float>( i.y ) )
	{}

	inline operator uint2()
	{
		return uint2( static_cast< uint >( x ), static_cast< uint >( y ) );
	}

	inline float &operator[]( uint index )
	{
		return *( &this->x + index );
	}
};


union float3
{
	struct
	{
		float x;
		float y;
		float z;
	};
	struct
	{
		float2 xy;
		float z1;
	};

	inline explicit float3( float _x = 0.0f, float _y = 0.0f, float _z = 0.0f )
		: x( _x )
		, y( _y )
		, z( _z )
	{}

	inline float3( float2 _xy, float _z )
		: xy( _xy )
		, z1( _z )
	{}

	inline float3( uint3 i )
		: x( static_cast<float>( i.x ) )
		, y( static_cast<float>( i.y ) )
		, z( static_cast<float>( i.z ) )
	{}

	inline operator uint3()
	{
		return uint3( static_cast<uint>( x ), static_cast<uint>( y ), static_cast<uint>( z ) );
	}

	inline float &operator[]( uint index )
	{
		return *( &this->x + index );
	}
};


union float4
{
	struct
	{
		float x;
		float y;
		float z;
		float w;
	};
	struct
	{
		float3 xyz;
		float w1;
	};
	struct
	{
		float2 xy;
		float2 zw;
	};

	inline explicit float4( float _x = 0.0f, float _y = 0.0f, float _z = 0.0f, float _w = 0.0f )
		: x( _x )
		, y( _y )
		, z( _z )
		, w( _w )
	{}

	inline float4( float3 _xyz, float _w )
		: xyz( _xyz )
		, w1( _w )
	{}

	inline float4( float2 _xy, float _zw )
		: xy( _xy )
		, zw( _zw )
	{}

	inline float4( float2 _xy, float _z, float _w )
		: xy( _xy )
		, zw( float2( _z, _w ) )
	{}

	inline float4( uint4 i )
		: x( static_cast<float>( i.x ) )
		, y( static_cast<float>( i.y ) )
		, z( static_cast<float>( i.z ) )
		, w( static_cast<float>( i.w ) )
	{}

	inline operator uint4()
	{
		return uint4( static_cast<uint>( x ), static_cast<uint>( y ), static_cast<uint>( z ), static_cast<uint>( w ) );
	}

	inline float &operator[]( uint index )
	{
		return *( &this->x + index );
	}
};


inline bool operator==( const float4 &a, const float4 &b )
{
	return a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w;
}


inline bool operator==( const float3 &a, const float3 &b )
{
	return a.x == b.x && a.y == b.y && a.z == b.z;
}


inline bool operator==( const float2 &a, const float2 &b )
{
	return a.x == b.x && a.y == b.y;
}


inline bool operator!=( const float4 &a, const float4 &b )
{
	return !( a == b );
}


inline bool operator!=( const float3 &a, const float3 &b )
{
	return !( a == b );
}


inline bool operator!=( const float2 &a, const float2 &b )
{
	return !( a == b );
}


inline bool operator==( const uint4 &a, const uint4 &b )
{
	return a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w;
}


inline bool operator==( const uint3 &a, const uint3 &b )
{
	return a.x == b.x && a.y == b.y && a.z == b.z;
}


inline bool operator==( const uint2 &a, const uint2 &b )
{
	return a.x == b.x && a.y == b.y;
}


inline bool operator!=( const uint4 &a, const uint4 &b )
{
	return !( a == b );
}


inline bool operator!=( const uint3 &a, const uint3 &b )
{
	return !( a == b );
}


inline bool operator!=( const uint2 &a, const uint2 &b )
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
