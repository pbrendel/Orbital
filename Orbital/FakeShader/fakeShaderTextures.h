// pbrendel (c) 2019-21

#pragma once

#include "Core/assert.h"
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
		, m_pixelFormat( PIXEL_FORMAT_UNKNOWN )
		, m_bytesPerPixel( 0 )
		, m_converter( nullptr )
	{
	}
		
	Texture2D( const byte *data, uint width, uint height, uint pixelFormat )
		: m_data( data )
		, m_width( width )
		, m_height( height )
		, m_pixelFormat( pixelFormat )
	{
		m_bytesPerPixel = PixelFormat_GetBytesPerPixel( m_pixelFormat );
		m_converter = PixelFormat_GetConverter( m_pixelFormat, PIXEL_FORMAT_R_FLOAT );
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
	constexpr uint GetPixelFormat() const { return m_pixelFormat; }

private:

	const byte *m_data;
	uint m_width;
	uint m_height;
	uint m_pixelFormat;
	uint m_bytesPerPixel;
	PixelFormatConverter m_converter;
};
