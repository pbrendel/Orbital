// pbrendel (c) 2019-21

#include "d3dPerfCounter.h"

#include <d3d11.h>
#include <cstdint>


D3DPerfCounter::D3DPerfCounter( ID3D11Device *device )
	: m_begin( nullptr )
	, m_end( nullptr )
	, m_disjoint( nullptr )
{
	D3D11_QUERY_DESC queryDesc = { 0 };
	queryDesc.Query = D3D11_QUERY_TIMESTAMP;
	device->CreateQuery( &queryDesc, &m_begin );
	device->CreateQuery( &queryDesc, &m_end );
	queryDesc.Query = D3D11_QUERY_TIMESTAMP_DISJOINT;
	device->CreateQuery( &queryDesc, &m_disjoint );
}


D3DPerfCounter::~D3DPerfCounter()
{
	if ( m_begin != nullptr )
	{
		m_begin->Release();
	}
	if ( m_end != nullptr )
	{
		m_end->Release();
	}
	if ( m_disjoint != nullptr )
	{
		m_disjoint->Release();
	}
}


void D3DPerfCounter::Begin( ID3D11DeviceContext *context )
{
	context->Begin( m_disjoint );
	context->End( m_begin );
}


void D3DPerfCounter::End( ID3D11DeviceContext *context )
{
	context->End( m_end );
	context->End( m_disjoint );
}


double D3DPerfCounter::GetTime( ID3D11DeviceContext *context ) const
{
	uint64_t beginTicks;
	uint64_t endTicks;
	D3D11_QUERY_DATA_TIMESTAMP_DISJOINT disjoint;

	while ( context->GetData( m_disjoint, &disjoint, sizeof( disjoint ), 0 ) != S_OK )
	{
		Sleep( 0 );
	}

	if ( disjoint.Disjoint == TRUE )
	{
		return 0.0f;
	}

	while ( context->GetData( m_begin, &beginTicks, sizeof( beginTicks ), 0 ) != S_OK )
	{
		Sleep( 0 );
	}

	while ( context->GetData( m_end, &endTicks, sizeof( endTicks ), 0 ) != S_OK )
	{
		Sleep( 0 );
	}

	const double beginTime = static_cast<double>( beginTicks ) / disjoint.Frequency;
	const double endTime = static_cast<double>( endTicks ) / disjoint.Frequency;
	return endTime - beginTime;
}