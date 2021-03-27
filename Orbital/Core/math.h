// pbrendel (c) 2021

#pragma once

#include "types.h"


#define O_PI 3.14159265358979323846


constexpr bool IsPow2( uint i )
{
	return ( i != 0 && ( i & ( i - 1 ) ) == 0 );
}


constexpr uint Log2( uint i )
{
	return ( ( i < 2 ) ? 0 : 1 + Log2( i / 2 ) );
}


constexpr float Saturate( float f )
{
	return ( f < 0.0f ) ? 0.0f : ( f > 1.0f ? 1.0f : f );
}
