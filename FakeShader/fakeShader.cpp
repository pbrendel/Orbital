// pbrendel (c) 2019-21

#include "fakeShader.h"

#include "Core/dynArray.h"
#include "Gfx/d3d.h"
#include "Gfx/d3dPerfCounter.h"
#include "Gfx/image.h"

//////////////////////////////////////////////////////////////////////////

FsEntryPoint::FsEntryPoint( const FsGroupSize &groupSize, FsEntryPoint::FuncThread func )
	: m_groupSize( groupSize )
	, m_funcThread( func )
	, m_funcGroupThread( nullptr )
{
}


FsEntryPoint::FsEntryPoint( const FsGroupSize &groupSize, FsEntryPoint::FuncGroupThread func )
	: m_groupSize( groupSize )
	, m_funcThread( nullptr )
	, m_funcGroupThread( func )
{
}


void FsEntryPoint::operator()( const uint3 &groupId, const uint3 &groupThreadId ) const
{
	if ( m_funcGroupThread )
	{
		m_funcGroupThread( groupId, groupThreadId );
	}
	else
	{
		const uint3 threadId(
			groupId.x * m_groupSize.m_x + groupThreadId.x,
			groupId.y * m_groupSize.m_y + groupThreadId.y,
			groupId.z * m_groupSize.m_z + groupThreadId.z
		);

		assert( m_funcThread );
		m_funcThread( threadId );
	}
}

//////////////////////////////////////////////////////////////////////////

class FakeShaderDeviceCpu
{
public:

	FakeShaderDeviceCpu()
		: m_constantBuffer( nullptr )
		, m_constantBufferSize( 0 )
		, m_srvCount( 0 )
		, m_uavDesc( nullptr )
		, m_uavCount( 0 )
	{
	}

	bool IsA( FsDevice::Type type ) const
	{
		return type == FsDevice::Type::Cpu;
	}

	void Reset()
	{
		m_constantBuffer = nullptr;
		m_constantBufferSize = 0;

		m_srvCount = 0;

		m_uavDesc = nullptr;
		m_uavCount = 0;

		m_bufferMem.Clear();
		m_shaderEntryPoints.Clear();
	}

	FsId CreateConstantBuffer( const FsResources &res, uint size, void *data )
	{
		assert( res.m_cbCount > 0 );
		assert( res.m_cbDesc[0].m_size == size );
		m_constantBuffer = res.m_cbDesc[0].m_data;
		m_constantBufferSize = size;
		UpdateConstantBuffer( 0, data );
		return static_cast<FsId>( 0 );
	}

	void UpdateConstantBuffer( FsId cbId, void *data ) const
	{
		assert( cbId == 0 );
		memcpy( m_constantBuffer, data, m_constantBufferSize );
	}

	FsId CreateTextureSRV( const FsResources &res, uint width, uint height, uint depth, uint pixelFormat, const void *data )
	{
		assert( m_srvCount < res.m_srvCount );
		if ( depth == 1 )
		{
			assert( res.m_srvDesc[m_srvCount].m_type == FsSrvDesc::SRV_TYPE_TEXTURE_2D );
			res.m_srvDesc[m_srvCount].m_texture->ResetRaw( data, width, height, pixelFormat, res.m_srvDesc[m_srvCount].m_pixelFormat );
		}
		else
		{
			assert( res.m_srvDesc[m_srvCount].m_type == FsSrvDesc::SRV_TYPE_TEXTURE_2D_ARRAY );
			res.m_srvDesc[m_srvCount].m_textureArray->ResetRaw( data, width, height, depth, pixelFormat, res.m_srvDesc[m_srvCount].m_pixelFormat );
		}
		m_srvCount++;
		return static_cast<FsId>( m_srvCount - 1 );
	}

	FsId CreateRawBufferSRV( const FsResources &res, const void *data )
	{
		assert( m_srvCount < res.m_srvCount );
		assert( res.m_srvDesc[m_srvCount].m_type == FsSrvDesc::SRV_TYPE_RAW_BUFFER );
		res.m_srvDesc[m_srvCount].m_rawBuffer->ResetRaw( data );
		m_srvCount++;
		return static_cast<FsId>( m_srvCount - 1 );
	}

