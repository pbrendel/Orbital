// pbrendel (c) 2020-21

#pragma once

#include "types.h"

#include <memory>


namespace o
{

template< typename T >
struct DynBuffer
{
public:

	inline DynBuffer();
	inline DynBuffer( uint size );
	inline DynBuffer( DynBuffer &&other );

	inline DynBuffer &operator=( DynBuffer &&other );

	inline void Reset( uint size );
	inline void Reset( T *ptr, uint size );

	constexpr bool IsEmpty() const { return m_size == 0; }
	constexpr uint GetSize() const { return m_size; }
	constexpr uint GetDataSize() const { return m_size * sizeof( T ); }

	template< typename U = T >
	U *Get() { return reinterpret_cast<U *>( m_ptr.get() ); }
	template< typename U = T >
	const U *Get() const { return reinterpret_cast<const U *>( m_ptr.get() ); }

	T &operator[]( uint index ) { return m_ptr.get()[index]; }
	const T &operator[]( uint index ) const { return m_ptr.get()[index]; }

private:

	std::unique_ptr<T[]> m_ptr;
	uint m_size;
};

//////////////////////////////////////////////////////////////////////////

template< typename T >
inline DynBuffer<T>::DynBuffer()
	: m_size( 0 )
{}


template< typename T >
inline DynBuffer<T>::DynBuffer( uint size )
	: m_ptr( new T[size] )
	, m_size( size )
{}


template< typename T >
inline DynBuffer<T>::DynBuffer( DynBuffer &&other )
	: m_ptr( std::move( other.m_ptr ) )
	, m_size( other.m_size )
{
	other.m_size = 0;
}


template< typename T >
inline DynBuffer<T> &DynBuffer<T>::operator=( DynBuffer &&other )
{
	m_ptr = std::move( other.m_ptr );
	m_size = other.m_size;
	other.m_size = 0;
	return *this;
}


template< typename T >
inline void DynBuffer<T>::Reset( uint size )
{
	m_ptr.reset( new T[size] );
	m_size = size;
}


template< typename T >
inline void DynBuffer<T>::Reset( T *ptr, uint size )
{
	m_ptr.reset( ptr );
	m_size = size;
}

} // namespace o