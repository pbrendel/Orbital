// pbrendel (c) 2019-21

#pragma once

#include "orbital.h"

#include "Core/vector.h"
#include "Core/matrix.h"

#include "fakeShaderBuffers.h"
#include "fakeShaderTextures.h"
#include "fakeShaderSyntax.h"

#include <functional>

class Image;


#define FAKE_SHADER 1

//////////////////////////////////////////////////////////////////////////

typedef uint FsId;


struct FsCbDesc
{
	void *m_data;
	uint m_size;
};


struct FsSrvDesc
{
	typedef Texture2D<uint>			Texture2DCommon;
	typedef Texture2DArray<uint>	Texture2DArrayCommon;
	typedef StructuredBuffer<uint>	StructuredBufferCommon;

	enum : uint
	{
		SRV_TYPE_UNKNOWN,
		SRV_TYPE_TEXTURE_2D,
		SRV_TYPE_TEXTURE_2D_ARRAY,
		SRV_TYPE_RAW_BUFFER,
		SRV_TYPE_STRUCTURED_BUFFER,
	};

	FsSrvDesc()
		: m_data( nullptr )
		, m_type( SRV_TYPE_UNKNOWN )
		, m_pixelFormat( PIXEL_FORMAT_UNKNOWN )
	{}

	template< typename T >
	FsSrvDesc( Texture2D<T> &tex )
		: m_texture( As<Texture2DCommon>( &tex ) )
		, m_type( SRV_TYPE_TEXTURE_2D )
		, m_pixelFormat( TypeToPixelFormat<T>::Value )
	{}

	template< typename T >
	FsSrvDesc( Texture2DArray<T> &texArr )
		: m_textureArray( As<Texture2DArrayCommon>( &texArr ) )
		, m_type( SRV_TYPE_TEXTURE_2D_ARRAY )
		, m_pixelFormat( TypeToPixelFormat<T>::Value )
	{}

	FsSrvDesc( ByteAddressBuffer &buffer )
		: m_rawBuffer( &buffer )
		, m_type( SRV_TYPE_RAW_BUFFER )
		, m_pixelFormat( PIXEL_FORMAT_UNKNOWN )
	{}

	template< typename T >
	FsSrvDesc( StructuredBuffer<T> &buffer )
		: m_structuredBuffer( As<StructuredBufferCommon>( &buffer ) )
		, m_type( SRV_TYPE_STRUCTURED_BUFFER )
		, m_pixelFormat( PIXEL_FORMAT_UNKNOWN )
	{}

	union
	{
		void *m_data;
		Texture2DCommon *m_texture;
		Texture2DArrayCommon *m_textureArray;
		ByteAddressBuffer *m_rawBuffer;
		StructuredBufferCommon *m_structuredBuffer;
	};
	uint m_type;
	uint m_pixelFormat;
};


struct FsUavDesc
{
	enum : uint
	{
		UAV_TYPE_UNKNOWN,
		UAV_TYPE_TEXTURE_2D,
		UAV_TYPE_TEXTURE_2D_ARRAY,
		UAV_TYPE_RAW_BUFFER,
		UAV_TYPE_STRUCTURED_BUFFER,
	};

	typedef Texture2D<uint>			Texture2DCommon;
	typedef Texture2DArray<uint>	Texture2DArrayCommon;
	typedef StructuredBuffer<uint>	StructuredBufferCommon;

	FsUavDesc()
		: m_data( nullptr )
		, m_type( UAV_TYPE_UNKNOWN )
		, m_pixelFormat( PIXEL_FORMAT_UNKNOWN )
	{}

	template< typename T >
	FsUavDesc( RWTexture2D<T> &tex )
		: m_texture( As<Texture2DCommon>( &tex ) )
		, m_type( UAV_TYPE_TEXTURE_2D )
		, m_pixelFormat( TypeToPixelFormat<T>::Value )
	{}

	template< typename T >
	FsUavDesc( RWTexture2DArray<T> &texArr )
		: m_textureArray( As<Texture2DArrayCommon>( &texArr ) )
		, m_type( UAV_TYPE_TEXTURE_2D_ARRAY )
		, m_pixelFormat( TypeToPixelFormat<T>::Value )
	{}

	FsUavDesc( RWByteAddressBuffer &buffer )
		: m_rawBuffer( &buffer )
		, m_type( UAV_TYPE_RAW_BUFFER )
		, m_pixelFormat( PIXEL_FORMAT_UNKNOWN )
	{}