	FsId CreateStructuredBufferSRV( const FsResources &res, const void *data )
	{
		assert( m_srvCount < res.m_srvCount );
		assert( res.m_srvDesc[m_srvCount].m_type == FsSrvDesc::SRV_TYPE_STRUCTURED_BUFFER );
		res.m_srvDesc[m_srvCount].m_structuredBuffer->ResetRaw( data );
		m_srvCount++;
		return static_cast<FsId>( m_srvCount - 1 );
	}

	FsId CreateTextureUAV( const FsResources &res, uint width, uint height, uint depth, uint pixelFormat, void *data )
	{
		assert( m_uavCount < res.m_uavCount );
		assert( m_uavDesc == nullptr || m_uavDesc == res.m_uavDesc );

		m_uavDesc = res.m_uavDesc;
		if ( depth == 1 )
		{
			assert( m_uavDesc[m_uavCount].m_type == FsUavDesc::UAV_TYPE_TEXTURE_2D );
		}
		else
		{
			assert( m_uavDesc[m_uavCount].m_type == FsUavDesc::UAV_TYPE_TEXTURE_2D_ARRAY );
		}
		if ( data == nullptr )
		{
			o::DynBuffer<byte> newData( width * height * depth * PixelFormat_GetBytesPerPixel( pixelFormat ) );
			data = newData.Get();
			m_bufferMem.PushBack( std::move( newData ) );
		}
		if ( depth == 1 )
		{
			m_uavDesc[m_uavCount].m_texture->ResetRaw( data, width, height, pixelFormat, m_uavDesc[m_uavCount].m_pixelFormat );
		}
		else
		{
			m_uavDesc[m_uavCount].m_textureArray->ResetRaw( data, width, height, depth, pixelFormat, m_uavDesc[m_uavCount].m_pixelFormat );
		}
		m_uavCount++;
		return static_cast<FsId>( m_uavCount - 1 );
	}

	FsId CreateRawBufferUAV( const FsResources &res, uint bufferSize, void *data )
	{
		assert( m_uavCount < res.m_uavCount );
		assert( m_uavDesc == nullptr || m_uavDesc == res.m_uavDesc );

		m_uavDesc = res.m_uavDesc;
		assert( m_uavDesc[m_uavCount].m_type == FsUavDesc::UAV_TYPE_RAW_BUFFER );
		if ( data == nullptr )
		{
			o::DynBuffer<byte> newData( bufferSize );
			data = newData.Get();
			m_bufferMem.PushBack( std::move( newData ) );
		}
		m_uavDesc[m_uavCount].m_rawBuffer->ResetRaw( data );
		m_uavCount++;
		return static_cast<FsId>( m_uavCount - 1 );
	}

	FsId CreateStructuredBufferUAV( const FsResources &res, uint elemSize, uint count, void *data )
	{
		assert( m_uavCount < res.m_uavCount );
		assert( m_uavDesc == nullptr || m_uavDesc == res.m_uavDesc );

		m_uavDesc = res.m_uavDesc;
		assert( m_uavDesc[m_uavCount].m_type == FsUavDesc::UAV_TYPE_STRUCTURED_BUFFER );
		if ( data == nullptr )
		{
			o::DynBuffer<byte> newData( elemSize * count );
			data = newData.Get();
			m_bufferMem.PushBack( std::move( newData ) );
		}
		m_uavDesc[m_uavCount].m_structuredBuffer->ResetRaw( data );
		m_uavCount++;
		return static_cast<FsId>( m_uavCount - 1 );
	}

	FsId CreateShader( FsEntryPoint entryPoint )
	{
		m_shaderEntryPoints.PushBack( entryPoint );
		return static_cast<FsId>( m_shaderEntryPoints.GetSize() - 1 );
	}

	void Dispatch( FsId shaderId, uint sizeX, uint sizeY, uint sizeZ ) const
	{
		const FsEntryPoint &entryPoint = m_shaderEntryPoints[static_cast<uint>( shaderId )];
		const FsGroupSize &groupSize = entryPoint.GetGroupSize();
		uint3 groupId;
		for ( groupId.z = 0; groupId.z < sizeZ; ++groupId.z )
		{
			for ( groupId.y = 0; groupId.y < sizeY; ++groupId.y )
			{
				for ( groupId.x = 0; groupId.x < sizeX; ++groupId.x )
				{
					uint3 groupThreadId;
					for ( groupThreadId.z = 0; groupThreadId.z < groupSize.m_z; ++groupThreadId.z )
					{
						for ( groupThreadId.y = 0; groupThreadId.y < groupSize.m_y; ++groupThreadId.y )
						{
							for ( groupThreadId.x = 0; groupThreadId.x < groupSize.m_x; ++groupThreadId.x )
							{
								entryPoint( groupId, groupThreadId );
							}
						}
					}
				}
			}
		}
	}

