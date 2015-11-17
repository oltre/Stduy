#include "ShellSort.h"

ShellSort::ShellSort( )
{

}

ShellSort::~ShellSort( )
{

}

void ShellSort::Sort( int* arBase, int nSize )
{
	int h;
	int i;
	int j;
	int k;
	int sortTemp;
	int pos;

	for ( h = (nSize - 1) / 3; h > 0; h = h / 2 )
	{
		for ( i = 0; i < h; i ++ )
		{
			for ( j = i + h; j < nSize; j = j + h )
			{
				sortTemp = arBase[ j ];
				pos = -1;

				for ( k = j - h; k >= 0; k = k - h )
				{
					if ( sortTemp < arBase[ k ] )
					{
						arBase[ k + h ] = arBase[ k ];
						pos = k;
					}
					else
						break;
				}

				if ( pos != -1 )
					arBase[ pos ] = sortTemp;
			}
		}
	}
}