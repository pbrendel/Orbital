// pbrendel (c) 2019-21

#pragma once

#include "fakeShaderSyntax.h"
#include "fakeShaderBuffers.h"
#include "fakeShaderTextures.h"

#include <functional>


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
	enum : uint
	{
		SRV_TYPE_TEXTURE_2D,
		SRV_TYPE_RAW_BUFFER,
		SRV_TYPE_STRUCTURED_BUFFER,
		SRV_TYPE_UNKNOWN,
	};

	FsSrvDesc()
		: m_data( nullptr )
		, m_type( SRV_TYPE_UNKNOWN )
	{
	}

	FsSrvDesc( Texture2D<float> &tex )
		: m_tex( &tex )
		, m_type( SRV_TYPE_TEXTURE_2D )
	{
	}

	FsSrvDesc( ByteAddressBuffer &buffer )
		: m_rawBuffer( &buffer )
		, m_type( SRV_TYPE_RAW_BUFFER )
	{
	}

	template< typename T >
	FsSrvDesc( StructuredBuffer<T> &buffer )
		: m_structuredBuffer( reinterpret_cast<StructuredBuffer<uint> >( &buffer ) )
		, m_type( SRV_TYPE_STRUCTURED_BUFFER )
	{
	}

	union
	{
		void *m_data;
		Texture2D<float> *m_tex;
		ByteAddressBuffer *m_rawBuffer;
		StructuredBuffer<uint> *m_structuredBuffer;
	};
	uint m_type;
};


struct FsUavDesc
{
	enum : uint
	{
		UAV_TYPE_RAW_BUFFER,
		UAV_TYPE_STRUCTURED_BUFFER,
		UAV_TYPE_UNKNOWN,
	};

	typedef RWStructuredBuffer<uint> StructuredBufferCommon;

	FsUavDesc()
		: m_data( nullptr )
		, m_type( UAV_TYPE_UNKNOWN )
	{
	}

	FsUavDesc( RWByteAddressBuffer &buffer )
		: m_rawBuffer( &buffer )
		, m_type( UAV_TYPE_RAW_BUFFER )
	{
	}

	template< typename T >
	FsUavDesc( RWStructuredBuffer<T> &buffer )
		: m_structuredBuffer( reinterpret_cast<StructuredBufferCommon *>( &buffer ) )
		, m_type( UAV_TYPE_STRUCTURED_BUFFER )
	{
	}

	union
	{
		void *m_data;
		ByteAddressBuffer *m_rawBuffer;
		StructuredBufferCommon *m_structuredBuffer;
	};
	uint m_type;
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

#define FAKE_SHADER_CB_0					FsCbDesc m_cbDesc[1] = { { nullptr, 0 } };
#define FAKE_SHADER_CB_1( index0 )			FsCbDesc m_cbDesc[1] = { { &s_cb##index0, sizeof( s_cb##index0 ) } };
#define FAKE_SHADER_CB_2( index0, index1 )	FsCbDesc m_cbDesc[2] = { { &s_cb##index0, sizeof( s_cb##index0 ) }, { &s_cb##index1, sizeof( s_cb##index1 ) } };

#define FAKE_SHADER_SRV_0					FsSrvDesc m_srvDesc[1] = { FsSrvDesc() };
#define FAKE_SHADER_SRV_1( srv0 )			FsSrvDesc m_srvDesc[1] = { FsSrvDesc( srv0 ) };
#define FAKE_SHADER_SRV_2( srv0, srv1 )		FsSrvDesc m_srvDesc[2] = { FsSrvDesc( srv0 ), FsSrvDesc( srv1 ) };

#define FAKE_SHADER_UAV_0					FsUavDesc m_uavDesc[1] = { FsUavDesc() };
#define FAKE_SHADER_UAV_1( uav0 )			FsUavDesc m_uavDesc[1] = { FsUavDesc( uav0 ) };
#define FAKE_SHADER_UAV_2( uav0, uav1 )		FsUavDesc m_uavDesc[2] = { FsUavDesc( uav0 ), FsUavDesc( uav1 ) };

#define FAKE_SHADER_RESOURCES( device ) ( device.GetType() == FsDevice::Type::Cpu ) ? FsResources( s_fsResDesc.m_cbDesc, s_fsResDesc.m_srvDesc, s_fsResDesc.m_uavDesc ) : FsResources();

typedef std::function< void( uint3 ) > FsEntryPoint;

struct FsGroupSize
{
	uint m_x;
	uint m_y;
	uint m_z;
};

#define FAKE_SHADER_MAIN( entryPoint, x, y, z ) FsGroupSize s_fsGroup_##entryPoint { x, y, z }; void entryPoint( const uint3 &threadID )
#define CREATE_FAKE_SHADER( device, shaderFilename, entryPoint ) device.CreateShader( FsEntryPoint( ::entryPoint ), s_fsGroup_##entryPoint, shaderFilename, #entryPoint )

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

	FsId CreateConstantBuffer( const FsResources &res, uint size, void *data );
	void UpdateConstantBuffer( FsId cbId, void *data ) const;

	FsId CreateTextureSRV( const FsResources &res, const Texture2D<float> &tex );
	void ApplySRV() const;

	FsId CreateStructuredBufferUAV( const FsResources &res, uint elemSize, uint count, void *data );
	void ApplyUAV() const;

	FsId CreateShader( FsEntryPoint entryPointFunc, const FsGroupSize &groupSize, const char *shaderFilename, const char *entryPointName );
	void Dispatch( FsId shaderId, uint sizeX, uint sizeY, uint sizeZ ) const;

	void ReadBack( FsId bufferId, uint dataSize, void *data ) const;

	void ProfileBegin();
	double ProfileEnd();

private:

	template< typename T >
	T *GetImpl()
	{
		T *impl = reinterpret_cast<T *>( m_impl );
		assert( impl->IsA( m_type ) );
		return impl;
	}

	template< typename T >
	const T *GetImpl() const
	{
		const T *impl = reinterpret_cast<const T *>( m_impl );
		assert( impl->IsA( m_type ) );
		return impl;
	}

	Type m_type;
	void *m_impl;
};
