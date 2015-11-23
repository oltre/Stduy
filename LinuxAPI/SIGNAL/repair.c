#include "repair.h"

ssize_t nread;
//////////////// Signal  Handler Function ////////////////
void Repair_Sigint(int); 
void Repair_GoWith(int);
void ProcExit(int);

static struct sigaction sigint;
static struct sigaction sigusr;
static struct sigaction sigexit;

extern char * filename;

void Normal_int (int * i)
{ 
	scanf("%d", i);
	write(fd, i, sizeof(int)); 
}
int Repair_int (int * i)
{ 
	if ((nread = read(fd, i, sizeof(int))) <= 0)
	{
		Normal_int(i);
		return 0;
	} 
	else
		return 1;
}

void Normal_string (char * s)
{ 
	scanf("%s", s);
	write(fd, s, NAME_SIZE); 
}
int Repair_string (char * s)
{
	if ((nread = read(fd, s, NAME_SIZE)) <= 0)
	{
		Normal_string(s);
		return 0;
	}
	else
		return 1;
}

void Normal_float (float * f)
{ 
	scanf("%f", f);
	write(fd, f, sizeof(float));  
}
int Repair_float (float * f)
{ 
	if ((nread = read(fd, f, sizeof(float))) <= 0)
	{
		Normal_float(f);
		return 0;
	}
	else
		return 1;
}

// Child Processor 생성 함수
void Make_Process( )
{
	prepid = getpid( ); // 자식 프로세서가 부모프로세서에게 신호를 보내기 위해 부모 pid값을 저장해놓는다.
	pid = fork( );

	if (pid > 0)
	{
		// 부모프로세서의 시그널 핸들링 설정 부분
		sigint.sa_handler = Repair_Sigint;
		sigaction(SIGINT, & sigint, NULL);

		sigusr.sa_handler = Repair_GoWith;
		sigaction(SIGUSR1, & sigusr, NULL);
		
		sigexit.sa_handler = ProcExit;
		sigaction(SIGUSR2, & sigexit, NULL);
		
		
		//  자식 프로세서가 동작할동안 대기 시킨다.
		ProcessWait( );	

		// 자식 프로세서를 새로 생성해야 할 경우  동작하게 하기 위해  MakeProc 함수 저장.
		if (MakeProc)
		{
			// 정상모드로 동작하던 Child 프로세서가 SIGINT신호를 받아서 멈추게 되면
			// 부모프로세서가 대기하고 있다 새로운 프로세서를 생성해주어야 하는데 
			// 복구 모드로 생성시키게 하기 위한 함수이다.
			re = 1;			
			MakeProc= 0;
			Make_Process( );
			return;
		}
	}
	else if (pid == 0)
	{
		// 자식 프로세서의 시그널 받았을 경우 처리하기 위한 설정
		sigint.sa_handler = ProcExit;
		sigaction(SIGINT, & sigint, NULL);

		sigusr.sa_handler = ProcExit;;
		sigaction(SIGUSR1, & sigusr, NULL);
		
		// 새로 생성된 자식 프로세서는 파일의 처음부터 복구모드로 진행되기 때문에 파일 포인터를 처음부분으로 만들어준다.
		lseek(fd, 0, SEEK_SET);
	}
}

// 자식프로세서가 동작하면서 부모 프로세서에게 일정 기간 복구모드로 따라올 수 있도록 하기 위해 신호를 전달하는 함수
void Transport_Signal( )
{
	if (prepid != getpid( ))
	{
		// 자식 프로세서는 부모 프로세서에게 SIGUSR1 신호를 전달 후 pause함수로 대기한다.
		kill (prepid, SIGUSR1);
		pause( );
	}
	else 
	{
		// 부모 프로세서는 자식 프로세서에게 SIGUSR1신호를  전달한다.
		kill (pid, SIGUSR1);
		//  자식 프로세서는 종료 되기 때문에  정상모드로 동작하게 한다.
		re = 0;
		// 파일의 크기가 커지는걸 방지하기 위해 기존 파일의 내용을 지우고 새로 오픈한다.
		fd = open(filename, O_RDWR | O_TRUNC);
		Make_Process( );
	}
}

// 부모프로세서를 대기 시키기 위한 함수로 무한루프를 돌렸다.
void ProcessWait( )
{
	loop = 1;
	while (loop){ }
}

// SIGINT 신호를 받으면 동작하는 함수
void Repair_Sigint(int signo)
{
	// 부모 프로세서는 자식 프로세서를 생성 후 루프를 돌면서 대기 하고 있기에.
	// 루프를 종료시키고, 새로운 프로세서를 생성하도록 변수 설정.
	loop = 0;
	MakeProc = 1;
}

// 부모 프로세서가 SIGUSR1 신호를받았을 경우 동작
void Repair_GoWith(int signo)
{
	// 자식 프로세서가 부모 프로세서를 일정 기간 복구 지점 갱신을 위해 SIGUSR1 신호를 보낸다.
	// 따라서 신호를 받게 되면 부모프로세서는 루프를 멈춘 후 복구모드로 파일의 처음부터 동작한다.
	loop = 0; 
	re = 1;
	lseek(fd, 0, SEEK_SET);
}

// 자식 프로세서는 신호를 받게 되면 종료하는 동작을 해놓는 함수이다.
void ProcExit(int signo)
{  exit(1); }

