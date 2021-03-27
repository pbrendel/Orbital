// pbrendel (c) 2019-21

#pragma once


struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11Query;


class D3DPerfCounter
{
public:

	D3DPerfCounter( ID3D11Device *device );
	~D3DPerfCounter();

	void Begin( ID3D11DeviceContext *context );
	void End( ID3D11DeviceContext *context );

	double GetTime( ID3D11DeviceContext *context ) const;

private:

	ID3D11Query	*m_begin;
	ID3D11Query	*m_end;
	ID3D11Query	*m_disjoint;
};