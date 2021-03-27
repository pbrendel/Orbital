// pbrendel (c) 2021

#pragma once

#include "orbital.h"
#include "Core/vector.h"

#include <functional>


enum PixelFormat : uint
{
	PIXEL_FORMAT_UNKNOWN,
	PIXEL_FORMAT_RGBA_FLOAT,
	PIXEL_FORMAT_RGB_FLOAT,
	PIXEL_FORMAT_RG_FLOAT,
	PIXEL_FORMAT_R_FLOAT,
	PIXEL_FORMAT_RGBA_UNORM,
	PIXEL_FORMAT_RGB_UNORM,
	PIXEL_FORMAT_RG_UNORM,
	PIXEL_FORMAT_R_UNORM,
	PIXEL_FORMAT_RGBA_BYTE,
	PIXEL_FORMAT_RGB_BYTE,
	PIXEL_FORMAT_RG_BYTE,
	PIXEL_FORMAT_R_BYTE,
};


typedef std::function<void( const void *, void * )> PixelFormatConverter;

uint PixelFormat_GetChannelsCount( uint format );
uint PixelFormat_GetBytesPerPixel( uint format );
bool PixelFormat_IsFloat( uint format );
bool PixelFormat_IsUnorm( uint format );
bool PixelFormat_IsByte( uint format );
PixelFormatConverter PixelFormat_GetConverter( uint srcFormat, uint dstFormat );


template< typename T > struct TypeToPixelFormat { static const uint Value = PIXEL_FORMAT_UNKNOWN; };
template<> struct TypeToPixelFormat<float> { static const uint Value = PIXEL_FORMAT_R_FLOAT; };
template<> struct TypeToPixelFormat<float2> { static const uint Value = PIXEL_FORMAT_RG_FLOAT; };
template<> struct TypeToPixelFormat<float3> { static const uint Value = PIXEL_FORMAT_RGB_FLOAT; };
template<> struct TypeToPixelFormat<float4> { static const uint Value = PIXEL_FORMAT_RGBA_FLOAT; };
template<> struct TypeToPixelFormat<uint> { static const uint Value = PIXEL_FORMAT_R_UNORM; };
template<> struct TypeToPixelFormat<uint2> { static const uint Value = PIXEL_FORMAT_RG_UNORM; };
template<> struct TypeToPixelFormat<uint3> { static const uint Value = PIXEL_FORMAT_RGB_UNORM; };
template<> struct TypeToPixelFormat<uint4> { static const uint Value = PIXEL_FORMAT_RGBA_UNORM; };
