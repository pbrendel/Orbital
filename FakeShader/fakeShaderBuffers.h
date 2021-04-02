// pbrendel (c) 2019-21

#pragma once


class ByteAddressBuffer
{
public:

	ByteAddressBuffer()
		: m_buffer( nullptr )
	{}

	ByteAddressBuffer( byte *buffer )
		: m_buffer( buffer )
	{}

	void operator=( byte* buffer )
	{
		m_buffer = buffer;
	}

	void ResetRaw( void *buffer )
	{
		m_buffer = reinterpret_cast<byte *>( buffer );
	}

	constexpr const void *GetData() const { return m_buffer; }

	uint Load( uint index ) const
	{
		return *reinterpret_cast<const uint *>( &m_buffer[index] );
	}

	uint2 Load2( uint index ) const
	{
		return *reinterpret_cast<const uint2 *>( &m_buffer[index] );
	}

	const uint3 &Load3( uint index ) const
	{
		return *reinterpret_cast<const uint3 *>( &m_buffer[index] );
	}

	const uint4 &Load4( uint index ) const
	{
		return *reinterpret_cast<const uint4 *>( &m_buffer[index] );
	}

protected:

	byte *m_buffer;
};


class RWByteAddressBuffer : public ByteAddressBuffer
{
public:

	RWByteAddressBuffer()
		: ByteAddressBuffer()
	{}

	RWByteAddressBuffer( byte *buffer )
		: ByteAddressBuffer( buffer )
	{}

	void Store( uint index, uint value )
	{
		*reinterpret_cast<uint *>( &m_buffer[index] ) = value;
	}

	void Store2( uint index, uint2 value )
	{
		*reinterpret_cast<uint2 *>( &m_buffer[index] ) = value;
	}

	void Store3( uint index, const uint3 &value )
	{
		*reinterpret_cast<uint3 *>( &m_buffer[index] ) = value;
	}

	void Store4( uint index, const uint4 &value )
	{
		*reinterpret_cast<uint4 *>( &m_buffer[index] ) = value;
	}

	void InterlockedAdd( uint index, uint value, uint &prevValue )
	{
		prevValue = *reinterpret_cast<uint *>( &m_buffer[index] );
		*reinterpret_cast<uint *>( &m_buffer[index] ) += value;
	}

	using ByteAddressBuffer::operator=;
};


template< typename T >
class StructuredBuffer
{
public:

	StructuredBuffer()
		: m_buffer( nullptr )
	{}

	StructuredBuffer( T *buffer )
		: m_buffer( buffer )
	{}

	void operator=( T *buffer )
	{
		m_buffer = buffer;
	}

	void ResetRaw( void *buffer )
	{
		m_buffer = reinterpret_cast<T *>( buffer );
	}

	constexpr const void *GetData() const { return m_buffer; }

	const T& operator[]( uint index ) const
	{
		return m_buffer[index];
	}

protected:

	T* m_buffer;
};


template< typename T >
class RWStructuredBuffer : public StructuredBuffer<T>
{
public:

	RWStructuredBuffer()
		: StructuredBuffer<T>()
	{}

	RWStructuredBuffer( T *buffer )
		: StructuredBuffer<T>( buffer )
	{}

	T& operator[]( uint index )
	{
		return this->m_buffer[index];
	}

	using StructuredBuffer<T>::operator=;
};
