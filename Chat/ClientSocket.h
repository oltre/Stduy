#pragma once

#include <iostream>
#include <process.h>
#include <WinSock2.h>
#include <fstream>

using namespace std;

class ClientSocket 
{
private :
	WSADATA wsaData;
	SOCKET ConnectSocket;

	bool bContinue;

	HANDLE hRecvThread;
	unsigned RecvThreadID;

public :
	// Constructor
	ClientSocket( );
	// Destructor
	~ClientSocket( );

	// Initialize Winsock & Create a socket.
	bool Initialize( );

	// Connect to server
	bool ConnectServer( char * str, int port );

	// Close Socket
	void Close( );

	// Data Send
	bool SendData( const char * data );

	// File Send
	bool SendFile( const char * data, int size );

	// Recv Thread Start 
	void RecvStart( );

	// Recv Thread Stop
	void RecvStop( );

	// Data recv
	static unsigned __stdcall RecvData( void * pArg );
};

ClientSocket::ClientSocket( )
: ConnectSocket( INVALID_SOCKET )
  , bContinue( true )
{

}

ClientSocket::~ClientSocket( )
{
	Close( );
}

bool ClientSocket::Initialize( )
{
	int iResult;

	iResult = WSAStartup( MAKEWORD(2, 2), & wsaData );
	if ( iResult != 0 )
	{
		cout << "WSAStartup function failed with error : " << GetLastError( ) << endl;
		return false;
	}

	ConnectSocket = socket( AF_INET, SOCK_STREAM, 0 );
	if ( ConnectSocket == INVALID_SOCKET )
	{
		cout << "socket function failed with error : " << GetLastError( ) << endl;
		return false;
	}

	return true;
}

bool ClientSocket::ConnectServer( char * str, int port )
{
	/* IP Connect */
	unsigned long ulAddress = inet_addr( str );

	SOCKADDR_IN sin;
	sin.sin_family				= AF_INET;
	sin.sin_addr.S_un.S_addr	= ulAddress;
	sin.sin_port				= htons( port );


	/* Domain Name connect */
	//hostent * he;
	//if ( (he = gethostbyname( str ) )  == 0 )
	//{
	//	closesocket( s );
	//	return false;
	//}

	//sockaddr_in addr;
	//addr.sin_family = AF_INET;
	//addr.sin_port = htons( port );
	//addr.sin_addr = * ( (in_addr *) he->h_addr );
	//memset( & ( addr.sin_zero ), 0, 8 );

	if ( connect( ConnectSocket, (sockaddr *) & sin, sizeof( SOCKADDR_IN ) ) )
	{
		closesocket( ConnectSocket );
		return false;
	}

	cout << "connect success!" << endl;
	return true;
}

void ClientSocket::Close( )
{
	if ( ConnectSocket != INVALID_SOCKET )
		closesocket( ConnectSocket );

	WSACleanup( );
}

void ClientSocket::RecvStart( )
{
	hRecvThread = (HANDLE) _beginthreadex( NULL, 0, RecvData, this, 0, & RecvThreadID );
}

void ClientSocket::RecvStop( )
{
	bContinue = false;
}


unsigned __stdcall ClientSocket::RecvData( void * pArg )
{
	ClientSocket * c = (ClientSocket *) pArg;

	string ret;
	while ( c->bContinue )
	{
		while ( true )  
		{
			char r;
			if ( recv( c->ConnectSocket, &r, 1, 0 ) <= 0 ) 
				break;

			ret += r;

			if ( r == '\n' )
			{
				// 서버 종료시 클라이언트 종료
				if ( ! ret.compare( "exit") )
				{
					closesocket( c->ConnectSocket );
					c->ConnectSocket = INVALID_SOCKET;
					c->bContinue = false;
					break;
				}

				if ( ! ret.compare( "file" ) )
				{
					// 파일명 받기
					string fileName;

					// 파일 이름 받아오기
					while ( true )  
					{
						char r;
						if ( recv( c->ConnectSocket, &r, 1, 0 ) <= 0  ) 
							continue;

						fileName += r;

						if ( r == '\n' )
							break;
					}

					// 파일 사이즈 전달 받기
					string fileSize;

					// 파일 사이즈 받아오기
					while ( true )  
					{
						char r;
						if ( recv( c->ConnectSocket, &r, 1, 0 ) <= 0  ) 
							continue;

						fileSize += r;

						if ( r == '\n' )
							break;
					}

					// 파일 내용 전달 받기
					int size = atol( fileSize.c_str( ) );
					char * fileData = new char[ size ];
					int recvFileData = recv( c->ConnectSocket, fileData, size, 0 );

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
				}

				cout << ret;
				break;
			}
		}
		
		ret.clear( );
	}

	return 0;
}


bool ClientSocket::SendData( const char * data )
{
	if ( ConnectSocket == INVALID_SOCKET )
		return false;

	send( ConnectSocket, data, strlen( data ), 0 );
	return true;
}

bool ClientSocket::SendFile( const char * data, int size )
{
	if ( ConnectSocket == INVALID_SOCKET )
		return false;

	send( ConnectSocket, data, size, 0 );
	return true;
}