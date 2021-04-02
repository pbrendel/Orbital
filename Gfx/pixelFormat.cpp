// pbrendel (c) 2021

#include "pixelFormat.h"
#include "Core/math.h"


uint PixelFormat_GetChannelsCount( uint format )
{
	switch ( format )
	{
	case PIXEL_FORMAT_RGBA_FLOAT:
	case PIXEL_FORMAT_RGBA_UNORM:
	case PIXEL_FORMAT_RGBA_BYTE:
		return 4;
	case PIXEL_FORMAT_RGB_FLOAT:
	case PIXEL_FORMAT_RGB_UNORM:
	case PIXEL_FORMAT_RGB_BYTE:
		return 3;
	case PIXEL_FORMAT_RG_FLOAT:
	case PIXEL_FORMAT_RG_UNORM:
	case PIXEL_FORMAT_RG_BYTE:
		return 2;
	case PIXEL_FORMAT_R_FLOAT:
	case PIXEL_FORMAT_R_UNORM:
	case PIXEL_FORMAT_R_BYTE:
		return 1;
	default:
		assertex( false, "Unknown image format." );
		return 0;
	}
}


uint PixelFormat_GetBytesPerPixel( uint format )
{
	switch ( format )
	{
	case PIXEL_FORMAT_RGBA_FLOAT:
	case PIXEL_FORMAT_RGBA_UNORM:
		return 16;
	case PIXEL_FORMAT_RGB_FLOAT:
	case PIXEL_FORMAT_RGB_UNORM:
		return 12;
	case PIXEL_FORMAT_RG_FLOAT:
	case PIXEL_FORMAT_RG_UNORM:
		return 8;
	case PIXEL_FORMAT_RGBA_BYTE:
	case PIXEL_FORMAT_R_FLOAT:
	case PIXEL_FORMAT_R_UNORM:
		return 4;
	case PIXEL_FORMAT_RGB_BYTE:
		return 3;
	case PIXEL_FORMAT_RG_BYTE:
		return 2;
	case PIXEL_FORMAT_R_BYTE:
		return 1;
	default:
		assertex( false, "Unknown image format." );
		return 0;
	}
}


bool PixelFormat_IsFloat( uint format )
{
	switch ( format )
	{
	case PIXEL_FORMAT_RGBA_FLOAT:
	case PIXEL_FORMAT_RGB_FLOAT:
	case PIXEL_FORMAT_RG_FLOAT:
	case PIXEL_FORMAT_R_FLOAT:
		return true;
	case PIXEL_FORMAT_RGBA_UNORM:
	case PIXEL_FORMAT_RGB_UNORM:
	case PIXEL_FORMAT_RG_UNORM:
	case PIXEL_FORMAT_R_UNORM:
	case PIXEL_FORMAT_RGBA_BYTE:
	case PIXEL_FORMAT_RGB_BYTE:
	case PIXEL_FORMAT_RG_BYTE:
	case PIXEL_FORMAT_R_BYTE:
		return false;
	default:
		assertex( false, "Unknown image format." );
		return false;
	}
}


bool PixelFormat_IsUnorm( uint format )
{
	switch ( format )
	{
	case PIXEL_FORMAT_RGBA_UNORM:
	case PIXEL_FORMAT_RGB_UNORM:
	case PIXEL_FORMAT_RG_UNORM:
	case PIXEL_FORMAT_R_UNORM:
		return true;
	case PIXEL_FORMAT_RGBA_FLOAT:
	case PIXEL_FORMAT_RGB_FLOAT:
	case PIXEL_FORMAT_RG_FLOAT:
	case PIXEL_FORMAT_R_FLOAT:
	case PIXEL_FORMAT_RGBA_BYTE:
	case PIXEL_FORMAT_RGB_BYTE:
	case PIXEL_FORMAT_RG_BYTE:
	case PIXEL_FORMAT_R_BYTE:
		return false;
	default:
		assertex( false, "Unknown image format." );
		return false;
	}
}


