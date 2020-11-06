// pbrendel (c) 2019-21

#include "image.h"
#include "Core/file.h"

#include <algorithm>
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


Image Image_Load( const char *filename )
{
	Image loadedImage = { 0 };

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
		loadedImage.m_channelsCount = 1;
		loadedImage.m_bytesPerPixel = 4;
		loadedImage.m_floatData = true;
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
		loadedImage.m_channelsCount = desiredChannels;
		loadedImage.m_bytesPerPixel = desiredChannels; // stbi loads image with 8 bits for each component
		loadedImage.m_floatData = false;
		const uint imageDataSize = imageWidth * imageHeight * desiredChannels;
		loadedImage.m_data.Reset( imageDataSize );
		memcpy( loadedImage.m_data.Get(), imageData.get(), imageDataSize );
	}

	return loadedImage;
}


Image Image_Copy( const Image &srcImage )
{
	Image dstImage = { 0 };
	dstImage.m_width = srcImage.m_width;
	dstImage.m_height = srcImage.m_height;
	dstImage.m_channelsCount = srcImage.m_channelsCount;
	dstImage.m_bytesPerPixel = srcImage.m_bytesPerPixel;
	dstImage.m_floatData = srcImage.m_floatData;

#if DEBUG_IMAGE
	memcpy( dstImage.m_name, srcImage.m_name, MAX_IMAGE_NAME_SIZE );
#endif // #if DEBUG_IMAGE

	const uint imageDataSize = srcImage.m_width * srcImage.m_height * srcImage.m_bytesPerPixel;
	dstImage.m_data.Reset( imageDataSize );
	memcpy( dstImage.m_data.Get(), srcImage.m_data.Get(), imageDataSize );

	return dstImage;
}