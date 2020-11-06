// pbrendel (c) 2019-21

#pragma once

#include "fakeShaderSyntax.h"
#include "fakeShaderBuffers.h"
#include "fakeShaderTextures.h"

#include <functional>


#define FAKE_SHADER 1


typedef std::function< void( uint3 ) > FakeShaderMain;

void FakeShader_Dispatch( FakeShaderMain fsMain, uint sizeX, uint sizeY, uint sizeZ );
