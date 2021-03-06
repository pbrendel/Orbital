// pbrendel (c) 2020-21

#pragma once

template< typename T >
void D3D_ReadBack( ID3D11Device *d3dDevice, ID3D11Buffer *gpuBuffer, uint elementsCount, o::DynBuffer<T> &cpuBuffer )
{
	ID3D11DeviceContext *d3dContext = D3D_GetImmediateContext( d3dDevice );
	ID3D11Buffer *cpuAccessBuffer = D3D_CreateCpuAccessBuffer( d3dDevice, d3dContext, gpuBuffer, true );

	D3D11_MAPPED_SUBRESOURCE MappedResource;
	d3dContext->Map( cpuAccessBuffer, 0, D3D11_MAP_READ, 0, &MappedResource );
	cpuBuffer.Reset( elementsCount );
	memcpy( cpuBuffer.Get(), MappedResource.pData, elementsCount * sizeof( T ) );
	d3dContext->Unmap( cpuAccessBuffer, 0 );

	cpuAccessBuffer->Release();
	d3dContext->Release();
}