	void ReadBack( FsId uavId, uint dataSize, void *data ) const
	{
		const uint bufferIndex = static_cast<uint>( uavId );
		assert( bufferIndex < m_uavCount );
		if ( m_uavDesc[bufferIndex].m_type == FsUavDesc::UAV_TYPE_TEXTURE_2D )
		{
			memcpy( data, m_uavDesc[bufferIndex].m_texture->GetData(), dataSize );
		}
		else if ( m_uavDesc[bufferIndex].m_type == FsUavDesc::UAV_TYPE_TEXTURE_2D_ARRAY )
		{
			memcpy( data, m_uavDesc[bufferIndex].m_textureArray->GetData(), dataSize );
		}
		else if ( m_uavDesc[bufferIndex].m_type == FsUavDesc::UAV_TYPE_RAW_BUFFER )
		{
			memcpy( data, m_uavDesc[bufferIndex].m_rawBuffer->GetData(), dataSize );
		}
		else if ( m_uavDesc[bufferIndex].m_type == FsUavDesc::UAV_TYPE_STRUCTURED_BUFFER )
		{
			memcpy( data, m_uavDesc[bufferIndex].m_structuredBuffer->GetData(), dataSize );
		}
		else
		{
			assert( false );
		}
	}

private:

	void *m_constantBuffer;
	uint m_constantBufferSize;

	uint m_srvCount;

	FsUavDesc *m_uavDesc;
	uint m_uavCount;

	o::DynArray<o::DynBuffer<byte> > m_bufferMem;

	o::DynArray<FsEntryPoint> m_shaderEntryPoints;
};

#define DEVICE_CPU GetImpl<FakeShaderDeviceCpu>()

//////////////////////////////////////////////////////////////////////////

class FakeShaderDeviceD3D
{
public:

	FakeShaderDeviceD3D( ID3D11Device *d3dDevice )
		: m_device( d3dDevice )
		, m_perfCounter( d3dDevice )
	{
		m_context = D3D_GetImmediateContext( d3dDevice );

		m_pointClampSampler = D3D_CreatePointClampSampler( d3dDevice );
		ID3D11SamplerState *samplers[] = { m_pointClampSampler.Get() };
		m_context->CSSetSamplers( 0, ArrayCount( samplers ), samplers );
	}

	~FakeShaderDeviceD3D()
	{
		Reset();

		ID3D11SamplerState *nullSamplers[] = { nullptr };
		m_context->CSSetSamplers( 0, ArrayCount( nullSamplers ), nullSamplers );
	}

	bool IsA( FsDevice::Type type ) const
	{
		return type == FsDevice::Type::Gpu;
	}

	void Reset()
	{
		if ( m_constantBuffer )
		{
			ID3D11Buffer *nullConstantBuffers[] = { nullptr };
			m_context->CSSetConstantBuffers( 0, ArrayCount( nullConstantBuffers ), nullConstantBuffers );
			m_constantBuffer.Reset();
		}

		if ( !m_srv.IsEmpty() )
		{
			o::DynBuffer<ID3D11ShaderResourceView *> nullSrv( m_srv.GetSize() );
			memset( nullSrv.Get(), 0, nullSrv.GetDataSize() );
			m_context->CSSetShaderResources( 0, nullSrv.GetSize(), nullSrv.Get() );

			m_srv.Clear();
			m_srvTextures.Clear();
			m_srvBuffers.Clear();
		}

		if ( !m_uav.IsEmpty() )
		{
			o::DynBuffer<ID3D11UnorderedAccessView *> nunllUav( m_srv.GetSize() );
			memset( nunllUav.Get(), 0, nunllUav.GetDataSize() );
			m_context->CSSetUnorderedAccessViews( 0, nunllUav.GetSize(), nunllUav.Get(), nullptr );

			m_uav.Clear();
			m_uavTextures.Clear();
			m_uavBuffers.Clear();
		}

		m_shaders.Clear();
	}

	FsId CreateConstantBuffer( uint size, void *data )
	{
		m_constantBuffer = D3D_CreateConstantBuffer( m_device, size, data );
		return static_cast<FsId>( 0 );
	}

