// pbrendel (c) 2019-21

#pragma once

#include "types.h"


template< typename T, uint N >
constexpr uint ArrayCount( const T (&arr)[N] )
{
	return N;
}


constexpr uint GroupsCount( uint threadsCount, uint groupSize )
{
	return ( threadsCount + groupSize - 1 ) / groupSize;
}


constexpr uint AlignUp( uint size, uint align )
{
	const uint mask = align - 1;
	return ( size + mask ) & ( ~mask );
}


constexpr bool IsPow2( uint i )
{
	return ( i != 0 && ( i & ( i - 1 ) ) == 0 );
}
