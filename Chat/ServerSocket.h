#pragma once

#include <iostream>
#include <process.h>
#include <WinSock2.h>
#include <fstream>
#include <time.h>

#include <string>
#include <vector>
#include <list>

using namespace std;

class ServerSocket
{
private :
	WSADATA wsaData;
	SOCKET	ListenSocket;
	int		ListenPort;

	bool bContinue;

	vector<SOCKET>  vBlockingSocket;
	vector<SOCKET>	vNonBlockingSocket; // non-blocking socket

	vector<string>	vClientID;

	list<string>	lData;
	list<int>		lDataIdx;

	HANDLE hAcceptThread;
	unsigned AcceptThreadID;

	HANDLE hRecvThread;
	unsigned RecvThreadID;

public :
	// Constructor
	ServerSocket( );

	// Destructor
	~ServerSocket( );


	// Initialize Winsock & Create a socket.
	bool Initialize( );

	// Bind the Socket
	bool BindSocket( );

	// Listen on the socket for a client 
	bool ListenServer( );

	// Close Socket
	void Close( );

	// Send Data
	bool SendData( const char * data, int idx );

	// Send File
	bool SendFile ( const char * data, int size, int idx );

	// Accept Thread Start
	void AcceptStart( );

	// Accept Thread Stop
	void AcceptStop( );

	// Accept a connection from a clinet.
	static unsigned __stdcall AcceptClient( void* pArg );

	// RecvData Thread Start
	void RecvStart( );

	// RecvData Thread Stop
	void RecvStop( );

	// Recv Data
	static unsigned __stdcall RecvData( void* pArg );
};

// Constructor
ServerSocket::ServerSocket( )
: ListenSocket( INVALID_SOCKET )
  , bContinue( true )
{
}

// Destructor
ServerSocket::~ServerSocket( )
{
	Close( );
}

bool ServerSocket::Initialize( )
{
	int iResult;

	iResult = WSAStartup( MAKEWORD(2, 2), & wsaData );
	if ( iResult != 0 )
	{
		cout << "WSAStartup function failed with error : " << GetLastError( ) << endl;
		return false;
	}

	ListenSocket = socket( AF_INET, SOCK_STREAM, 0 );
	if ( ListenSocket == INVALID_SOCKET )
	{
		cout << "socket function failed with error : " << GetLastError( ) << endl;
		return false;
	}

	return true;
}

bool ServerSocket::BindSocket( )
{
	SOCKADDR_IN saServer;

	srand( (unsigned int) time( NULL ) );
	ListenPort = (rand( ) % 65535) + 1025;

	// Set up the sockaddr structure
	memset( & saServer, 0, sizeof( SOCKADDR_IN ) );
	saServer.sin_family				= AF_INET;
	saServer.sin_addr.S_un.S_addr	= htonl( INADDR_ANY );
	/*saServer.sin_port				= htons( ListenPort );*/
	saServer.sin_port				= htons( 50000 );

	if ( bind ( ListenSocket, 
		(SOCKADDR *) & saServer, 
		sizeof( saServer ) ) == SOCKET_ERROR )
	{
		cout << "bind function failed with error : " << GetLastError( ) << endl;
		return false;
	}

	return true;
}


bool ServerSocket::ListenServer( )
{
	if ( listen( ListenSocket, SOMAXCONN ) == SOCKET_ERROR )
	{
		cout << "listen function failed with error : " << GetLastError( ) << endl;
		return false;
	}

	return true;
}

void ServerSocket::Close( )
{
	if ( ListenSocket != INVALID_SOCKET )
		closesocket( ListenSocket );

	WSACleanup( );
}

void ServerSocket::AcceptStart( )
{
	hAcceptThread = (HANDLE) _beginthreadex( NULL, 0, AcceptClient, this, 0, & AcceptThreadID );
}

void ServerSocket::AcceptStop( )
{
	TerminateThread( hAcceptThread, 0 );
}

