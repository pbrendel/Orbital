// pbrendel (c) 2019-21

#pragma once

#include "types.h"


template< typename T, uint N >
constexpr uint ArrayCount( const T (&arr)[N] )
{
	return N;
}


constexpr uint BlockCount( uint i, uint blockSize )
{
	return ( i + blockSize - 1 ) / blockSize;
}


constexpr uint AlignUp( uint size, uint align )
{
	const uint mask = align - 1;
	return ( size + mask ) & ( ~mask );
}


template< typename T >
constexpr T *As( void *a )
{
	return reinterpret_cast<T *>( a );
}


template< typename T >
constexpr const T *As( const void *a )
{
	return reinterpret_cast<const T *>( a );
}


template< typename T >
constexpr T *PtrOffset( T *ptr, uint offset )
{
	return reinterpret_cast<T *>( reinterpret_cast<byte *>( ptr ) + offset );
}


template< typename T >
constexpr const T *PtrOffset( const T *ptr, uint offset )
{
	return reinterpret_cast<const T *>( reinterpret_cast<const byte *>( ptr ) + offset );
}