	template< typename T >
	FsUavDesc( RWStructuredBuffer<T> &buffer )
		: m_structuredBuffer( As<StructuredBufferCommon>( &buffer ) )
		, m_type( UAV_TYPE_STRUCTURED_BUFFER )
		, m_pixelFormat( PIXEL_FORMAT_UNKNOWN )
	{}

	union
	{
		void *m_data;
		Texture2DCommon *m_texture;
		Texture2DArrayCommon *m_textureArray;
		ByteAddressBuffer *m_rawBuffer;
		StructuredBufferCommon *m_structuredBuffer;
	};
	uint m_type;
	uint m_pixelFormat;
};


struct FsResources
{
	FsResources()
		: m_cbDesc( nullptr )
		, m_cbCount( 0 )
		, m_srvDesc( nullptr )
		, m_srvCount( 0 )
		, m_uavDesc( nullptr )
		, m_uavCount( 0 )
	{
	}

	template< uint CB_COUNT, uint SRV_COUNT, uint UAV_COUNT >
	FsResources( FsCbDesc( &cbDesc )[CB_COUNT], FsSrvDesc ( &srvDesc )[SRV_COUNT], FsUavDesc( &uavDesc )[UAV_COUNT] )
	{
		static_assert( CB_COUNT > 0 );
		m_cbCount = cbDesc[0].m_data != nullptr ? CB_COUNT : 0;
		m_cbDesc = cbDesc[0].m_data != nullptr ? cbDesc : nullptr;

		static_assert( SRV_COUNT > 0 );
		m_srvCount = srvDesc[0].m_data != nullptr ? SRV_COUNT : 0;
		m_srvDesc = srvDesc[0].m_data != nullptr ? srvDesc : nullptr;

		static_assert( UAV_COUNT > 0 );
		m_uavCount = uavDesc[0].m_data != nullptr ? UAV_COUNT : 0;
		m_uavDesc = uavDesc[0].m_data != nullptr ? uavDesc : nullptr;
	}

	FsCbDesc *m_cbDesc;
	uint m_cbCount;

	FsSrvDesc *m_srvDesc;
	uint m_srvCount;

	FsUavDesc *m_uavDesc;
	uint m_uavCount;
};


#define FAKE_SHADER_CB_BEGIN( index )	struct FsCb##index {
#define FAKE_SHADER_CB_END( index )		} static s_cb##index;

#define FAKE_SHADER_RESOURCES_BEGIN		struct FsResDesc {
#define FAKE_SHADER_RESOURCES_END }		static s_fsResDesc;

#define FAKE_SHADER_CB_0	\
	FsCbDesc m_cbDesc[1] = { { nullptr, 0 } };
