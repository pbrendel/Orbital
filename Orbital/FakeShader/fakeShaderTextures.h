// pbrendel (c) 2019-21

#pragma once

#include "Gfx/pixelFormat.h"


constexpr uint pointClampSampler = 0;


template< typename T >
class Texture2D
{
public:

	Texture2D()
		: m_data( nullptr )
		, m_width( 0 )
		, m_height( 0 )
		, m_bytesPerPixel( 0 )
		, m_converter( nullptr )
	{}
		
	void ResetRaw( const byte *data, uint width, uint height, uint dataPixelFormat, uint texturePixelFormat )
	{
		m_data = const_cast<byte *>( data );
		m_width = width;
		m_height = height;
		m_bytesPerPixel = PixelFormat_GetBytesPerPixel( dataPixelFormat );
		m_converter = PixelFormat_GetConverter( dataPixelFormat, texturePixelFormat );
	}

	void Reset( const byte *data, uint width, uint height, uint dataPixelFormat )
	{
		ResetRaw( data, width, height, dataPixelFormat, TypeToPixelFormat<T>::Value );
	}

	T SampleLevel( uint samplerId, float2 uv, uint level ) const
	{
		assert( samplerId == pointClampSampler );
		const uint x = static_cast<uint>( uv.x * m_width );
		const uint y = static_cast< uint >( uv.y * m_height );
		const uint offset = ( x + y * m_width ) * m_bytesPerPixel;
		float val = 0.0f;
		m_converter( &m_data[offset], &val );
		return val;
	}

	T operator[]( uint2 index ) const
	{
		const uint offset = ( index.x + index.y * m_width ) * m_bytesPerPixel;
		float val = 0.0f;
		m_converter( &m_data[offset], &val );
		return val;
	}

	constexpr const byte *GetData() const { return m_data; }
	constexpr uint GetWidth() const { return m_width; }
	constexpr uint GetHeight() const { return m_height; }

protected:

	byte *m_data;
	uint m_width;
	uint m_height;
	uint m_bytesPerPixel;
	PixelFormatConverter m_converter;
};

//////////////////////////////////////////////////////////////////////////

template< typename T >
class RWTexture2D : public Texture2D<T>
{
public:

	void ResetRaw( const byte *data, uint width, uint height, uint dataPixelFormat, uint texturePixelFormat )
	{
		assertex( dataPixelFormat == texturePixelFormat, "RWTexture2D internal data format must match texture type." );
		Texture2D<T>::ResetRaw( data, width, height, dataPixelFormat, texturePixelFormat );
	}

	void ResetRaw( const byte *data, uint width, uint height, uint dataPixelFormat )
	{
		assertex( dataPixelFormat == TypeToPixelFormat<T>::Value, "RWTexture2D internal data format must match texture type." );
		Texture2D<T>::Reset( data, width, height, dataPixelFormat );
	}

	T &operator[]( uint2 index )
	{
		const uint offset = ( index.x + index.y *this->m_width ) * this->m_bytesPerPixel;
		return *As<T>( &this->m_data[offset] );
	}
};

//////////////////////////////////////////////////////////////////////////

template< typename T >
class Texture2DArray
{
public:

	Texture2DArray()
		: m_data( nullptr )
		, m_width( 0 )
		, m_height( 0 )
		, m_depth( 0 )
		, m_bytesPerPixel( 0 )
		, m_converter( nullptr )
	{}

	void ResetRaw( const byte *data, uint width, uint height, uint depth, uint dataPixelFormat, uint texturePixelFormat )
	{
		m_data = const_cast<byte *>( data );
		m_width = width;
		m_height = height;
		m_depth = depth;
		m_bytesPerPixel = PixelFormat_GetBytesPerPixel( dataPixelFormat );
		m_converter = PixelFormat_GetConverter( dataPixelFormat, texturePixelFormat );
	}

	void Reset( const byte *data, uint width, uint height, uint depth, uint dataPixelFormat )
	{
		ResetRaw( data, width, height, depth, dataPixelFormat, TypeToPixelFormat<T>::Value );
	}

	T operator[]( uint3 index ) const
	{
		const uint offset = ( index.x + index.y + ( index.z * m_height ) * m_width ) * m_bytesPerPixel;
		T val;
		m_converter( &m_data[offset], &val );
		return val;
	}

	constexpr const byte *GetData() const { return m_data; }
	constexpr uint GetWidth() const { return m_width; }
	constexpr uint GetHeight() const { return m_height; }
	constexpr uint GetDepth() const { return m_depth; }

protected:

	byte *m_data;
	uint m_width;
	uint m_height;
	uint m_depth;
	uint m_bytesPerPixel;
	PixelFormatConverter m_converter;
};

//////////////////////////////////////////////////////////////////////////

template< typename T >
class RWTexture2DArray : public Texture2DArray<T>
{
public:

	void ResetRaw( const byte *data, uint width, uint height, uint depth, uint dataPixelFormat, uint texturePixelFormat )
	{
		assertex( dataPixelFormat == texturePixelFormat, "RWTexture2DArray internal data format must match texture type." );
		Texture2DArray<T>::ResetRaw( data, width, height, depth, dataPixelFormat, texturePixelFormat );
	}

	void Reset( const byte *data, uint width, uint height, uint depth, uint dataPixelFormat )
	{
		assertex( dataPixelFormat == TypeToPixelFormat<T>::Value, "RWTexture2DArray internal data format must match texture type." );
		Texture2DArray<T>::Reset( data, width, height, depth, dataPixelFormat );
	}

	T &operator[]( uint3 index )
	{
		const uint offset = ( index.x + index.y + ( index.z * this->m_height ) * this->m_width ) * this->m_bytesPerPixel;
		return *As<T>( &this->m_data[offset] );
	}
};
