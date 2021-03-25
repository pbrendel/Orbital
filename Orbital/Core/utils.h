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


constexpr uint Log2( uint i )
{
	return ( ( i < 2 ) ? 0 : 1 + Log2( i / 2 ) );
}


constexpr uint BlockCount( uint i, uint blockSize )
{
	return ( i + blockSize - 1 ) / blockSize;
}


constexpr float Saturate( float f )
{
	return ( f < 0.0f ) ? 0.0f : ( f > 1.0f ? 1.0f : f );
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
