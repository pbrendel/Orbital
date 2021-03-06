// pbrendel (c) 2020-21

#pragma once

#include <memory>


template< typename T>
inline static T Mem_Read( const void *data )
{
	return *reinterpret_cast<const T *>( data );
}


template< typename T>
inline static T Mem_Read( const void *data, uint offset )
{
	return *reinterpret_cast<const T *>( reinterpret_cast<const byte *>( data ) + offset );
}


template< typename T>
inline static void Mem_Write( void *data, const T& val )
{
	*reinterpret_cast<T *>( data ) = val;
}


template< typename T>
inline static T Mem_Write( void *data, uint offset, const T &val )
{
	*reinterpret_cast<T *>( reinterpret_cast<byte *>( data ) + offset ) = val;
}
