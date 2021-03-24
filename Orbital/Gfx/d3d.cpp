// pbrendel (c) 2019-21

#include "d3d.h"
#include "shaderCache.h"
#include "Core/utils.h"
#include "Core/assert.h"


#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dcompiler.lib")


#define SUPPORTED_TEXTURE_FORMAT	DXGI_FORMAT_R32_FLOAT;


ID3D11Device *D3D_CreateDevice()
{
	constexpr D3D_FEATURE_LEVEL lvl[] = { D3D_FEATURE_LEVEL_11_1, D3D_FEATURE_LEVEL_11_0 };
#if D3D_DEBUG
	constexpr UINT createDeviceFlags = D3D11_CREATE_DEVICE_DEBUG;
#else // #if D3D_DEBUG
	constexpr UINT createDeviceFlags = 0;
#endif  // #else // #if D3D_DEBUG
	
	ID3D11Device* device = nullptr;
	const HRESULT hr = D3D11CreateDevice( nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, lvl, ArrayCount( lvl ), D3D11_SDK_VERSION, &device, nullptr, nullptr );
	
	return SUCCEEDED( hr ) ? device : nullptr;
}


ID3D11DeviceContext *D3D_GetImmediateContext( ID3D11Device *device )
{
	ID3D11DeviceContext *context = nullptr;
	device->GetImmediateContext( &context );
	return context;
}


ID3D11ComputeShader *D3D_CreateComputeShader( ID3D11Device *device, const char *filename, const char *entryPoint )
{
	ID3DBlob *shaderBlob = ShaderCache_Get( filename, entryPoint );
	if ( shaderBlob == nullptr )
	{
		return nullptr;
	}

	ID3D11ComputeShader* shader = nullptr;
	HRESULT hr = device->CreateComputeShader( shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), nullptr, &shader );

	return SUCCEEDED( hr ) ? shader : nullptr;
}


ID3D11Buffer *D3D_CreateConstantBuffer( ID3D11Device *device, uint bufferSize, void *data )
{
	D3D11_BUFFER_DESC desc = { 0 };
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.ByteWidth = bufferSize;
	desc.Usage = D3D11_USAGE_DEFAULT; // PB: D3D11_USAGE_DYNAMIC?
	desc.CPUAccessFlags = 0; // PB: D3D11_CPU_ACCESS_WRITE?

	ID3D11Buffer *buffer = nullptr;
	HRESULT hr;
	if ( data != nullptr )
	{
		D3D11_SUBRESOURCE_DATA bufferData = { 0 };
		bufferData.pSysMem = data;
		hr = device->CreateBuffer( &desc, &bufferData, &buffer );
	}
	else
	{
		hr = device->CreateBuffer( &desc, nullptr, &buffer );
	}

	return SUCCEEDED( hr ) ? buffer : nullptr;
}


ID3D11Buffer *D3D_CreateStructuredBuffer( ID3D11Device *device, uint elementSize, uint count, void *data )
{
	D3D11_BUFFER_DESC desc = { 0 };
	desc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
	desc.ByteWidth = elementSize * count;
	desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	desc.StructureByteStride = elementSize;

	ID3D11Buffer *buffer = nullptr;
	HRESULT hr;
	if ( data != nullptr )
	{
		D3D11_SUBRESOURCE_DATA bufferData = { 0 };
		bufferData.pSysMem = data;
		hr = device->CreateBuffer( &desc, &bufferData, &buffer );
	}
	else
	{
		hr = device->CreateBuffer( &desc, nullptr, &buffer );
	}

	return SUCCEEDED( hr ) ? buffer : nullptr;
}


ID3D11Buffer *D3D_CreateRawBuffer( ID3D11Device *device, uint bufferSize, void *data )
{
	D3D11_BUFFER_DESC desc = { 0 };
	desc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_INDEX_BUFFER | D3D11_BIND_VERTEX_BUFFER;
	desc.ByteWidth = bufferSize;
	desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS;

	ID3D11Buffer *buffer = nullptr;
	HRESULT hr;
	if ( data != nullptr )
	{
		D3D11_SUBRESOURCE_DATA bufferData = { 0 };
		bufferData.pSysMem = data;
		hr = device->CreateBuffer( &desc, &bufferData, &buffer );
	}
	else
	{
		hr = device->CreateBuffer( &desc, nullptr, &buffer );
	}

	return SUCCEEDED( hr ) ? buffer : nullptr;
}


ID3D11Buffer *D3D_CreateCpuAccessBuffer( ID3D11Device* device, ID3D11DeviceContext* context, ID3D11Buffer* sourceBuffer, bool copyData )
{
	D3D11_BUFFER_DESC desc = { 0 };
	sourceBuffer->GetDesc( &desc );
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	desc.Usage = D3D11_USAGE_STAGING;
	desc.BindFlags = 0;
	desc.MiscFlags = 0;

	ID3D11Buffer* cpuBuffer = nullptr;
	const HRESULT hr = device->CreateBuffer( &desc, nullptr, &cpuBuffer );
	if ( SUCCEEDED( hr ) )
	{
		if ( copyData )
		{
			context->CopyResource( cpuBuffer, sourceBuffer );
		}
		return cpuBuffer;
	}
	return nullptr;
}


