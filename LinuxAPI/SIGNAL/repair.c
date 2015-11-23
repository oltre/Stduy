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

// Child Processor ���� �Լ�
void Make_Process( )
{
	prepid = getpid( ); // �ڽ� ���μ����� �θ����μ������� ��ȣ�� ������ ���� �θ� pid���� �����س��´�.
	pid = fork( );

	if (pid > 0)
	{
		// �θ����μ����� �ñ׳� �ڵ鸵 ���� �κ�
		sigint.sa_handler = Repair_Sigint;
		sigaction(SIGINT, & sigint, NULL);

		sigusr.sa_handler = Repair_GoWith;
		sigaction(SIGUSR1, & sigusr, NULL);
		
		sigexit.sa_handler = ProcExit;
		sigaction(SIGUSR2, & sigexit, NULL);
		
		
		//  �ڽ� ���μ����� �����ҵ��� ��� ��Ų��.
		ProcessWait( );	

		// �ڽ� ���μ����� ���� �����ؾ� �� ���  �����ϰ� �ϱ� ����  MakeProc �Լ� ����.
		if (MakeProc)
		{
			// ������� �����ϴ� Child ���μ����� SIGINT��ȣ�� �޾Ƽ� ���߰� �Ǹ�
			// �θ����μ����� ����ϰ� �ִ� ���ο� ���μ����� �������־�� �ϴµ� 
			// ���� ���� ������Ű�� �ϱ� ���� �Լ��̴�.
			re = 1;			
			MakeProc= 0;
			Make_Process( );
			return;
		}
	}
	else if (pid == 0)
	{
		// �ڽ� ���μ����� �ñ׳� �޾��� ��� ó���ϱ� ���� ����
		sigint.sa_handler = ProcExit;
		sigaction(SIGINT, & sigint, NULL);

		sigusr.sa_handler = ProcExit;;
		sigaction(SIGUSR1, & sigusr, NULL);
		
		// ���� ������ �ڽ� ���μ����� ������ ó������ �������� ����Ǳ� ������ ���� �����͸� ó���κ����� ������ش�.
		lseek(fd, 0, SEEK_SET);
	}
}

// �ڽ����μ����� �����ϸ鼭 �θ� ���μ������� ���� �Ⱓ �������� ����� �� �ֵ��� �ϱ� ���� ��ȣ�� �����ϴ� �Լ�
void Transport_Signal( )
{
	if (prepid != getpid( ))
	{
		// �ڽ� ���μ����� �θ� ���μ������� SIGUSR1 ��ȣ�� ���� �� pause�Լ��� ����Ѵ�.
		kill (prepid, SIGUSR1);
		pause( );
	}
	else 
	{
		// �θ� ���μ����� �ڽ� ���μ������� SIGUSR1��ȣ��  �����Ѵ�.
		kill (pid, SIGUSR1);
		//  �ڽ� ���μ����� ���� �Ǳ� ������  ������� �����ϰ� �Ѵ�.
		re = 0;
		// ������ ũ�Ⱑ Ŀ���°� �����ϱ� ���� ���� ������ ������ ����� ���� �����Ѵ�.
		fd = open(filename, O_RDWR | O_TRUNC);
		Make_Process( );
	}
}

// �θ����μ����� ��� ��Ű�� ���� �Լ��� ���ѷ����� ���ȴ�.
void ProcessWait( )
{
	loop = 1;
	while (loop){ }
}

// SIGINT ��ȣ�� ������ �����ϴ� �Լ�
void Repair_Sigint(int signo)
{
	// �θ� ���μ����� �ڽ� ���μ����� ���� �� ������ ���鼭 ��� �ϰ� �ֱ⿡.
	// ������ �����Ű��, ���ο� ���μ����� �����ϵ��� ���� ����.
	loop = 0;
	MakeProc = 1;
}

// �θ� ���μ����� SIGUSR1 ��ȣ���޾��� ��� ����
void Repair_GoWith(int signo)
{
	// �ڽ� ���μ����� �θ� ���μ����� ���� �Ⱓ ���� ���� ������ ���� SIGUSR1 ��ȣ�� ������.
	// ���� ��ȣ�� �ް� �Ǹ� �θ����μ����� ������ ���� �� �������� ������ ó������ �����Ѵ�.
	loop = 0; 
	re = 1;
	lseek(fd, 0, SEEK_SET);
}

// �ڽ� ���μ����� ��ȣ�� �ް� �Ǹ� �����ϴ� ������ �س��� �Լ��̴�.
void ProcExit(int signo)
{  exit(1); }