	void UpdateConstantBuffer( FsId cbId, void *data ) const
	{
		assert( cbId == 0 );
		assert( m_constantBuffer );
		D3D_UpdateConstantBuffer( m_context, m_constantBuffer, data );
		ID3D11Buffer *constantBuffers[] = { m_constantBuffer.Get() };
		m_context->CSSetConstantBuffers( 0, ArrayCount( constantBuffers ), constantBuffers );
	}

	FsId CreateTextureSRV( uint width, uint height, uint depth, uint pixelFormat, const void *data )
	{
		const uint bytesPerPixel = PixelFormat_GetBytesPerPixel( pixelFormat );
		const DXGI_FORMAT d3dPixelFormat = D3D_GetPixelFormat( pixelFormat );
		D3DTexture2D d3dTex = D3D_CreateTexture( m_device, data, width, height, depth, bytesPerPixel, d3dPixelFormat );
		D3DSRV d3dSrv = D3D_CreateTextureSRV( m_device, d3dTex, d3dPixelFormat );
		m_srvTextures.PushBack( std::move( d3dTex ) );
		m_srv.PushBack( std::move( d3dSrv ) );
		return static_cast<FsId>( m_srv.GetSize() - 1 );
	}

	FsId CreateRawBufferSRV( uint bufferSize, const void *data )
	{
		D3DBuffer d3dBuffer = D3D_CreateRawBuffer( m_device, bufferSize, data );
		D3DSRV d3dSrv = D3D_CreateBufferSRV( m_device, d3dBuffer );
		m_srvBuffers.PushBack( std::move( d3dBuffer ) );
		m_srv.PushBack( std::move( d3dSrv ) );
		return static_cast<FsId>( m_srv.GetSize() - 1 );
	}

	FsId CreateStructuredBufferSRV( uint elemSize, uint count, const void *data )
	{
		D3DBuffer d3dBuffer = D3D_CreateStructuredBuffer( m_device, elemSize, count, data );
		D3DSRV d3dSrv = D3D_CreateBufferSRV( m_device, d3dBuffer );
		m_srvBuffers.PushBack( std::move( d3dBuffer ) );
		m_srv.PushBack( std::move( d3dSrv ) );
		return static_cast<FsId>( m_srv.GetSize() - 1 );
	}

	void ApplySRV() const
	{
		o::DynBuffer<ID3D11ShaderResourceView *> srv( m_srv.GetSize() );
		for ( uint i = 0; i < srv.GetSize(); ++i )
		{
			srv[i] = m_srv[i].Get();
		}
		m_context->CSSetShaderResources( 0, srv.GetSize(), srv.Get() );
	}

	FsId CreateTextureUAV( uint width, uint height, uint depth, uint pixelFormat, const void *data )
	{
		const uint bytesPerPixel = PixelFormat_GetBytesPerPixel( pixelFormat );
		const DXGI_FORMAT d3dPixelFormat = D3D_GetPixelFormat( pixelFormat );
		D3DTexture2D d3dTex = D3D_CreateTexture( m_device, data, width, height, depth, bytesPerPixel, d3dPixelFormat );
		D3DUAV d3dUav = D3D_CreateTextureUAV( m_device, d3dTex, d3dPixelFormat );
		m_uavTextures.PushBack( std::move( d3dTex ) );
		m_uav.PushBack( std::move( d3dUav ) );
		return static_cast<FsId>( m_uav.GetSize() - 1 );
	}

	FsId CreateRawBufferUAV( uint bufferSize, void *data )
	{
		D3DBuffer d3dBuffer = D3D_CreateRawBuffer( m_device, bufferSize, data );
		D3DUAV d3dUav = D3D_CreateBufferUAV( m_device, d3dBuffer );
		m_uavBuffers.PushBack( std::move( d3dBuffer ) );
		m_uav.PushBack( std::move( d3dUav ) );
		return static_cast<FsId>( m_uav.GetSize() - 1 );
	}

	FsId CreateStructuredBufferUAV( uint elemSize, uint count, void *data )
	{
		D3DBuffer d3dBuffer = D3D_CreateStructuredBuffer( m_device, elemSize, count, data );
		D3DUAV d3dUav = D3D_CreateBufferUAV( m_device, d3dBuffer );
		m_uavBuffers.PushBack( std::move( d3dBuffer ) );
		m_uav.PushBack( std::move( d3dUav ) );
		return static_cast<FsId>( m_uav.GetSize() - 1 );
	}

