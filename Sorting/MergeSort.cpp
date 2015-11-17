#include "MergeSort.h"

MergeSort::MergeSort( )
{

}

MergeSort::~MergeSort( )
{

}

void MergeSort::Sort( int* arBase, int nSize )
{ 
	int i, j, k;
	int nPtrSize1 = nSize / 2;
	int nPtrSize2 = nSize - nPtrSize1;
	int* arPtr1;
	int* arPtr2;

	arPtr1 = new int[ nPtrSize1 * sizeof( int ) ];
	memcpy( arPtr1, arBase, nPtrSize1 * sizeof( int ) );

	arPtr2 = new int[ nPtrSize2 * sizeof( int ) ];
	memcpy( arPtr2, arBase + nPtrSize1, nPtrSize2 * sizeof( int ) );

	if ( nSize > 1 )
	{
		Sort( arPtr1, nPtrSize1 );
		Sort( arPtr2, nPtrSize2 );
	}

	for ( i = 0, j = 0, k = 0; i < nSize; i ++ )
	{
		if ( arPtr1[ j ] <= arPtr2[ k ] )
		{
			if ( arPtr1[ j ] < 0 )
				arBase[ i ] = arPtr2[ k ++ ];
			else
				arBase[ i ] = arPtr1[ j ++ ];
		}
		else
		{
			if ( arPtr2[ k ] < 0 )
				arBase[ i ] = arPtr1[ j ++ ];
			else
				arBase[ i ] = arPtr2[ k ++ ];
		}
	}

	delete [] arPtr1;
	delete [] arPtr2;
}