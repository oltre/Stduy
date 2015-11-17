#include "SelectionSort.h"

SelectionSort::SelectionSort( )
{

}

SelectionSort::~SelectionSort( )
{

}

void SelectionSort::Sort( int* arBase, int nSize )
{
	int i;
	int j;
	int min;
	int minpos;
	int temp;

	for ( i = 0; i < nSize; i ++ )
	{
		min = arBase[ i ];
		minpos = 0;

		for ( j = i + 1; j < nSize; j ++ )
		{
			if ( min > arBase[ j ] )
			{
				min = arBase[ j ];
				minpos = j;
			}
		}

		if ( minpos != 0 )
		{
			temp = arBase[ i ];
			arBase[ i ] = arBase[ minpos ];
			arBase[ minpos ] = temp;
		}
	}
}