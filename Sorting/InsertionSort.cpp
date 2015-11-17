#include "InsertionSort.h"

InsertionSort::InsertionSort( )
{

}

InsertionSort::~InsertionSort( )
{

}

void InsertionSort::Sort( int* arBase, int nSize )
{
	int i;
	int j;
	int pos;
	int sortTemp;

	for ( i = 1; i < nSize; i ++ )
	{
		sortTemp = arBase[ i ];
		pos = -1;

		for ( j = i - 1; j >= 0; j -- )
		{
			// compare
			if ( sortTemp < arBase[ j ] )
			{
				// shift
				arBase[ j + 1 ] = arBase[ j ];
				pos = j;
			}
			else 
				break;
		}

		if ( pos != -1 )
			arBase[ pos ] = sortTemp;
	}
}