#define FAKE_SHADER_CB_1( index0 )	\
	FsCbDesc m_cbDesc[1] = { { &s_cb##index0, sizeof( s_cb##index0 ) } };
#define FAKE_SHADER_CB_2( index0, index1 )	\
	FsCbDesc m_cbDesc[2] = { { &s_cb##index0, sizeof( s_cb##index0 ) }, { &s_cb##index1, sizeof( s_cb##index1 ) } };

#define FAKE_SHADER_SRV_0	\
	FsSrvDesc m_srvDesc[1] = { FsSrvDesc() };
#define FAKE_SHADER_SRV_1( srv0 )	\
	FsSrvDesc m_srvDesc[1] = { FsSrvDesc( srv0 ) };
#define FAKE_SHADER_SRV_2( srv0, srv1 )	\
	FsSrvDesc m_srvDesc[2] = { FsSrvDesc( srv0 ), FsSrvDesc( srv1 ) };
#define FAKE_SHADER_SRV_3( srv0, srv1, srv2 )	\
	FsSrvDesc m_srvDesc[3] = { FsSrvDesc( srv0 ), FsSrvDesc( srv1 ), FsSrvDesc( srv2 ) };
#define FAKE_SHADER_SRV_4( srv0, srv1, srv2, srv3 )	\
	FsSrvDesc m_srvDesc[4] = { FsSrvDesc( srv0 ), FsSrvDesc( srv1 ), FsSrvDesc( srv2 ), FsSrvDesc( srv3 ) };
#define FAKE_SHADER_SRV_5( srv0, srv1, srv2, srv3, srv4 )	\
	FsSrvDesc m_srvDesc[5] = { FsSrvDesc( srv0 ), FsSrvDesc( srv1 ), FsSrvDesc( srv2 ), FsSrvDesc( srv3 ), FsSrvDesc( srv4 ) };

#define FAKE_SHADER_UAV_0	\
	FsUavDesc m_uavDesc[1] = { FsUavDesc() };
#define FAKE_SHADER_UAV_1( uav0 )	\
	FsUavDesc m_uavDesc[1] = { FsUavDesc( uav0 ) };
#define FAKE_SHADER_UAV_2( uav0, uav1 )	\
	FsUavDesc m_uavDesc[2] = { FsUavDesc( uav0 ), FsUavDesc( uav1 ) };
#define FAKE_SHADER_UAV_3( uav0, uav1, uav2 )	\
	FsUavDesc m_uavDesc[3] = { FsUavDesc( uav0 ), FsUavDesc( uav1 ), FsUavDesc( uav2 ) };

#define FAKE_SHADER_RESOURCES( device ) \
	( device.GetType() == FsDevice::Type::Cpu ) ? FsResources( s_fsResDesc.m_cbDesc, s_fsResDesc.m_srvDesc, s_fsResDesc.m_uavDesc ) : FsResources();


struct FsGroupSize
{
	uint m_x;
	uint m_y;
	uint m_z;
};


class FsEntryPoint
{
public:

	typedef std::function< void( const uint3 & ) > FuncThread;
	typedef std::function< void( const uint3 &, const uint3 & ) > FuncGroupThread;

	FsEntryPoint( const FsGroupSize &groupSize, FuncThread func );
	FsEntryPoint( const FsGroupSize &groupSize, FuncGroupThread func );

	constexpr const FsGroupSize &GetGroupSize() const { return m_groupSize; }

	void operator()( const uint3 &groupId, const uint3 &groupThreadId ) const;

private:

	FsGroupSize m_groupSize;
	FuncThread m_funcThread;
	FuncGroupThread m_funcGroupThread;
};


#define FAKE_SHADER_MAIN( entryPoint, x, y, z ) FsGroupSize s_fsGroup_##entryPoint { x, y, z }; void entryPoint( const uint3 &threadID )
#define FAKE_SHADER_MAIN_GROUP( entryPoint, x, y, z ) FsGroupSize s_fsGroup_##entryPoint { x, y, z }; void entryPoint( const uint3 &groupID, const uint3 &groupThreadID )
#define CREATE_FAKE_SHADER( device, shaderFilename, entryPoint ) device.CreateShader( FsEntryPoint( s_fsGroup_##entryPoint, entryPoint ), shaderFilename, #entryPoint )

#define DECLARE_FAKE_SHADER_WRAPPER( name )						\
	namespace name												\
	{															\
		FsResources GetShaderResources( FsDevice &fsDevice );	\
		FsId CreateShader( FsDevice &fsDevice );				\
	}

//////////////////////////////////////////////////////////////////////////

struct FsDevice
{
public:

	enum class Type : uint
	{
		Cpu,
		Gpu,
	};

	FsDevice();
	FsDevice( struct ID3D11Device *d3dDevice );
	FsDevice( FsDevice &&other );
	~FsDevice();

	FsDevice &operator=( FsDevice &&other );

	constexpr Type GetType() const { return m_type; }

	void Reset();

	FsId CreateConstantBuffer( const FsResources &res, uint size, void *data );
	void UpdateConstantBuffer( FsId cbId, void *data ) const;

	FsId CreateTextureSRV( const FsResources &res, const Image &img );
	FsId CreateTextureSRV( const FsResources &res, uint width, uint height, uint depth, uint pixelFormat, const void *data );
	FsId CreateRawBufferSRV( const FsResources &res, uint bufferSize, const void *data );
	FsId CreateStructuredBufferSRV( const FsResources &res, uint elemSize, uint count, const void *data );
	void ApplySRV() const;

	FsId CreateTextureUAV( const FsResources &res, Image &img );
	FsId CreateTextureUAV( const FsResources &res, uint width, uint height, uint depth, uint pixelFormat, void *data );
	FsId CreateRawBufferUAV( const FsResources &res, uint bufferSize, void *data );
	FsId CreateStructuredBufferUAV( const FsResources &res, uint elemSize, uint count, void *data );
	void ApplyUAV() const;

	FsId CreateShader( FsEntryPoint entryPoint, const char *shaderFilename, const char *entryPointName );
	void Dispatch( FsId shaderId, uint sizeX, uint sizeY, uint sizeZ ) const;

	void ReadBack( FsId bufferId, uint dataSize, void *data ) const;

	void ProfileBegin();
	double ProfileEnd();

private:

	template< typename T >
	T *GetImpl()
	{
		T *impl = As<T>( m_impl );
		assert( impl->IsA( m_type ) );
		return impl;
	}

	template< typename T >
	const T *GetImpl() const
	{
		const T *impl = As<T>( m_impl );
		assert( impl->IsA( m_type ) );
		return impl;
	}

	Type m_type;
	void *m_impl;
};