bool PixelFormat_IsByte( uint format )
{
	switch ( format )
	{
	case PIXEL_FORMAT_RGBA_BYTE:
	case PIXEL_FORMAT_RGB_BYTE:
	case PIXEL_FORMAT_RG_BYTE:
	case PIXEL_FORMAT_R_BYTE:
		return true;
	case PIXEL_FORMAT_RGBA_FLOAT:
	case PIXEL_FORMAT_RGB_FLOAT:
	case PIXEL_FORMAT_RG_FLOAT:
	case PIXEL_FORMAT_R_FLOAT:
	case PIXEL_FORMAT_RGBA_UNORM:
	case PIXEL_FORMAT_RGB_UNORM:
	case PIXEL_FORMAT_RG_UNORM:
	case PIXEL_FORMAT_R_UNORM:
		return false;
	default:
		assertex( false, "Unknown image format." );
		return false;
	}
}

//////////////////////////////////////////////////////////////////////////

namespace pixel_format_converter
{

template< typename IntegerType >
struct FloatToInteger
{
	typedef float SrcType;
	typedef IntegerType DstType;

	static void Convert( const void *src, void *dst )
	{
		constexpr float norm = float( std::numeric_limits<IntegerType>::max() );
		*As<IntegerType>( dst ) = static_cast<IntegerType>( saturate( *As<float>( src ) ) * norm );
	}
};


template< typename IntegerType >
struct IntegerToFloat
{
	typedef IntegerType SrcType;
	typedef float DstType;

	static void Convert( const void *src, void *dst )
	{
		constexpr float norm = 1.0f / float( std::numeric_limits<IntegerType>::max() );
		*As<float>( dst ) = static_cast<float>( *As<IntegerType>( src ) ) * norm;
	}
};


template< typename SrcIntegerType, typename DstIntegerType >
struct IntegerToInteger
{
	typedef SrcIntegerType SrcType;
	typedef DstIntegerType DstType;

	static void Convert( const void *src, void *dst )
	{
		constexpr float norm = float( std::numeric_limits<DstIntegerType>::max() ) / float( std::numeric_limits<SrcIntegerType>::max() );
		*As<DstIntegerType>( dst ) = static_cast<DstIntegerType>( static_cast<float>( *As<SrcIntegerType>( src ) ) * norm );
	}
};


template< typename T >
struct Passthrough
{
	typedef T SrcType;
	typedef T DstType;

	static void Convert( const void *src, void *dst )
	{
		*As<T>( dst ) = *As<T>( src );
	}
};


template< uint ChannelsCount, typename InternalConverter >
struct Channels
{
	typedef typename InternalConverter::SrcType SrcType;
	typedef typename InternalConverter::DstType DstType;

	static void Convert( const void *src, void *dst )
	{
		for ( uint i = 0; i < ChannelsCount; ++i )
		{
			InternalConverter::Convert( src, dst );
			src = PtrOffset( src, sizeof( SrcType ) );
			src = PtrOffset( dst, sizeof( DstType ) );
		}
	}
};

} // namespace pixel_format_converter

