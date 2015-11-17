#include "FileIO.h"

// 생성자
FileIO::FileIO( )
{
	this->fp = NULL;
}

// 생성자
FileIO::FileIO( char* fileName, char* openMode )
{
	if ( (this->fp = fopen( fileName, openMode )) == NULL )
		cout << "Fopen Error!!!" << endl;
}

// 소멸자
FileIO::~FileIO( )
{
	if ( this->fp != NULL )
		fclose( this->fp );
}

// 파일에 데이터 저장
bool FileIO::FileWrite( int* Buffer, int ElementSize, int ElementCnt )
{
	//if ( fwrite( Buffer, ElementSize, ElementCnt, this->fp ) != ElementCnt )
	//	cout << "fwrite Failed!!! " << endl;
	for ( int i = 0; i < ElementCnt; i ++ )
		fprintf( fp, "%d ", Buffer[ i ] );

	return true;
}

// 파일에서 읽어 오기
bool FileIO::FileRead( int* Buffer, int ElementSize, int ElementCnt )
{
	//if ( fread( Buffer, ElementSize, ElementCnt, this->fp ) != ElementCnt)
	//	cout << "fread Failed!!!" << endl;
	for ( int i = 0; i < ElementCnt; i ++ )
		fscanf( fp, "%d ", & Buffer[ i ] );

	return true;
}

