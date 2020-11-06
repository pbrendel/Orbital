// pbrendel (c) 2019-21

#include "fakeShaderSyntax.h"
#include "assert.h"


const float &asfloat( const uint &i )
{
	return reinterpret_cast<const float &>( i );
}


const float2 &asfloat( const uint2 &i )
{
	return reinterpret_cast<const float2 &>( i );
}


const float3 &asfloat( const uint3 &i )
{
	return reinterpret_cast<const float3 &>( i );
}


const float4 &asfloat( const uint4 &i )
{
	return reinterpret_cast<const float4 &>( i );
}


const int &asint( const float &f )
{
	return reinterpret_cast<const int &>( f );
}


const uint &asuint( const float &f )
{
	return reinterpret_cast<const uint &>( f );
}


const uint2 &asuint( const float2 &f )
{
	return reinterpret_cast<const uint2 &>( f );
}


const uint3 &asuint( const float3 &f )
{
	return reinterpret_cast<const uint3 &>( f );
}


const uint4 &asuint( const float4 &f )
{
	return reinterpret_cast<const uint4 &>( f );
}


uint countbits( uint i )
{
	uint bits = 0;
	while ( i > 0 )
	{
		if ( i & 1 )
		{
			bits++;
		}
		i >>= 1;
	}
	return bits;
}


uint firstbithigh( uint i )
{
	assert( i > 0 );
	uint mask = 1 << 31;
	uint bits = 31;
	while ( mask > 0 )
	{
		if ( i & mask )
		{
			break;
		}
		bits--;
		mask >>= 1;
	}
	return bits;
}


uint firstbitlow( uint i )
{
	assert( i > 0 );
	uint bits = 0;
	while ( i > 0 )
	{
		if ( i & 1 )
		{
			break;
		}
		bits++;
		i >>= 1;
	}
	return bits;
}


uint reversebits( uint i )
{
	uint o = 0;
	uint flagI = 1;
	uint flagO = 1 << 31;
	for ( uint index = 0; index < 32; ++index )
	{
		if ( i & flagI )
		{
			o |= flagO;
		}
		flagI <<= 1;
		flagO >>= 1;
	}
	return o;
}


float2 round( const float2 &v )
{
	return float2( roundf( v.x ), roundf( v.y ) );
}


float3 round( const float3 &v )
{
	return float3( roundf( v.x ), roundf( v.y ), roundf( v.z ) );
}


float4 round( const float4 &v )
{
	return float4( roundf( v.x ), roundf( v.y ), roundf( v.z ), roundf( v.w ) );
}


float2 frac( const float2 &v )
{
	return float2( v.x - floorf( v.x ), v.y - floorf( v.y ) );
}


float3 frac( const float3 &v )
{
	return float3( v.x - floorf( v.x ), v.y - floorf( v.y ), v.z - floorf( v.z ) );
}


float4 frac( const float4 &v )
{
	return float4( v.x - floorf( v.x ), v.y - floorf( v.y ), v.z - floorf( v.z ), v.w - floorf( v.w ) );
}



float clamp( float f, float l, float h )
{
	return min( max( f, l ), h );
}


float lerp( float a, float b, float t )
{
	return a * ( 1.0f - t ) + b * t;
}


float2 lerp( const float2 &a, const float2 &b, float t )
{
	return a * ( 1.0f - t ) + b * t;
}


float3 lerp( const float3 &a, const float3 &b, float t )
{
	return a * ( 1.0f - t ) + b * t;
}


float4 lerp( const float4 &a, const float4 &b, float t )
{
	return a * ( 1.0f - t ) + b * t;
}


float sign( float f )
{
	return f > 0.0f ? 1.0f : ( f < 0.0f ? -1.0f : 0.0f );
}


float rcp( float f )
{
	return 1.0f / f;
}


float step( float a, float b )
{
	return a >= b ? 0.0f : 1.0f;
}


float2 step( const float2 &a, const float2 &b )
{
	return float2( a.x >= b.x ? 0.0f : 1.0f, a.y >= b.y ? 0.0f : 1.0f );
}


float3 step( const float3 &a, const float3 &b )
{
	return float3( a.x >= b.x ? 0.0f : 1.0f, a.y >= b.y ? 0.0f : 1.0f, a.z >= b.z ? 0.0f : 1.0f );
}


float4 step( const float4 &a, const float4 &b )
{
	return float4( a.x >= b.x ? 0.0f : 1.0f, a.y >= b.y ? 0.0f : 1.0f, a.z >= b.z ? 0.0f : 1.0f, a.w >= b.w ? 0.0f : 1.0f );
}


float saturate( float f )
{
	return f < 0.0f ? 0.0f : ( f > 1.0f ? 1.0f : f );
}


float dot( const float2 &a, const float2 &b )
{
	return a.x * b.x + a.y * b.y;
}


float dot( const float3 &a, const float3 &b )
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}


float dot( const float4 &a, const float4 &b )
{
	return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}


float3 cross( const float3 &a, const float3 &b )
{
	float3 c;
	c.x = a.y * b.z - a.z * b.y;
	c.y = a.z * b.x - a.x * b.z;
	c.z = a.x * b.y - a.y * b.x;
	return c;
}


float length( const float2 &v )
{
	return static_cast<float>( sqrt( dot( v, v ) ) );
}


float length( const float3 &v )
{
	return static_cast<float>( sqrt( dot( v, v ) ) );
}


float length( const float4 &v )
{
	return static_cast<float>( sqrt( dot( v, v ) ) );
}


float2 normalize( const float2 & v )
{
	const float len = length( v );
	return ( len > 0.00001f ) ? ( v / len ) : v;
}


float3 normalize( const float3 & v )
{
	const float len = length( v );
	return ( len > 0.00001f ) ? ( v / len ) : v;
}


float4 normalize( const float4 & v )
{
	const float len = length( v );
	return ( len > 0.00001f ) ? ( v / len ) : v;
}


float2 min( const float2 &a, const float2 &b )
{
	return float2( min( a.x, b.x ), min( a.y, b.y ) );
}


float3 min( const float3 &a, const float3 &b )
{
	return float3( min( a.x, b.x ), min( a.y, b.y ), min( a.z, b.z ) );
}


float4 min( const float4 &a, const float4 &b )
{
	return float4( min( a.x, b.x ), min( a.y, b.y ), min( a.z, b.z ), min( a.w, b.w ) );
}


float2 max( const float2 &a, const float2 &b )
{
	return float2( max( a.x, b.x ), max( a.y, b.y ) );
}


float3 max( const float3 &a, const float3 &b )
{
	return float3( max( a.x, b.x ), max( a.y, b.y ), max( a.z, b.z ) );
}


float4 max( const float4 &a, const float4 &b )
{
	return float4( max( a.x, b.x ), max( a.y, b.y ), max( a.z, b.z ), max( a.w, b.w ) );
}


// We're not using multithreading for fake shaders.
void InterlockedAdd( uint &dest, uint value )
{
	dest += value;
}


void InterlockedAnd( uint &dest, uint value )
{
	dest &= value;
}


void InterlockedOr( uint &dest, uint value )
{
	dest |= value;
}