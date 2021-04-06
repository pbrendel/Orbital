// pbrendel (c) 2019-21

#pragma once

#include "orbital.h"
#include "Core/dynBuffer.h"


#define DEBUG_IMAGE				1
#define MAX_IMAGE_NAME_SIZE		256

class Image
{
public:

	typedef o::DynBuffer<byte> Data;

	Image();
	Image( Image &&other );
	Image &operator=( Image &&other );

	operator bool() const { return !m_data.IsEmpty(); }

	constexpr uint GetWidth() const { return m_width; }
	constexpr uint GetHeight() const { return m_height; }
	constexpr uint GetPixelFormat() const { return m_pixelFormat; }
	constexpr uint GetDataSize() const { return m_data.GetSize(); }
	byte *GetData() { return m_data.Get(); }
	const byte *GetData() const { return m_data.Get(); }

	static Image Create( uint width, uint height, uint pixelFormat );
	static Image Load( const char *filename );
	static Image Copy( const Image &srcImage );
	static Image Convert( const Image &srcImage, uint dstPixelFormat );

private:

	uint m_width;
	uint m_height;
	uint m_pixelFormat;
	Data m_data;

#if DEBUG_IMAGE
	char m_name[MAX_IMAGE_NAME_SIZE];
#endif // #if DEBUG_IMAGE
};
