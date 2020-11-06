// pbrendel (c) 2019-21

#pragma once

#include "Core/types.h"
#include "Core/memUtils.h"


#define DEBUG_IMAGE				1
#define MAX_IMAGE_NAME_SIZE		256

typedef DynBuffer<byte>	ImageData;

struct Image
{
	uint m_width;
	uint m_height;
	byte m_channelsCount;
	byte m_bytesPerPixel;
	bool m_floatData;
	ImageData m_data;

#if DEBUG_IMAGE
	char m_name[MAX_IMAGE_NAME_SIZE];
#endif // #if DEBUG_IMAGE
};


Image Image_Load( const char *filename );
Image Image_Copy( const Image &srcImage );
