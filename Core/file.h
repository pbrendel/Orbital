// pbrendel (c) 2019-21

#pragma once

#include "dynBuffer.h"


typedef o::DynBuffer<byte> FileData;


bool File_ReadBinary( const char *filename, byte **outFileData, uint *outFileSize );
FileData File_ReadBinary( const char *filename );

bool File_WriteBinary( const char *filename, const void *data, uint dataSize );
template< typename T >
bool File_WriteBinary( const char *filename, const o::DynBuffer<T> &buffer )
{
	return File_WriteBinary( filename, reinterpret_cast<const void *>( buffer.Get() ), static_cast<uint>( buffer.GetSize() * sizeof( T ) ) );
}
