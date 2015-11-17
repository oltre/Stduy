#include "QuickSort.h"

QuickSort::QuickSort( )
{

}

QuickSort::~QuickSort( )
{

}

void QuickSort::Sort( int* arBase, int LeftPos, int RightPos )
{ 
	int Pivot = arBase[ LeftPos ];
	int PivotPos;
	int LP = LeftPos + 1;
	int RP = RightPos;
	int temp;

	do
	{
		// Pivot보다 작은 원소가 있으면 Left는 계속 증가
		while ( arBase[ LP ] <= Pivot )
		{
			if ( LP < RightPos )
				LP ++;
			else
				break;
		}

		// Pivot보다 큰 원소가 있으면 Right는 계속 감소
		while ( arBase[ RP ] >= Pivot )
		{
			if ( RP > LeftPos )
				RP --;
			else
				break;
		}

		// Right 위치가 Left 위치보다 큰 경우 원소 교환
		if ( RP > LP )
		{
			temp = arBase[ LP ];
			arBase[ LP ] = arBase[ RP ];
			arBase[ RP ] = temp;
		}

	} while ( RP > LP );

	// Pivot 위치 교환
	// RP에 교환하는 이유는 RP와 LP가 무조건 엇갈리기 때문이다.
	arBase[ LeftPos ] = arBase[ RP ];
	arBase[ RP ] = Pivot;
	PivotPos = RP;

	if ( LeftPos < PivotPos - 1 )
		Sort( arBase, LeftPos, PivotPos - 1 );
	if ( PivotPos + 1 < RightPos )
		Sort( arBase, PivotPos + 1, RightPos );
}