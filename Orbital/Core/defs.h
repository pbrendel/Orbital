// pbrendel (c) 2021

#pragma once

#include "types.h"


constexpr uint O_INVALID_INDEX = static_cast<uint>( -1 );


template< typename T >
constexpr T *As( void * a )
{
	return reinterpret_cast<T *>( a );
}


template< typename T >
constexpr const T *As( const void *a )
{
	return reinterpret_cast<const T *>( a );
}