// pbrendel (c) 2019-21

#pragma once

#include "Core/types.h"

#include <d3d11.h>


ID3DBlob *ShaderCache_Get( const char *filename, const char *entryPoint );
void ShaderCache_Shutdown();

