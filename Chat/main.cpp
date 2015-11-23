#include <iostream>
#include <fstream>
#include <string>

#include "MyDB.h"
#include "ServerSocket.h"
#include "ClientSocket.h"

using namespace std;


#define SERVER_MOTION	1
#define CLIENT_MOTION	2

int main( )
{
	ServerSocket sSocket;
	ClientSocket cSocket;


	char inputID[ 20 ];
	char inputPW[ 20 ];
	
	string chatData;
	string IDandData;
	string filePath;


	int inputCnt = 0;
	int iSelect;


	bool bLoginState = false;
	bool bContinue = true;


	//while ( inputCnt < 3 )
	//{
		memset( inputID, 0, sizeof( inputID ) );
		memset( inputPW, 0, sizeof( inputPW ) );

		cout << "Input ID : ";
		cin >> inputID;

		cout << "Input PW : ";
		cin >> inputPW;

		inputCnt ++;
	//}

	//if ( ! bLoginState )
	//{
	//	cout << "## Login Failed!!! ##" << endl;
	//	return 0;
	//}


	cout << endl << endl << "1. Make chatting room..." << endl;
	cout << "2. Join the chatting room... " << endl;
	cin >> iSelect;

	/**
	 * Make Chatting room
	 **/

	if ( iSelect == SERVER_MOTION )
	{
		if ( ! sSocket.Initialize( ) )
			return 0;
		
		if ( ! sSocket.BindSocket( ) )
			return 0;
		
		if ( ! sSocket.ListenServer( ) )
			return 0;
			
		// Accept Thread Start...
		sSocket.AcceptStart( );

		// Recv Thread Start...
		sSocket.RecvStart( );
	}
	else if ( iSelect == CLIENT_MOTION )
	{
		if ( ! cSocket.Initialize( ) )
			return 0;

		if ( ! cSocket.ConnectServer( "127.0.0.1", 50000 ) )
			return 0;

		// ID ���� ����.
		cSocket.SendData( inputID );

		// Recv Thread Start;
		cSocket.RecvStart( );
	}


	while ( bContinue )
	{
		IDandData.clear( );
		getline( cin, chatData );

		// ���� ��ȣ
		if ( (! chatData.compare( "Exit" ) )
			 || (! chatData.compare( "exit") )
			 || (! chatData.compare( "EXIT") ) )
		{
			chatData.append( "\n" );
			if ( iSelect == SERVER_MOTION )
			{
				sSocket.SendData( chatData.c_str( ), -1 );

				sSocket.AcceptStop( );
				sSocket.RecvStop( );
			}

			if ( iSelect == CLIENT_MOTION )
			{
				cSocket.SendData( chatData.c_str( ) );
				cSocket.RecvStop( );
			}

			break;
		}

		// ���� ����
		if ( (! chatData.compare( "file" ) )
			|| (! chatData.compare( "File") )
			|| (! chatData.compare( "FILE") ) )
		{
			chatData.append( "\n" );
			// ������ ���� ��� �Է�
			if ( iSelect == SERVER_MOTION )
				sSocket.SendData( chatData.c_str( ), -1 );
			else if ( iSelect == CLIENT_MOTION )
				cSocket.SendData( chatData.c_str( ) );


			cout << "File path >> ";
			getline( cin, filePath );

			// ���� ����
			fstream file;
			file.open( filePath.c_str( ), ios::in | ios::binary );

			// ������ ���ϸ�����
			char FileName[MAX_PATH];
			char FileExt[10];

			_splitpath( filePath.c_str( ), NULL, NULL, FileName, FileExt );

			filePath.clear( );
			filePath = FileName;
			filePath += FileExt;

			// ���ϸ� ����
			filePath.append( "\n" );
			if ( iSelect == SERVER_MOTION )
				sSocket.SendData( filePath.c_str( ), -1 );
			else if ( iSelect == CLIENT_MOTION )
				cSocket.SendData( filePath.c_str( ) );

			string fileData;
			char ch;
			while ( ! file.eof( ) )
			{
				file.get( ch );
				fileData += ch;
			}

			long begin = file.tellg();
			file.seekg (0, ios::end);
			long end = file.tellg();
			file.close();
			
			long fileSize = end - begin;
			char buffer[1024];
			memset( buffer, 0, sizeof( buffer ) );
			itoa( fileSize, buffer, 10 );
			strcat( buffer, "\n" );

			// ���� ������ ����
			if ( iSelect == SERVER_MOTION )
				sSocket.SendData( buffer, -1 );
			else if ( iSelect == CLIENT_MOTION )
				cSocket.SendData( buffer );

			// ���� ������ ����
			fileData.append( "\n" );
			if ( iSelect == SERVER_MOTION )
				sSocket.SendFile( fileData.c_str( ), fileSize, -1 );
			else if ( iSelect == CLIENT_MOTION )
				cSocket.SendFile( fileData.c_str( ), fileSize );
		}

		// ��� �ִ� �����̸� �Ѿ��
		if ( ! chatData.compare( "" ) )
			continue;

		IDandData = inputID;
		IDandData.append( " : " );
		IDandData.append( chatData );
		IDandData.append( "\n" );

		if ( iSelect == SERVER_MOTION )
			sSocket.SendData( IDandData.c_str( ), -1 );
		else if ( iSelect == CLIENT_MOTION )
			cSocket.SendData( IDandData.c_str( ) );
	}

	return 0;
}