PixelFormatConverter PixelFormat_GetConverter( uint srcFormat, uint dstFormat )
{
	using namespace pixel_format_converter;

	const uint dstChannelsCount = PixelFormat_GetChannelsCount( dstFormat );
	const uint srcChannelsCount = PixelFormat_GetChannelsCount( srcFormat );
	assertex( dstChannelsCount <= srcChannelsCount, "Cannot convert to a greater number of channels." );

	if ( PixelFormat_IsFloat( srcFormat ) )
	{
		if ( PixelFormat_IsFloat( dstFormat ) )
		{
			typedef Passthrough<float> InternalConverter;
			switch ( dstChannelsCount )
			{
			case 4:
				return Channels<4, InternalConverter>::Convert;
			case 3:
				return Channels<3, InternalConverter>::Convert;
			case 2:
				return Channels<2, InternalConverter>::Convert;
			case 1:
				return InternalConverter::Convert;
			}
		}
		else if ( PixelFormat_IsUnorm( dstFormat ) )
		{
			typedef FloatToInteger<uint> InternalConverter;
			switch ( dstChannelsCount )
			{
			case 4:
				return Channels<4, InternalConverter>::Convert;
			case 3:
				return Channels<3, InternalConverter>::Convert;
			case 2:
				return Channels<2, InternalConverter>::Convert;
			case 1:
				return InternalConverter::Convert;
			}
		}
		else if ( PixelFormat_IsByte( dstFormat ) )
		{
			typedef FloatToInteger<byte> InternalConverter;
			switch ( dstChannelsCount )
			{
			case 4:
				return Channels<4, InternalConverter>::Convert;
			case 3:
				return Channels<3, InternalConverter>::Convert;
			case 2:
				return Channels<2, InternalConverter>::Convert;
			case 1:
				return InternalConverter::Convert;
			}
		}
	}
	else if ( PixelFormat_IsUnorm( srcFormat ) )
	{
		if ( PixelFormat_IsFloat( dstFormat ) )
		{
			typedef IntegerToFloat<uint> InternalConverter;
			switch ( dstChannelsCount )
			{
			case 4:
				return Channels<4, InternalConverter>::Convert;
			case 3:
				return Channels<3, InternalConverter>::Convert;
			case 2:
				return Channels<2, InternalConverter>::Convert;
			case 1:
				return InternalConverter::Convert;
			}
		}
		else if ( PixelFormat_IsUnorm( dstFormat ) )
		{
			typedef Passthrough<uint> InternalConverter;
			switch ( dstChannelsCount )
			{
			case 4:
				return Channels<4, InternalConverter>::Convert;
			case 3:
				return Channels<3, InternalConverter>::Convert;
			case 2:
				return Channels<2, InternalConverter>::Convert;
			case 1:
				return InternalConverter::Convert;
			}
		}
		else if ( PixelFormat_IsByte( dstFormat ) )
		{
			typedef IntegerToInteger<uint, byte> InternalConverter;
			switch ( dstChannelsCount )
			{
			case 4:
				return Channels<4, InternalConverter>::Convert;
			case 3:
				return Channels<3, InternalConverter>::Convert;
			case 2:
				return Channels<2, InternalConverter>::Convert;
			case 1:
				return InternalConverter::Convert;
			}
		}
	}
	else if ( PixelFormat_IsByte( srcFormat ) )
	{
		if ( PixelFormat_IsFloat( dstFormat ) )
		{
			typedef IntegerToFloat<byte> InternalConverter;
			switch ( dstChannelsCount )
			{
			case 4:
				return Channels<4, InternalConverter>::Convert;
			case 3:
				return Channels<3, InternalConverter>::Convert;
			case 2:
				return Channels<2, InternalConverter>::Convert;
			case 1:
				return InternalConverter::Convert;
			}
		}
		else if ( PixelFormat_IsUnorm( dstFormat ) )
		{
			typedef IntegerToInteger<byte, uint> InternalConverter;
			switch ( dstChannelsCount )
			{
			case 4:
				return Channels<4, InternalConverter>::Convert;
			case 3:
				return Channels<3, InternalConverter>::Convert;
			case 2:
				return Channels<2, InternalConverter>::Convert;
			case 1:
				return InternalConverter::Convert;
			}
		}
		else if ( PixelFormat_IsByte( dstFormat ) )
		{
			typedef Passthrough<byte> InternalConverter;
			switch ( dstChannelsCount )
			{
			case 4:
				return Channels<4, InternalConverter>::Convert;
			case 3:
				return Channels<3, InternalConverter>::Convert;
			case 2:
				return Channels<2, InternalConverter>::Convert;
			case 1:
				return InternalConverter::Convert;
			}
		}
	}

	assertex( false, "Conversion not supported." );
	return nullptr;
}