unsigned __stdcall ServerSocket::AcceptClient( void * pArg )
{
	ServerSocket * s = (ServerSocket *) pArg;

	while ( s->bContinue )
	{
		SOCKET ClientSocket = INVALID_SOCKET;
		
		ClientSocket = accept( s->ListenSocket, NULL, NULL );
		if ( ClientSocket == INVALID_SOCKET )
		{
			cout << "accept failed with error : " << GetLastError( ) << endl;
			continue;
		}
		
		char arCharID[ 20 ];
		memset( arCharID, 0, sizeof( arCharID ) );

		// 클라이언트의 아이디 받아오기.
		if ( recv( ClientSocket, arCharID, sizeof( arCharID ), 0 ) <= 0 ) 
			continue;

		string ret = arCharID;
		s->vClientID.push_back( ret );

		// blocking 소켓 저장
		s->vBlockingSocket.push_back( ClientSocket );

		// non-blocking 소켓으로 변경
		unsigned long flags;

		flags = 1;
		ioctlsocket( ClientSocket, FIONBIO, & flags );

		// non-blocking 소켓 저장.
		s->vNonBlockingSocket.push_back( ClientSocket );
	}

	return 0;
}

void ServerSocket::RecvStart( )
{
	hRecvThread = (HANDLE) _beginthreadex( NULL, 0, RecvData, this, 0, & RecvThreadID );
}

void ServerSocket::RecvStop( )
{
	bContinue = false;
}

unsigned __stdcall ServerSocket::RecvData( void * pArg )
{
	ServerSocket * s = (ServerSocket *) pArg;

	string ret;
	while ( s->bContinue )
	{
		for ( unsigned int i = 0; i < s->vNonBlockingSocket.size( ); i ++ )
		{
			if ( s->vNonBlockingSocket[ i ] == INVALID_SOCKET )
				continue;

			while ( true )  
			{
				char r;
				if ( recv( s->vNonBlockingSocket[ i ], &r, 1, 0 ) <= 0 ) 
					break;
				
				ret += r;

				if ( r == '\n' )
				{
					// 클라이언트 종료시 연결 소켓 종료
					if ( ! ret.compare( "exit\n") )
					{
						closesocket( s->vNonBlockingSocket[ i ] );
						closesocket( s->vBlockingSocket[ i ] );

						s->vNonBlockingSocket[ i ] = INVALID_SOCKET;
						s->vBlockingSocket[ i ] = INVALID_SOCKET;

						break;
					}

					if ( ! ret.compare( "file\n" ) )
					{
						// 클라이언트들에게 파일 전송할 것을 알림
						s->SendData( ret.c_str( ), i );

						// 파일명 받기
						string fileName;


						// 파일 이름 받아오기
						while ( true )  
						{
							char r;
							if ( recv( s->vNonBlockingSocket[ i ], &r, 1, 0 ) <= 0  ) 
								continue;

							fileName += r;

							if ( r == '\n' )
								break;
						}

						// 파일명을 전송한다.
						s->SendData( fileName.c_str( ), i );

						// 파일 사이즈 전달 받기
						string fileSize;

						// 파일 사이즈 받아오기
						while ( true )  
						{
							char r;
							if ( recv( s->vNonBlockingSocket[ i ], &r, 1, 0 ) <= 0  ) 
								continue;

							fileSize += r;

							if ( r == '\n' )
								break;
						}
						s->SendData( fileSize.c_str( ), i );

						// 파일 내용 전달 받기
						int size = atol( fileSize.c_str( ) );
						char * fileData = new char[ size ];
						int recvFileData = recv( s->vNonBlockingSocket[ i ], fileData, size, 0 );

						// 클라이언트들에게 파일 데이터 전송
						s->SendFile( fileData, size, i );

						// 파일 데이터 저장
						fstream file;
						string::iterator itr = fileName.end() - 1;
						fileName.erase( itr );
						cout << fileName.c_str();
						file.open( fileName.c_str( ), ios::trunc | ios::out );

						file << fileData;

						fileName.clear( );
						file.close( );

						delete [] fileData;

						ret.clear( );
						break;
					}

					s->SendData( ret.c_str( ), i );
					break;
				}
			}

			cout << ret;
			ret.clear( );
		}

		Sleep( 10 );
	}

	return 0;
}

bool ServerSocket::SendData( const char * data, int idx )
{
	for ( unsigned int i = 0; i < vNonBlockingSocket.size( ); i ++ )
	{
		if ( vNonBlockingSocket[ i ] == INVALID_SOCKET )
			continue;

		if ( i != idx )
			send( vNonBlockingSocket[ i ], data, strlen( data ), 0 );
	}

	return true;
}

bool ServerSocket::SendFile ( const char * data, int size, int idx )
{
	for ( unsigned int i = 0; i < vNonBlockingSocket.size( ); i ++ )
	{
		if ( vNonBlockingSocket[ i ] == INVALID_SOCKET )
			continue;

		if ( i != idx )
			send( vNonBlockingSocket[ i ], data, size, 0 );
	}

	return true;
}