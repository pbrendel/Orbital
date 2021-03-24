// pbrendel (c) 2019-21

#pragma once

#include "Core/assert.h"

//////////////////////////////////////////////////////////////////////////

namespace texture_utils
{

template< typename T >
class Converter
{
public:
	
	static T Convert( const void *, uint, bool )
	{
		static_assert( false, "Unsupported sampler format" );
	}
};


template<>
class Converter<float>
{
public:

	static float Convert( const void *ptr, uint bytesPerChannel, bool floatData )
	{
		if ( floatData )
		{
			// Only single precision floating point values are supported.
			assert( bytesPerChannel == 4 );
			return *reinterpret_cast<const float *>( ptr );
		}
		else
		{
			const uint bitsPerChannel = bytesPerChannel * 8;
			const uint valueMask = ( 1 << bitsPerChannel ) - 1;
			const float normalization = 1.0f / static_cast<float>( valueMask );
			const uint val = *reinterpret_cast<const uint *>( ptr ) & valueMask;
			return static_cast<float>( val ) * normalization;
		}
	}
};

} // namespace sampler_utils

//////////////////////////////////////////////////////////////////////////
// Fake samplers

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
		, m_channelsCount( 0 )
		, m_floatData( false )
	{}
		
	Texture2D( const byte *data, uint width, uint height, uint bytesPerPixel, uint channelsCount, bool floatData )
		: m_data( data )
		, m_width( width )
		, m_height( height )
		, m_bytesPerPixel( bytesPerPixel )
		, m_channelsCount( channelsCount )
		, m_floatData( floatData )
	{}

	T SampleLevel( uint samplerId, float2 uv, uint level ) const
	{
		assert( samplerId == pointClampSampler );
		const uint x = static_cast<uint>( uv.x * m_width );
		const uint y = static_cast< uint >( uv.y * m_height );
		const uint offset = ( x + y * m_width ) * m_bytesPerPixel;
		return texture_utils::Converter<T>::Convert( &m_data[offset], m_bytesPerPixel / m_channelsCount, m_floatData );
	}

	constexpr const byte *GetData() const { return m_data; }
	constexpr uint GetWidth() const { return m_width; }
	constexpr uint GetHeight() const { return m_height; }
	constexpr uint GetBytesPerPixel() const { return m_bytesPerPixel; }

private:

	const byte *m_data;
	uint m_width;
	uint m_height;
	uint m_bytesPerPixel;
	uint m_channelsCount;
	bool m_floatData;
};
