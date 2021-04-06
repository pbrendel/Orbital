// pbrendel (c) 2021

#include "matrix.h"


#define CLEAR_MATRIX 1


inline static void ClearMatrix( float4x4 &outMatrix )
{
#ifdef CLEAR_MATRIX
	memset( &outMatrix, 0, sizeof( outMatrix ) );
#endif // #ifdef CLEAR_MATRIX
}


inline static void ClearMatrix( float3x3 &outMatrix )
{
#ifdef CLEAR_MATRIX
	memset( &outMatrix, 0, sizeof( outMatrix ) );
#endif // #ifdef CLEAR_MATRIX
}


inline static void ClearMatrix( float2x2 &outMatrix )
{
#ifdef CLEAR_MATRIX
	memset( &outMatrix, 0, sizeof( outMatrix ) );
#endif // #ifdef CLEAR_MATRIX
}


void IdentityMatrix( float4x4 &outMatrix )
{
	ClearMatrix( outMatrix );
	outMatrix[0][0] = 1.0f;
	outMatrix[1][1] = 1.0f;
	outMatrix[2][2] = 1.0f;
	outMatrix[3][3] = 1.0f;
}


void IdentityMatrix( float3x3 &outMatrix )
{
	ClearMatrix( outMatrix );
	outMatrix[0][0] = 1.0f;
	outMatrix[1][1] = 1.0f;
	outMatrix[2][2] = 1.0f;
}


void IdentityMatrix( float2x2 &outMatrix )
{
	ClearMatrix( outMatrix );
	outMatrix[0][0] = 1.0f;
	outMatrix[1][1] = 1.0f;
}


void TranslationMatrix( const float3 &t, float4x4 &outMatrix )
{
	IdentityMatrix( outMatrix );
	outMatrix[3][0] = t.x;
	outMatrix[3][1] = t.y;
	outMatrix[3][2] = t.z;
}


void InfiniteProjectionMatrix( float tanHalfFovX, float tanHalfFovY, float nearZ, float4x4 &outMatrix )
{
	ClearMatrix( outMatrix );
	outMatrix[0][0] = 1.0f / tanHalfFovX;
	outMatrix[1][1] = 1.0f / tanHalfFovY;
	outMatrix[2][3] = 1.0f;
	outMatrix[3][2] = nearZ;
}


void ClipToScreenMatrix( float screenWidth, float screenHeight, float3x3 &outMatrix )
{
	ClearMatrix( outMatrix );
	outMatrix[0][0] = screenWidth * 0.5f;
	outMatrix[1][1] = -screenHeight * 0.5f;
	outMatrix[2][0] = screenWidth * 0.5f;
	outMatrix[2][1] = screenHeight * 0.5f;
	outMatrix[2][2] = 1.0f;
}
