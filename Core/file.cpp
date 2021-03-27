// pbrendel (c) 2019-21

#include "file.h"

#include <cstdio>


bool File_ReadBinary( const char *filename, byte **outFileData, uint *outFileSize )
{
	FILE *fp = fopen( filename, "rb" );
	if ( !fp )
	{
		return false;
	}

	fseek( fp, 0, SEEK_END );
	const uint fileSize = static_cast<uint>( ftell( fp ) );
	fseek( fp, 0, SEEK_SET );

	byte *fileData = new byte[fileSize];
	if ( fread( fileData, sizeof( byte ), fileSize, fp ) == fileSize )
	{
		*outFileData = fileData;
		if ( outFileSize != nullptr )
		{
			*outFileSize = fileSize;
		}
	}
	else
	{
		delete [] fileData;
		fileData = nullptr;
	}
	fclose( fp );
	return fileData != nullptr;
}


FileData File_ReadBinary( const char *filename )
{
	FileData fileData;
	byte *fileDataBuffer = nullptr;
	uint fileDataSize = 0;
	if ( File_ReadBinary( filename, &fileDataBuffer, &fileDataSize ) )
	{
		fileData.Reset( fileDataBuffer, fileDataSize );
	}
	return fileData;
}


bool File_WriteBinary( const char *filename, const void *data, uint dataSize )
{
	FILE *fp = fopen( filename, "wb" );
	if ( !fp )
	{
		return false;
	}
 
	const bool res = ( fwrite( data, 1, dataSize, fp ) == dataSize );
	fclose( fp );
	
	return res;
}