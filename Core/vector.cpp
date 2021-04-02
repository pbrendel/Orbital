// pbrendel (c) 2019-21

#include "vector.h"


float3 cross( const float3 &a, const float3 &b )
{
	float3 c;
	c.x = a.y * b.z - a.z * b.y;
	c.y = a.z * b.x - a.x * b.z;
	c.z = a.x * b.y - a.y * b.x;
	return c;
}


float dot( const float2 &a, const float2 &b )
{
	return a.x * b.x + a.y * b.y;
}


float dot( const float3 &a, const float3 &b )
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}


float dot( const float4 &a, const float4 &b )
{
	return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}


float length( const float2 &v )
{
	return sqrt( dot( v, v ) );
}


float length( const float3 &v )
{
	return sqrt( dot( v, v ) );
}


float length( const float4 &v )
{
	return sqrt( dot( v, v ) );
}


float2 normalize( const float2 &v )
{
	const float len = length( v );
	return ( len > 0.00001f ) ? ( v / len ) : v;
}


float3 normalize( const float3 &v )
{
	const float len = length( v );
	return ( len > 0.00001f ) ? ( v / len ) : v;
}


float4 normalize( const float4 &v )
{
	const float len = length( v );
	return ( len > 0.00001f ) ? ( v / len ) : v;
}
