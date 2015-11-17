#include <Windows.h>
#include <time.h>

#include "FileIO.h"
#include "BubbleSort.h"
#include "SelectionSort.h"
#include "InsertionSort.h"
#include "ShellSort.h"
#include "MergeSort.h"
#include "QuickSort.h"

#define SORT_METHOD		6

#define BUBBLE			0
#define SELECTION		1
#define INSERTION		2
#define SHELL			3
#define	MERGE			4
#define QUICK			5

int main( )
{
	long iSelect;
	long lStartTime;
	long lEndTime;
	int rNumber;
	int* arWriteNumber;
	int* arReadNumber;
	int i;

	char* FileName[ ] = { 
		"bubble", 
		"selection", 
		"insertion",
		"shell",
		"merge",
		"quick"
	};

	char* SortFileName[ ] = {
		"bubbleSort",
		"selectionSort",
		"insertionSort",
		"shellSort",
		"mergeSort",
		"quickSort"
	};

	FileIO* fileIO[ 6 ];

	BubbleSort		bubble;
	SelectionSort	selection;
	InsertionSort	insertion;
	ShellSort		shell;
	MergeSort		merge;
	QuickSort		quick;

	srand( (unsigned int) time( NULL ) );
	while ( true )
	{
		printf( "테스트 할 데이터 갯수 입력 (1 ~ 100000)(0 : 종료) >>> ");
		scanf ( "%ld", & iSelect );

		if ( iSelect == 0 )
			break;

		if ( iSelect > 100000 )
			continue;

		arWriteNumber = new int [ sizeof( int ) * iSelect ];

		// Create Random Number
		for ( i = 0; i < iSelect; i ++ )
		{
			arWriteNumber[ i ] = rand( );
		}		

		// write file
		for ( i = 0; i < SORT_METHOD; i ++ )
		{
			fileIO[ i ] = new FileIO( FileName[ i ], "wb" );
			fileIO[ i ]->FileWrite( arWriteNumber, sizeof( int ), iSelect );
			delete fileIO[ i ];
		}

		//cout << "Before >> ";
		//for ( int k = 0; k < iSelect; k ++ )
		//	cout << arWriteNumber[ k ] << ", ";
		//cout << endl;

		delete [] arWriteNumber;


		// read file
		for ( i = 0; i < SORT_METHOD; i ++ )
		{
			arReadNumber = new int [ sizeof( int ) * iSelect ];

			fileIO[ i ] = new FileIO( FileName[ i ], "rb" );
			fileIO[ i ]->FileRead( arReadNumber, sizeof( int ), iSelect );

			lStartTime = GetTickCount( );
			switch ( i )
			{
			case BUBBLE:
				cout << "1. Bubble Sort" << endl;
				bubble.Sort( arReadNumber, iSelect );
				break;

			case SELECTION:
				cout << "2. Selection Sort" << endl;
				selection.Sort( arReadNumber, iSelect );
				break;

			case INSERTION:
				cout << "3. Insertion Sort" << endl;
				insertion.Sort( arReadNumber, iSelect );
				break;

			case SHELL:
				cout << "4. Shell Sort" << endl;
				shell.Sort( arReadNumber, iSelect );

				//cout << "After >> ";
				//for ( int k = 0; k < iSelect; k ++ )
				//	cout << arReadNumber[ k ] << ", ";
				//cout << endl;
				break;

			case MERGE:
				cout << "5. Merge Sort" << endl;
				merge.Sort( arReadNumber, iSelect );

				//cout << "After >> ";
				//for ( int k = 0; k < iSelect; k ++ )
				//	cout << arReadNumber[ k ] << ", ";
				//cout << endl;

				break;

			case QUICK:
				cout << "6. Quick Sort" << endl;
				quick.Sort( arReadNumber, 0, iSelect - 1 );

				//cout << "After >> ";
				//for ( int k = 0; k < iSelect; k ++ )
				//	cout << arReadNumber[ k ] << ", ";
				//cout << endl;

				break;
			}
			lEndTime = GetTickCount( );

			cout << "StartTime : " << lStartTime << " ms" << endl;
			cout << "EndTime   : " << lEndTime << " ms" << endl;
			cout << "Tick Difference : " << lEndTime - lStartTime << " ms" << endl;
			cout << "Second Difference : " << ( lEndTime - lStartTime ) / 1000 << " sec" << endl << endl;

			delete fileIO[ i ];

			// 정렬 된 내용 파일 작성
			fileIO[ i ] = new FileIO( SortFileName[ i ], "wb" );
			fileIO[ i ]->FileWrite( arReadNumber, sizeof( int ), iSelect );
			delete fileIO[ i ];

			delete [] arReadNumber;
		}
	} 

	return 0;
}