// pbrendel (c) 2019-21

#include "image.h"
#include "pixelFormat.h"

#include "Core/file.h"

#include <cctype>

#define STB_IMAGE_IMPLEMENTATION
#include "stb-master/stb_image.h"


enum class ImageFileFormat
{
	Unknown,
	PNG,
	RAW,
};


static ImageFileFormat Image_GetFileFormat( const char *filename )
{
	const size_t len = strlen( filename );
	if ( len < 4 )
	{
		return ImageFileFormat::Unknown;
	}
	if ( tolower( filename[ len - 3 ] ) == 'p' && tolower( filename[ len - 2 ] ) == 'n' && tolower( filename[ len - 1 ] ) == 'g' )
	{
		return ImageFileFormat::PNG;
	}
	if ( tolower( filename[ len - 3 ] ) == 'r' && tolower( filename[ len - 2 ] ) == 'a' && tolower( filename[ len - 1 ] ) == 'w' )
	{
		return ImageFileFormat::RAW;
	}
	return ImageFileFormat::Unknown;
}

//////////////////////////////////////////////////////////////////////////

Image::Image()
	: m_width( 0 )
	, m_height( 0 )
	, m_pixelFormat( PIXEL_FORMAT_UNKNOWN )
{
}


Image::Image( Image &&other )
	: m_width( other.m_width )
	, m_height( other.m_height )
	, m_pixelFormat( other.m_pixelFormat )
	, m_data( std::move( other.m_data ) )
{
#if DEBUG_IMAGE
	memcpy( m_name, other.m_name, MAX_IMAGE_NAME_SIZE );
#endif // #if DEBUG_IMAGE
}


Image &Image::operator=( Image &&other )
{
	m_width = other.m_width;
	m_height = other.m_height;
	m_pixelFormat = other.m_pixelFormat;
	m_data = std::move( other.m_data );
#if DEBUG_IMAGE
	memcpy( m_name, other.m_name, MAX_IMAGE_NAME_SIZE );
#endif // #if DEBUG_IMAGE
	return *this;
}

//////////////////////////////////////////////////////////////////////////

Image Image::Create( uint width, uint height, uint pixelFormat )
{
	Image newImage;
	newImage.m_width = width;
	newImage.m_height = height;
	newImage.m_pixelFormat = pixelFormat;
	newImage.m_data.Reset( width * height * PixelFormat_GetBytesPerPixel( pixelFormat ) );
	return newImage;
}


Image Image::Load( const char *filename )
{
	Image loadedImage;

	struct stbi_deleter
	{
		void operator()( void *data ) const { stbi_image_free( data ); }
	};

#if DEBUG_IMAGE
	const uint nameLength = std::min<uint>( static_cast<uint>( strlen( filename ) ), MAX_IMAGE_NAME_SIZE );
	memcpy( loadedImage.m_name, filename, nameLength );
#endif // #if DEBUG_IMAGE

	const ImageFileFormat fileFormat = Image_GetFileFormat( filename );
	if ( fileFormat == ImageFileFormat::Unknown )
	{
		return loadedImage;
	}

	FileData fileData = File_ReadBinary( filename );
	if ( fileData.IsEmpty() )
	{
		return loadedImage;
	}

	const uint fileSize = fileData.GetSize();
	if ( fileFormat == ImageFileFormat::RAW )
	{
		// Assuming RAW file format to be 1 channel float buffer of square size.
		const uint imageWidth = static_cast<uint>( roundf( sqrtf( static_cast<float>( fileSize / sizeof( float ) ) ) ) );
		loadedImage.m_width = imageWidth;
		loadedImage.m_height = imageWidth;
		loadedImage.m_pixelFormat = PIXEL_FORMAT_R_FLOAT;
		loadedImage.m_data.Reset( fileSize );
		memcpy( loadedImage.m_data.Get(), fileData.Get(), fileSize );
	}
	else
	{
		// Only PNG supported for now.
		assert( fileFormat == ImageFileFormat::PNG );
		int imageWidth = 0;
		int imageHeight = 0;
		int channelsCount = 0;
		constexpr int desiredChannels = 4;
		const std::unique_ptr<stbi_uc, stbi_deleter> imageData( stbi_load_from_memory( fileData.Get(), fileSize, &imageWidth, &imageHeight, &channelsCount, desiredChannels ) );
		if ( !imageData )
		{
			return loadedImage;
		}

		loadedImage.m_width = static_cast<uint>( imageWidth );
		loadedImage.m_height = static_cast<uint>( imageWidth );
		loadedImage.m_pixelFormat = PIXEL_FORMAT_RGBA_BYTE;	// stbi loads image with 8 bits for each component
		const uint imageDataSize = imageWidth * imageHeight * desiredChannels;
		loadedImage.m_data.Reset( imageDataSize );
		memcpy( loadedImage.m_data.Get(), imageData.get(), imageDataSize );
	}

	return loadedImage;
}


Image Image::Copy( const Image &srcImage )
{
	Image dstImage;
	dstImage.m_width = srcImage.m_width;
	dstImage.m_height = srcImage.m_height;
	dstImage.m_pixelFormat = srcImage.m_pixelFormat;

#if DEBUG_IMAGE
	memcpy( dstImage.m_name, srcImage.m_name, MAX_IMAGE_NAME_SIZE );
#endif // #if DEBUG_IMAGE

	const uint imageDataSize = srcImage.m_width * srcImage.m_height * PixelFormat_GetBytesPerPixel( srcImage.m_pixelFormat );
	dstImage.m_data.Reset( imageDataSize );
	memcpy( dstImage.m_data.Get(), srcImage.m_data.Get(), imageDataSize );

	return dstImage;
}


Image Image::Convert( const Image &srcImage, uint dstPixelFormat )
{
	const uint pixelCount = srcImage.m_width * srcImage.m_height;
	const uint srcBytesPerPixel = PixelFormat_GetBytesPerPixel( srcImage.m_pixelFormat );
	const uint dstBytesPerPixel = PixelFormat_GetBytesPerPixel( dstPixelFormat );

	Image dstImage;
	dstImage.m_width = srcImage.m_width;
	dstImage.m_height = srcImage.m_height;
	dstImage.m_pixelFormat = dstPixelFormat;
	dstImage.m_data.Reset( pixelCount * dstBytesPerPixel );
#if DEBUG_IMAGE
	memcpy( dstImage.m_name, srcImage.m_name, MAX_IMAGE_NAME_SIZE );
#endif // #if DEBUG_IMAGE

	const byte *srcData = srcImage.m_data.Get();
	byte *dstData = dstImage.m_data.Get();

	PixelFormatConverter converter = PixelFormat_GetConverter( srcImage.m_pixelFormat, dstImage.m_pixelFormat );
	uint srcOffset = 0;
	uint dstOffset = 0;
	for ( uint i = 0; i < pixelCount; ++i )
	{
		converter( srcData + srcOffset, dstData + dstOffset );
		srcOffset += srcBytesPerPixel;
		dstOffset += dstBytesPerPixel;
	}

	return dstImage;
}
