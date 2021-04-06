// pbrendel (c) 2019-21

#pragma once


class ByteAddressBuffer
{
public:

	ByteAddressBuffer()
		: m_buffer( nullptr )
	{}

	ByteAddressBuffer( const byte *buffer )
		: m_buffer( RemoveConst( buffer ) )
	{}

	void operator=( const byte* buffer )
	{
		m_buffer = RemoveConst( buffer );
	}

	void ResetRaw( const void *buffer )
	{
		m_buffer = As<byte>( RemoveConst( buffer ) );
	}

	uint Load( uint index ) const
	{
		return *As<uint>( &m_buffer[index] );
	}

	uint2 Load2( uint index ) const
	{
		return *As<uint2>( &m_buffer[index] );
	}

	const uint3 &Load3( uint index ) const
	{
		return *As<uint3>( &m_buffer[index] );
	}

	const uint4 &Load4( uint index ) const
	{
		return *As<uint4>( &m_buffer[index] );
	}

	constexpr const void *GetData() const { return m_buffer; }

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
		*As<uint>( &m_buffer[index] ) = value;
	}

	void Store2( uint index, uint2 value )
	{
		*As<uint2>( &m_buffer[index] ) = value;
	}

	void Store3( uint index, const uint3 &value )
	{
		*As<uint3>( &m_buffer[index] ) = value;
	}

	void Store4( uint index, const uint4 &value )
	{
		*As<uint4>( &m_buffer[index] ) = value;
	}

	void InterlockedAdd( uint index, uint value, uint &prevValue )
	{
		prevValue = *As<uint>( &m_buffer[index] );
		*As<uint>( &m_buffer[index] ) += value;
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

	StructuredBuffer( const T *buffer )
		: m_buffer( RemoveConst( buffer ) )
	{}

	void operator=( const T *buffer )
	{
		m_buffer = RemoveConst( buffer );
	}

	void ResetRaw( const void *buffer )
	{
		m_buffer = As<T>( RemoveConst( buffer ) );
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