ID3D11Texture2D* D3D_CreateTexture( ID3D11Device* device, const void *data, uint width, uint height, uint bytesPerPixel )
{
	// Only 32 bit RGBA textures are supported for now.
	assert( bytesPerPixel == 4 );
	
	D3D11_TEXTURE2D_DESC desc = { 0 };
	desc.Width = width;
	desc.Height = height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = SUPPORTED_TEXTURE_FORMAT;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;

	ID3D11Texture2D *texture = nullptr;
	HRESULT hr;
	if ( data != nullptr )
	{
		D3D11_SUBRESOURCE_DATA textureData = { 0 };
		textureData.pSysMem = data;
		textureData.SysMemPitch = width * bytesPerPixel;
		hr = device->CreateTexture2D( &desc, &textureData, &texture );
	}
	else
	{
		hr = device->CreateTexture2D( &desc, nullptr, &texture );
	}
	return SUCCEEDED( hr ) ? texture : nullptr;
}


ID3D11ShaderResourceView *D3D_CreateBufferSRV( ID3D11Device *device, ID3D11Buffer *buffer )
{
	D3D11_BUFFER_DESC descBuf = { 0 };
	buffer->GetDesc( &descBuf );

	D3D11_SHADER_RESOURCE_VIEW_DESC desc = { 0 };
	desc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
	desc.BufferEx.FirstElement = 0;

	if ( descBuf.MiscFlags & D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS )
	{
		desc.Format = DXGI_FORMAT_R32_TYPELESS;
		desc.BufferEx.Flags = D3D11_BUFFEREX_SRV_FLAG_RAW;
		desc.BufferEx.NumElements = descBuf.ByteWidth / 4;
	}
	else if ( descBuf.MiscFlags & D3D11_RESOURCE_MISC_BUFFER_STRUCTURED )
	{
		desc.Format = DXGI_FORMAT_UNKNOWN;
		desc.BufferEx.NumElements = descBuf.ByteWidth / descBuf.StructureByteStride;
	}
	else
	{
		return nullptr;
	}

	ID3D11ShaderResourceView *srv = nullptr;
	const HRESULT hr = device->CreateShaderResourceView( buffer, &desc, &srv );
	return SUCCEEDED( hr ) ? srv : nullptr;
}


ID3D11UnorderedAccessView *D3D_CreateBufferUAV( ID3D11Device* device, ID3D11Buffer* buffer )
{
	D3D11_BUFFER_DESC descBuf = { 0 };
	buffer->GetDesc( &descBuf );

	D3D11_UNORDERED_ACCESS_VIEW_DESC desc = { 0 };
	desc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	desc.Buffer.FirstElement = 0;

	if ( descBuf.MiscFlags & D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS )
	{
		desc.Format = DXGI_FORMAT_R32_TYPELESS; // Format must be DXGI_FORMAT_R32_TYPELESS, when creating Raw Unordered Access View
		desc.Buffer.Flags = D3D11_BUFFER_UAV_FLAG_RAW;
		desc.Buffer.NumElements = descBuf.ByteWidth / 4; 
	}
	else if ( descBuf.MiscFlags & D3D11_RESOURCE_MISC_BUFFER_STRUCTURED )
	{
		desc.Format = DXGI_FORMAT_UNKNOWN;      // Format must be must be DXGI_FORMAT_UNKNOWN, when creating a View of a Structured Buffer
		desc.Buffer.NumElements = descBuf.ByteWidth / descBuf.StructureByteStride; 
	}
	else
	{
		return nullptr;
	}

	ID3D11UnorderedAccessView *uav = nullptr;
	const HRESULT hr = device->CreateUnorderedAccessView( buffer, &desc, &uav );
	return SUCCEEDED( hr ) ? uav : nullptr;
}


ID3D11ShaderResourceView *D3D_CreateTextureSRV( ID3D11Device *device, ID3D11Texture2D *texture )
{
	D3D11_SHADER_RESOURCE_VIEW_DESC desc = { 0 };
	desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	desc.Format = SUPPORTED_TEXTURE_FORMAT;
	desc.Texture2D.MostDetailedMip = 0;
	desc.Texture2D.MipLevels = 1;

	ID3D11ShaderResourceView *srv = nullptr;
	const HRESULT hr = device->CreateShaderResourceView( texture, &desc, &srv );
	return SUCCEEDED( hr ) ? srv : nullptr;
}


void D3D_UpdateConstantBuffer( ID3D11DeviceContext *context, ID3D11Buffer *buffer, void *data )
{
	context->UpdateSubresource( buffer, 0, nullptr, data, 0, 0 );
}


ID3D11SamplerState *D3D_CreatePointClampSampler( ID3D11Device* device )
{
	D3D11_SAMPLER_DESC desc = { 0 };
	desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;

	ID3D11SamplerState *sampler = nullptr;
	const HRESULT hr = device->CreateSamplerState( &desc, &sampler );
	return SUCCEEDED( hr ) ? sampler : nullptr;
}


void D3D_ReadBack( ID3D11Device *d3dDevice, ID3D11Buffer *gpuBuffer, uint dataSize, void *data )
{
	ID3D11DeviceContext *d3dContext = D3D_GetImmediateContext( d3dDevice );
	ID3D11Buffer *cpuAccessBuffer = D3D_CreateCpuAccessBuffer( d3dDevice, d3dContext, gpuBuffer, true );

	D3D11_MAPPED_SUBRESOURCE MappedResource;
	d3dContext->Map( cpuAccessBuffer, 0, D3D11_MAP_READ, 0, &MappedResource );
	memcpy( data, MappedResource.pData, dataSize );
	d3dContext->Unmap( cpuAccessBuffer, 0 );

	cpuAccessBuffer->Release();
	d3dContext->Release();
}
