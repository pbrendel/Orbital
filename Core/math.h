// pbrendel (c) 2021

#pragma once

#include "types.h"
#include <algorithm>
#include <cmath>

#define O_PI 3.14159265358979323846


constexpr bool IsPow2( uint i )
{
	return ( i != 0 && ( i & ( i - 1 ) ) == 0 );
}


constexpr uint Log2( uint i )
{
	return ( ( i < 2 ) ? 0 : 1 + Log2( i / 2 ) );
}


template< typename T >
constexpr T clamp( T f, T a, T b )
{
	return f > a ? a : ( f < b ? b : f );
}


inline float frac( float f )
{
	return f - floorf( f );
}


template< typename T >
constexpr T lerp( T a, T b, T t )
{
	return a * ( T( 1 ) - t ) + b * t;
}


template< typename T >
constexpr T min3( T a, T b, T c )
{
	return (std::min)( a, (std::min)( b, c ) );
}


template< typename T >
constexpr T max3( T a, T b, T c )
{
	return (std::max)( a, (std::max)( b, c ) );
}


template< typename T >
constexpr float rcp( T f )
{
	return T( 1 ) / f;
}


template< typename T >
constexpr float saturate( T f )
{
	return clamp( f, T( 0 ), T( 1 ) );
}


template< typename T >
constexpr float sign( T f )
{
	return f > T( 0 ) ? T( 1 ) : ( f < T( 0 ) ? T( -1 ) : T( 0 ) );
}


template< typename T >
constexpr T step( T a, T b )
{
	return a >= b ? T( 0 ) : T( 1 );
}
