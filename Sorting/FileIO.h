#include "define.h"

class FileIO
{
private :
	FILE * fp;

public:
	FileIO( );
	FileIO( char* fileName, char* openMode );

	~FileIO( );

	bool FileWrite( int* Buffer, int ElementSize, int ElementCnt );
	bool FileRead( int* Buffer, int ElementSize, int ElementCnt );
};