	void ApplyUAV() const
	{
		o::DynBuffer<ID3D11UnorderedAccessView *> uav( m_uav.GetSize() );
		for ( uint i = 0; i < uav.GetSize(); ++i )
		{
			uav[i] = m_uav[i].Get();
		}
		m_context->CSSetUnorderedAccessViews( 0, uav.GetSize(), uav.Get(), nullptr );
	}

	FsId CreateShader( const char *filename, const char *entryPoint )
	{
		D3DComputeShader shader = D3D_CreateComputeShader( m_device, filename, entryPoint );
		m_shaders.PushBack( std::move( shader ) );
		return static_cast<FsId>( m_shaders.GetSize() - 1 );
	}

	void Dispatch( FsId shaderId, uint sizeX, uint sizeY, uint sizeZ ) const
	{
		const D3DComputeShader &shader = m_shaders[static_cast<uint>( shaderId )];
		m_context->CSSetShader( shader, nullptr, 0 );
		m_context->Dispatch( sizeX, sizeY, sizeZ );
	}

	void ReadBack( FsId uavId, uint dataSize, void *data ) const
	{
		const uint bufferIndex = static_cast<uint>( uavId );
		assert( bufferIndex < m_uavBuffers.GetSize() );
		D3D_ReadBack( m_device, m_uavBuffers[bufferIndex].Get(), dataSize, data );
	}

	void ProfileBegin()
	{
		m_perfCounter.Begin( m_context );
	}

	double ProfileEnd()
	{
		m_perfCounter.End( m_context );
		return m_perfCounter.GetTime( m_context );
	}

private:

	ID3D11Device *m_device;
	D3DContext m_context;
	
	D3DSampler m_pointClampSampler;
	D3DBuffer m_constantBuffer;

	o::DynArray<D3DTexture2D> m_srvTextures;
	o::DynArray<D3DBuffer> m_srvBuffers;
	o::DynArray<D3DSRV> m_srv;
	o::DynArray<D3DTexture2D> m_uavTextures;
	o::DynArray<D3DBuffer> m_uavBuffers;
	o::DynArray<D3DUAV> m_uav;

	o::DynArray<D3DComputeShader> m_shaders;

	D3DPerfCounter m_perfCounter;
};

#define DEVICE_GPU GetImpl<FakeShaderDeviceD3D>()

//////////////////////////////////////////////////////////////////////////

FsDevice::FsDevice()
	: m_type( Type::Cpu )
	, m_impl( new FakeShaderDeviceCpu() )
{
}


FsDevice::FsDevice( ID3D11Device *d3dDevice )
	: m_type( Type::Gpu )
	, m_impl( new FakeShaderDeviceD3D( d3dDevice ) )
{
}


FsDevice::FsDevice( FsDevice &&other )
	: m_type( other.m_type )
	, m_impl( other.m_impl )
{
	other.m_impl = nullptr;
}


FsDevice::~FsDevice()
{
	if ( m_type == Type::Cpu )
	{
		delete DEVICE_CPU;
	}
	else
	{
		delete DEVICE_GPU;
	}
}

FsDevice &FsDevice::operator=( FsDevice &&other )
{
	m_type = other.m_type;
	m_impl = other.m_impl;
	other.m_impl = nullptr;
	return *this;
}


void FsDevice::Reset()
{
	if ( m_type == FsDevice::Type::Cpu )
	{
		DEVICE_CPU->Reset();
	}
	else
	{
		DEVICE_GPU->Reset();
	}
}


FsId FsDevice::CreateConstantBuffer( const FsResources &res, uint size, void *data )
{
	return ( m_type == FsDevice::Type::Cpu )
		? DEVICE_CPU->CreateConstantBuffer( res, size, data )
		: DEVICE_GPU->CreateConstantBuffer( size, data );
}


void FsDevice::UpdateConstantBuffer( FsId cbId, void *data ) const
{
	if ( m_type == FsDevice::Type::Cpu )
	{
		DEVICE_CPU->UpdateConstantBuffer( cbId, data );
	}
	else
	{
		DEVICE_GPU->UpdateConstantBuffer( cbId, data );
	}
}


