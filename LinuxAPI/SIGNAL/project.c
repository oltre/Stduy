#include "repair.h"

char * filename = "test.txt";
struct student{
	int id;
	char name[20];
	float exams[10];
	float average;
};
struct student s[100];

main()
{
	int i, j, s_no, e_no;
	float t1;

	fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0644);

	Make_Process( ); // Child 프로세스 생성
	
	if (re)
	{ re = Repair_int(& s_no);	}
	else
	{
		printf("# of students >> ");
		Normal_int(& s_no); 
	}
	
	if (re)
	{ re = Repair_int(& e_no); }
	else
	{ 
		printf("# of exams >> ");
		Normal_int(& e_no); 
	}
	
	for (i=0;i<s_no;i++)
	{
		if (re)
		{ re = Repair_int(& s[i].id); }
		else
		{ 
			printf("Input Student id >> ");
			Normal_int (& s[i].id); 
		}
		
		if (re)
		{ re = Repair_string (s[i].name); }
		else
		{
			printf("Input Sutudent name >> ");
			Normal_string (s[i].name); 
		}
		
		t1 = 0.0;
		for (j=0;j<e_no;j++)
		{
			if (re)
			{ re = Repair_float (& s[i].exams[j]); }
			else
			{ 
				printf("%d Student course score >> ", i+1);
				Normal_float (& s[i].exams[j]); 
			}
			
			t1+=s[i].exams[j];
		}
		
		s[i].average = t1 / e_no;
		// 부모 프로세스에서 자식프로세스로 자식프로세스에서 부모프로세스로 신호 전달.
		// 복구 프로세스가 자식 프로세스를 일정간격을 두고 따라가게 해주기 위함이다.
		Transport_Signal( ); 
	}
	
	printf("\n");
	for (i=0;i<s_no;i++){
		printf("%d %s ", s[i].id, s[i].name);
		for (j=0;j<e_no;j++)
			printf("%.2f ", s[i].exams[j]);
		printf("%.2f\n", s[i].average);
	}
	
	// 자식 프로세스가 정상종료 하기전에 부모 프로세스에게 종료 신호를 보낸다.
	kill(prepid, SIGUSR2);
	exit(0);
}
