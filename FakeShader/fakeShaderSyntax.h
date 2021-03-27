// pbrendel (c) 2019-21

#pragma once

#include "Core/vector.h"

#include <algorithm>


const float &asfloat( const uint &i );
const float2 &asfloat( const uint2 &i );
const float3 &asfloat( const uint3 &i );
const float4 &asfloat( const uint4 &i );
const int &asint( const float &f );
const uint &asuint( const float &f );
const uint2 &asuint( const float2 &f );
const uint3 &asuint( const float3 &f );
const uint4 &asuint( const float4 &f );
uint countbits( uint i );
uint firstbithigh( uint i );
uint firstbitlow( uint i );
uint reversebits( uint i );
float2 round( const float2 &f );
float3 round( const float3 &f );
float4 round( const float4 &f );
float2 frac( const float2 &v );
float3 frac( const float3 &v );
float4 frac( const float4 &v );
float clamp( float f, float l, float h );
float lerp( float a, float b, float t );
float2 lerp( const float2 &a, const float2 &b, float t );
float3 lerp( const float3 &a, const float3 &b, float t );
float4 lerp( const float4 &a, const float4 &b, float t );
float sign( float f );
float rcp( float f );
float step( float a, float b );
float2 step( const float2 &a, const float2 &b );
float3 step( const float3 &a, const float3 &b );
float4 step( const float4 &a, const float4 &b );
float saturate( float f );
float dot( const float2 &a, const float2 &b );
float dot( const float3 &a, const float3 &b );
float dot( const float4 &a, const float4 &b );
float3 cross( const float3 &a, const float3 &b );
float length( const float2 & v );
float length( const float3 & v );
float length( const float4 & v );
float2 normalize( const float2 &v );
float3 normalize( const float3 &v );
float4 normalize( const float4 &v );
using std::min;
using std::max;
float2 min( const float2 &a, const float2 &b );
float3 min( const float3 &a, const float3 &b );
float4 min( const float4 &a, const float4 &b );
float2 max( const float2 &a, const float2 &b );
float3 max( const float3 &a, const float3 &b );
float4 max( const float4 &a, const float4 &b );
bool any( bool2 b );
bool any( bool3 b );
bool any( bool2 b );
bool all( bool2 b );
bool all( bool3 b );
bool all( bool2 b );
void InterlockedAdd( uint &dest, uint value );
void InterlockedAnd( uint &dest, uint value );
void InterlockedOr( uint &dest, uint value );
