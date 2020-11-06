// pbrendel (c) 2019-21

#include "fakeShader.h"


void FakeShader_Dispatch( FakeShaderMain fsMain, uint sizeX, uint sizeY, uint sizeZ )
{
	uint3 threadID;
	for ( threadID.z = 0; threadID.z < sizeZ; ++threadID.z )
	{
		for ( threadID.y = 0; threadID.y < sizeY; ++threadID.y )
		{
			for ( threadID.x = 0; threadID.x < sizeX; ++threadID.x )
			{
				fsMain( threadID );
			}
		}
	}
}
