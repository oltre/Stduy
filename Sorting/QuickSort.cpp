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
		// Pivot���� ���� ���Ұ� ������ Left�� ��� ����
		while ( arBase[ LP ] <= Pivot )
		{
			if ( LP < RightPos )
				LP ++;
			else
				break;
		}

		// Pivot���� ū ���Ұ� ������ Right�� ��� ����
		while ( arBase[ RP ] >= Pivot )
		{
			if ( RP > LeftPos )
				RP --;
			else
				break;
		}

		// Right ��ġ�� Left ��ġ���� ū ��� ���� ��ȯ
		if ( RP > LP )
		{
			temp = arBase[ LP ];
			arBase[ LP ] = arBase[ RP ];
			arBase[ RP ] = temp;
		}

	} while ( RP > LP );

	// Pivot ��ġ ��ȯ
	// RP�� ��ȯ�ϴ� ������ RP�� LP�� ������ �������� �����̴�.
	arBase[ LeftPos ] = arBase[ RP ];
	arBase[ RP ] = Pivot;
	PivotPos = RP;

	if ( LeftPos < PivotPos - 1 )
		Sort( arBase, LeftPos, PivotPos - 1 );
	if ( PivotPos + 1 < RightPos )
		Sort( arBase, PivotPos + 1, RightPos );
}