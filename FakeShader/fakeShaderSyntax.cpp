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


void sincos( float rad, float &outSin, float &outCos )
{
	outSin = sin( rad );
	outCos = cos( rad );
}


bool any( bool2 b )
{
	return b.x || b.y;
}


bool any( bool3 b )
{
	return b.x || b.y || b.z;
}


bool any( bool4 b )
{
	return b.x || b.y || b.z || b.w;
}


bool all( bool2 b )
{
	return b.x && b.y;
}


bool all( bool3 b )
{
	return b.x && b.y && b.z;
}


bool all( bool4 b )
{
	return b.x && b.y && b.z && b.w;
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