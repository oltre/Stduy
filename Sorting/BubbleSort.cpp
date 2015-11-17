#include "BubbleSort.h"

BubbleSort::BubbleSort( )
{

}

BubbleSort::~BubbleSort( )
{

}

void BubbleSort::Sort( int * arBase, int nSize )
{
	int i;
	int j;
	int temp;

	for ( i = nSize - 1; i >= 0; i -- )
	{
		for ( j = 0; j < i; j ++ )
		{
			if ( arBase[ j ] > arBase[ j + 1 ] )
			{
				temp = arBase[ j ];
				arBase[ j ] = arBase[ j + 1 ];
				arBase[ j + 1 ] = temp;
			}
		}
	}
}