FsId FsDevice::CreateTextureSRV( const FsResources &res, const Image &img )
{
	return CreateTextureSRV( res, img.GetWidth(), img.GetHeight(), 1, img.GetPixelFormat(), img.GetData() );
}


FsId FsDevice::CreateTextureSRV( const FsResources &res, uint width, uint height, uint depth, uint pixelFormat, const void *data )
{
	return ( m_type == FsDevice::Type::Cpu )
		? DEVICE_CPU->CreateTextureSRV( res, width, height, depth, pixelFormat, data )
		: DEVICE_GPU->CreateTextureSRV( width, height, depth, pixelFormat, data );
}


FsId FsDevice::CreateRawBufferSRV( const FsResources &res, uint bufferSize, const void *data )
{
	return ( m_type == FsDevice::Type::Cpu )
		? DEVICE_CPU->CreateRawBufferSRV( res, data )
		: DEVICE_GPU->CreateRawBufferSRV( bufferSize, data );
}


FsId FsDevice::CreateStructuredBufferSRV( const FsResources &res, uint elemSize, uint count, const void *data )
{
	return ( m_type == FsDevice::Type::Cpu )
		? DEVICE_CPU->CreateStructuredBufferSRV( res, data )
		: DEVICE_GPU->CreateStructuredBufferSRV( elemSize, count, data );
}


void FsDevice::ApplySRV() const
{
	if ( m_type == FsDevice::Type::Gpu )
	{
		DEVICE_GPU->ApplySRV();
	}
}


FsId FsDevice::CreateTextureUAV( const FsResources &res, Image &img )
{
	return CreateTextureUAV( res, img.GetWidth(), img.GetHeight(), 1, img.GetPixelFormat(), img.GetData() );
}


FsId FsDevice::CreateTextureUAV( const FsResources &res, uint width, uint height, uint depth, uint pixelFormat, void *data )
{
	return ( m_type == FsDevice::Type::Cpu )
		? DEVICE_CPU->CreateTextureUAV( res, width, height, depth, pixelFormat, data )
		: DEVICE_GPU->CreateTextureUAV( width, height, depth, pixelFormat, data );
}


FsId FsDevice::CreateRawBufferUAV( const FsResources &res, uint bufferSize, void *data )
{
	return ( m_type == FsDevice::Type::Cpu )
		? DEVICE_CPU->CreateRawBufferUAV( res, bufferSize, data )
		: DEVICE_GPU->CreateRawBufferUAV( bufferSize, data );
}


FsId FsDevice::CreateStructuredBufferUAV( const FsResources &res, uint elemSize, uint count, void *data )
{
	return ( m_type == FsDevice::Type::Cpu )
		? DEVICE_CPU->CreateStructuredBufferUAV( res, elemSize, count, data )
		: DEVICE_GPU->CreateStructuredBufferUAV( elemSize, count, data );
}


void FsDevice::ApplyUAV() const
{
	if ( m_type == FsDevice::Type::Gpu )
	{
		DEVICE_GPU->ApplyUAV();
	}
}


FsId FsDevice::CreateShader( FsEntryPoint entryPoint, const char *shaderFilename, const char *entryPointName )
{
	return ( m_type == FsDevice::Type::Cpu )
		? DEVICE_CPU->CreateShader( entryPoint )
		: DEVICE_GPU->CreateShader( shaderFilename, entryPointName );
}


void FsDevice::Dispatch( FsId shaderId, uint sizeX, uint sizeY, uint sizeZ ) const
{
	if ( m_type == FsDevice::Type::Cpu )
	{
		DEVICE_CPU->Dispatch( shaderId, sizeX, sizeY, sizeZ );
	}
	else
	{
		DEVICE_GPU->Dispatch( shaderId, sizeX, sizeY, sizeZ );
	}
}


void FsDevice::ReadBack( FsId bufferId, uint dataSize, void *data ) const
{
	if ( m_type == FsDevice::Type::Cpu )
	{
		DEVICE_CPU->ReadBack( bufferId, dataSize, data );
	}
	else
	{
		DEVICE_GPU->ReadBack( bufferId, dataSize, data );
	}
}


void FsDevice::ProfileBegin()
{
	if ( m_type == FsDevice::Type::Gpu )
	{
		DEVICE_GPU->ProfileBegin();
	}
}


double FsDevice::ProfileEnd()
{
	return ( m_type == FsDevice::Type::Gpu )
		? DEVICE_GPU->ProfileEnd()
		: 0.0;
}
