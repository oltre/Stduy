#include "FileIO.h"

// ������
FileIO::FileIO( )
{
	this->fp = NULL;
}

// ������
FileIO::FileIO( char* fileName, char* openMode )
{
	if ( (this->fp = fopen( fileName, openMode )) == NULL )
		cout << "Fopen Error!!!" << endl;
}

// �Ҹ���
FileIO::~FileIO( )
{
	if ( this->fp != NULL )
		fclose( this->fp );
}

// ���Ͽ� ������ ����
bool FileIO::FileWrite( int* Buffer, int ElementSize, int ElementCnt )
{
	//if ( fwrite( Buffer, ElementSize, ElementCnt, this->fp ) != ElementCnt )
	//	cout << "fwrite Failed!!! " << endl;
	for ( int i = 0; i < ElementCnt; i ++ )
		fprintf( fp, "%d ", Buffer[ i ] );

	return true;
}

// ���Ͽ��� �о� ����
bool FileIO::FileRead( int* Buffer, int ElementSize, int ElementCnt )
{
	//if ( fread( Buffer, ElementSize, ElementCnt, this->fp ) != ElementCnt)
	//	cout << "fread Failed!!!" << endl;
	for ( int i = 0; i < ElementCnt; i ++ )
		fscanf( fp, "%d ", & Buffer[ i ] );

	return true;
}

