// pbrendel (c) 2019-21

#include "shaderCache.h"
#include "d3d.h"

#include "Core/dynArray.h"
#include "Core/file.h"
#include "Core/map.h"

#include <d3dcompiler.h>
#include <iostream>
#include <string>

//////////////////////////////////////////////////////////////////////////

typedef size_t ShaderSignature;
typedef o::Map<ShaderSignature, ID3DBlob *> ShadersStorage;
typedef o::DynArray<std::string> IncludeDirectories;

static ShadersStorage s_storage;
static IncludeDirectories s_includeDirectories;

//////////////////////////////////////////////////////////////////////////

class SimpleShaderInclude : public ID3DInclude
{
public:

	virtual HRESULT __stdcall Open( D3D_INCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID *ppData, UINT *pBytes ) override
	{
		byte *fileData = nullptr;
		uint fileSize = 0;
		if ( File_ReadBinary( pFileName, &fileData, &fileSize ) )
		{
			*ppData = fileData;
			*pBytes = fileSize;
			return S_OK;
		}
		for ( const std::string &includeDir : s_includeDirectories )
		{
			char path[MAX_PATH] = { 0 };
			sprintf_s( path, "%s\\%s", includeDir.c_str(), pFileName );
			if ( File_ReadBinary( path, &fileData, &fileSize ) )
			{
				*ppData = fileData;
				*pBytes = fileSize;
				return S_OK;
			}
		}
		return S_FALSE;
	}

	virtual HRESULT __stdcall Close( LPCVOID pData ) override
	{
		const byte* buf = reinterpret_cast<const byte *>( pData );
		delete[] buf;
		return S_OK;
	}
};


static void ShaderCache_InitIncludeDirectories()
{
	if ( !s_includeDirectories.IsEmpty() )
	{
		return;
	}

	std::string thisPath( __FILE__ );
	const char *thisRelativePath = "\\Gfx\\shaderCache.cpp";
	const char *thisDir = strstr( thisPath.c_str(), thisRelativePath );
	assertex( thisDir != nullptr, "Fix this file relative path." );
	s_includeDirectories.PushBack( std::string( thisPath.c_str(), thisDir - thisPath.c_str() ) );
}


void ShaderCache_AddIncludeDirectory( const char *dir )
{
	ShaderCache_InitIncludeDirectories();
	s_includeDirectories.PushBack( dir );
}


static ID3DBlob *ShaderCache_Compile( const char *filename, const char *entryPoint )
{
	FileData fileData = File_ReadBinary( filename );
	if ( fileData.IsEmpty() )
	{
		return nullptr;
	}

	ShaderCache_InitIncludeDirectories();

#if D3D_DEBUG
	const UINT flags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else // #if D3D_DEBUG
	const UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#endif // #else // #if D3D_DEBUG

	const LPCSTR profile = "cs_5_0";
	const D3D_SHADER_MACRO defines[] = { nullptr, nullptr };

	SimpleShaderInclude shaderInclude;
	ID3DBlob* shaderBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;
	HRESULT hr = D3DCompile( fileData.Get(), fileData.GetSize(), nullptr, defines, &shaderInclude, entryPoint, profile, flags, 0, &shaderBlob, &errorBlob );
	if ( FAILED( hr ) )
	{
		if ( errorBlob )
		{
#if D3D_SHADER_COMPILER_OUTPUT
			const char *ptr = reinterpret_cast<const char *>( errorBlob->GetBufferPointer() );
			std::cout << "Shader compilation error: " << ptr << std::endl;
#endif // #if D3D_SHADER_COMPILER_OUTPUT
			errorBlob->Release();
		}
		if ( shaderBlob )
		{
			shaderBlob->Release();
		}
		return nullptr;
	}
	return shaderBlob;
}


ShaderSignature ShaderCache_GetSignature( const char *filename, const char *entryPoint )
{
	// Slow but should be enough for now.
	std::hash<std::string> hash;
	const size_t hashFilename = hash( filename );
	const size_t hashEntryPoint = hash( entryPoint );
	return hashFilename ^ hashEntryPoint;
}

//////////////////////////////////////////////////////////////////////////

ID3DBlob *ShaderCache_Get( const char *filename, const char *entryPoint )
{
	ShaderSignature sig = ShaderCache_GetSignature( filename, entryPoint );
	ID3DBlob *shaderBlob = s_storage[sig];
	if ( shaderBlob == nullptr )
	{
		shaderBlob = ShaderCache_Compile( filename, entryPoint );
		s_storage[sig] = shaderBlob;
	}
	return shaderBlob;
}


void ShaderCache_Shutdown()
{
	for ( auto &it : s_storage )
	{
		it.second->Release();
	}
}

