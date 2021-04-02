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
void sincos( float rad, float &outSin, float &outCos );
using std::min;
using std::max;
bool any( bool2 b );
bool any( bool3 b );
bool any( bool4 b );
bool all( bool2 b );
bool all( bool3 b );
bool all( bool4 b );
void InterlockedAdd( uint &dest, uint value );
void InterlockedAnd( uint &dest, uint value );
void InterlockedOr( uint &dest, uint value );
