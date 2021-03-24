// pbrendel (c) 2019-21

#pragma once

#include "d3dHelpers.h"
#include "Core/types.h"
#include "Core/dynBuffer.h"

#include <d3d11.h>


#define D3D_DEBUG 1
#define D3D_SHADER_COMPILER_OUTPUT 1
#define D3D_HELPER_TYPES 1


ID3D11Device *D3D_CreateDevice();
ID3D11DeviceContext *D3D_GetImmediateContext( ID3D11Device *device );
ID3D11ComputeShader *D3D_CreateComputeShader( ID3D11Device *device, const char *filename, const char *entryPoint );
ID3D11Buffer *D3D_CreateConstantBuffer( ID3D11Device *device, uint bufferSize, void *data );
ID3D11Buffer *D3D_CreateStructuredBuffer( ID3D11Device *device, uint elementSize, uint count, void *data );
ID3D11Buffer *D3D_CreateRawBuffer( ID3D11Device *device, uint bufferSize, void *data );
ID3D11Buffer *D3D_CreateCpuAccessBuffer( ID3D11Device* device, ID3D11DeviceContext* context, ID3D11Buffer* sourceBuffer, bool copyData );
ID3D11Texture2D* D3D_CreateTexture( ID3D11Device* device, const void *data, uint width, uint height, uint bytesPerPixel );
ID3D11ShaderResourceView *D3D_CreateBufferSRV( ID3D11Device *device, ID3D11Buffer *buffer );
ID3D11UnorderedAccessView *D3D_CreateBufferUAV( ID3D11Device* device, ID3D11Buffer* buffer );
ID3D11ShaderResourceView *D3D_CreateTextureSRV( ID3D11Device *device, ID3D11Texture2D *texture );
void D3D_UpdateConstantBuffer( ID3D11DeviceContext *context, ID3D11Buffer *buffer, void *data );
ID3D11SamplerState *D3D_CreatePointClampSampler( ID3D11Device* device );
void D3D_ReadBack( ID3D11Device *d3dDevice, ID3D11Buffer *gpuBuffer, uint dataSize, void *data );

//////////////////////////////////////////////////////////////////////////

#if D3D_HELPER_TYPES

D3D_DECLARE_HANDLE( D3DDevice, ID3D11Device );
D3D_DECLARE_HANDLE( D3DContext, ID3D11DeviceContext );
D3D_DECLARE_HANDLE( D3DComputeShader, ID3D11ComputeShader );
D3D_DECLARE_HANDLE( D3DBuffer, ID3D11Buffer );
D3D_DECLARE_HANDLE( D3DTexture2D, ID3D11Texture2D );
D3D_DECLARE_HANDLE( D3DSRV, ID3D11ShaderResourceView );
D3D_DECLARE_HANDLE( D3DUAV, ID3D11UnorderedAccessView );
D3D_DECLARE_HANDLE( D3DSampler, ID3D11SamplerState );

#endif // #if D3D_HELPER_TYPES
