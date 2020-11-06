// pbrendel (c) 2020-21

#pragma once

#include <memory>


template< typename T >
struct DynBuffer
{
public:

	DynBuffer()
		: m_size( 0 )
	{}

	DynBuffer( uint size )
		: m_ptr( new T[size] )
		, m_size( size )
	{}

	DynBuffer( DynBuffer &&other )
		: m_ptr( std::move( other.m_ptr ) )
		, m_size( other.m_size )
	{
		other.m_size = 0;
	}

	DynBuffer &operator=( DynBuffer &&other )
	{
		m_ptr = std::move( other.m_ptr );
		m_size = other.m_size;
		other.m_size = 0;
		return *this;
	}

	void Reset( uint size )
	{
		m_ptr.reset( new T[size] );
		m_size = size;
	}

	void Reset( T *ptr, uint size )
	{
		m_ptr.reset( ptr );
		m_size = size;
	}

	bool IsEmpty() const { return m_size == 0; }
	uint GetSize() const { return m_size; }

	T *Get() { return m_ptr.get(); }
	const T *Get() const { return m_ptr.get(); }

	T &operator[]( uint index ) { return m_ptr.get()[index]; }
	const T &operator[]( uint index ) const { return m_ptr.get()[index]; }

private:

	std::unique_ptr<T[]> m_ptr;
	uint m_size;
};

//////////////////////////////////////////////////////////////////////////

template< typename T>
inline static T Read( const void *data )
{
	return *reinterpret_cast<const T *>( data );
}


template< typename T>
inline static T Read( const void *data, uint offset )
{
	return *reinterpret_cast<const T *>( reinterpret_cast<const byte *>( data ) + offset );
}


template< typename T>
inline static void Write( void *data, const T& val )
{
	*reinterpret_cast<T *>( data ) = val;
}


template< typename T>
inline static T Write( void *data, uint offset, const T &val )
{
	*reinterpret_cast<T *>( reinterpret_cast<byte *>( data ) + offset ) = val;
}
