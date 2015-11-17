//#include "MyStack.h"

#include "define.h"

template <typename T>
class MyStack
{
private :
	T * arVector;
	int iVectorIdx;
	int iStackSize;

public :
	MyStack( void );
	MyStack( int nStackSize );
	~MyStack( void );

	bool Push( T chData );
	T Pop( );

	bool IsEmpty( );
	bool Clear( );
	T GetTop( );
};

template <typename T>
MyStack<T>::MyStack( void )
{
	arVector   = new T[ DEFAULT_STACKSIZE * sizeof( T ) ];
	memset( arVector, '\0', DEFAULT_STACKSIZE * sizeof( T ) );

	iVectorIdx = 0;
	iStackSize = DEFAULT_STACKSIZE;
}

template <typename T>
MyStack<T>::MyStack( int nStackSIze )
{
	arVector   = new T[ nStackSIze * sizeof( T ) ];
	memset( arVector, '\0', nStackSIze * sizeof( T ) );

	iVectorIdx = 0;
	iStackSize = nStackSIze;
}

template <typename T>
MyStack<T>::~MyStack( void )
{
	if ( iStackSize )
		delete [] arVector;
}

template <typename T>
bool MyStack<T>::Push( T chData )
{
	// Vector Resize
	if ( iVectorIdx > ((iStackSize / 5) * 4) )
	{
		int tempSize = iStackSize;
		T * temp;
		temp = new T[ iStackSize * sizeof( T ) ];

		memcpy( temp, arVector, iStackSize * sizeof( T ) );
		//strcpy_s( temp, iStackSize * sizeof( T ), arVector );

		delete [] arVector;

		iStackSize = iStackSize * 2;
		arVector = new T[ iStackSize * sizeof( T ) ];

		memcpy( arVector, temp, tempSize );
		//strcpy_s( arVector, tempSize, temp );
	}

	arVector[ ++ iVectorIdx ] = chData;

	return true;
}

template <typename T>
T MyStack<T>::Pop( )
{
	if ( iVectorIdx > 0 )
		return arVector[ iVectorIdx -- ];
	else
		return NULL;
}

template <typename T>
bool MyStack<T>::IsEmpty( )
{
	if ( iVectorIdx )
		return false;
	else
		return true;
}

template <typename T>
bool MyStack<T>::Clear(  )
{
	delete [] arVector;

	arVector   = new T[ DEFAULT_STACKSIZE * sizeof( T ) ];
	memset( arVector, '\0', DEFAULT_STACKSIZE * sizeof( T ) );

	iVectorIdx = 0;
	iStackSize = DEFAULT_STACKSIZE;

	return true;
}

template <typename T>
T MyStack<T>::GetTop( )
{
	return arVector[ iVectorIdx ];
}

int main( )
{
	int InputDataCnt = 0;
	int InFixSize = DEFAULT_STACKSIZE;
	char * arInFix = new char[ InFixSize * sizeof( char ) ];
	char * arPostFix;
	memset( arInFix, '\0', InFixSize );

	char chInput;
	MyStack<char> chStack;
	MyStack<int>  intStack;

	while ( strcmp( arInFix, "q" ) ) 
	{
		//////////////////////////////////////////////////////////////////
		// Input Infix;
		//

		InputDataCnt = 0;
		memset( arInFix, '\0', InFixSize );

		cout << "Input InFIx formula(Quit:q) >> ";
		while ( true )
		{
			chInput = getchar( );

			// 엔터키 입력시 루프 종료
			if ( (chInput == 0x0A) || ( chInput == 0x0A) )
				break;

			arInFix[ InputDataCnt ++ ] = chInput;

			// 배열 크기 재정의
			if ( InputDataCnt > ((InFixSize / 3) * 2) )
			{
				int tempSize = InFixSize;
				char * temp;
				temp = new char[ InFixSize * sizeof( char ) ];

				strcpy_s( temp, InFixSize * sizeof( char ), arInFix );

				delete [] arInFix;

				InFixSize = InFixSize * 2;
				arInFix = new char[ InFixSize * sizeof( char ) ];

				strcpy_s( arInFix, tempSize, temp );
			}
		}
		//////////////////////////////////////////////////////////////////

		//////////////////////////////////////////////////////////////////
		// Make Post Fix;;
		//

		int i, PostFixSize;
		arPostFix = new char[ InFixSize * sizeof( char ) ];
		memset( arPostFix, '\0', InFixSize );

		bool bBeforeBracket = false;
		bool bBeforeMakePostFix = false;

		cout << "MakePostFix >> ";

		for ( i = 0, PostFixSize = 0; i < InputDataCnt; i ++ )
		{
			if ( (arInFix[ i ] >= '0') && (arInFix[ i ] <= '9') )
			{
				if ( bBeforeMakePostFix )
				{
					arPostFix[ PostFixSize ++ ] = ' ';
					bBeforeMakePostFix = false;
				}
				arPostFix[ PostFixSize ++ ] = arInFix[ i ];
			}
			else
			{
				bBeforeMakePostFix = true;
				switch ( arInFix[ i ] )
				{
				case '^':
					// 우선 순위가 제일 높기 때문에 항상 푸쉬
					chStack.Push( arInFix[ i ] );
					break;

				case '*':
				case '/':
					// 스택이 비어 있으면 푸쉬
					if ( chStack.IsEmpty( ) )
						chStack.Push( arInFix[ i ] );

					// 그렇지 않다면
					else
					{
						while ( (chStack.GetTop( ) == '^') 
							|| (chStack.GetTop( ) == '*')
							|| (chStack.GetTop( ) == '/') )
						{
							arPostFix[ PostFixSize ++ ] = ' ';
							arPostFix[ PostFixSize ++ ] = chStack.Pop( );
						}

						chStack.Push( arInFix[ i ] );
					}
					break;

				case '+':
				case '-':
					// 스택이 비어 있으면 푸쉬
					if ( chStack.IsEmpty( ) )
						chStack.Push( arInFix[ i ] );

					// 그렇지 않다면
					else
					{
						while ( (chStack.GetTop( ) == '^') 
							|| (chStack.GetTop( ) == '*')
							|| (chStack.GetTop( ) == '/')
							|| (chStack.GetTop( ) == '+')
							|| (chStack.GetTop( ) == '-') )
						{
							arPostFix[ PostFixSize ++ ] = ' ';
							arPostFix[ PostFixSize ++ ] = chStack.Pop( );
						}

						chStack.Push( arInFix[ i ] );
					}
					break;
				case '(':
					bBeforeBracket = true;
					chStack.Push( arInFix[ i ] );
					break;

				case ')':
					if ( ! bBeforeBracket )
						break;

					while ( (chStack.GetTop( ) != '(') )
					{
						arPostFix[ PostFixSize ++ ] = ' ';
						arPostFix[ PostFixSize ++ ] = chStack.Pop( );
					}

					chStack.Pop( );
					break;
				}
			}
		}

		while ( ! chStack.IsEmpty( ) )
		{
			arPostFix[ PostFixSize ++ ] = ' ';
			arPostFix[ PostFixSize ++ ] = chStack.Pop( );
		}

		cout << arPostFix << endl;
		//////////////////////////////////////////////////////////////////

		//////////////////////////////////////////////////////////////////
		// Calc PostFix
		//
		bool bInt = true;

		for ( i = 0; i < PostFixSize; i ++ )
		{
			if ( (arPostFix[ i ] >= '0') && (arPostFix[ i ] <= '9') )
			{
				if ( bInt )
				{
					intStack.Push( atoi( arPostFix + i ) );
					bInt = false;
				}
			}
			else
			{
				int x, y, r = 0;
				bInt = true;

				switch ( arPostFix[ i ] )
				{
				case '^':
					x = intStack.Pop( );
					r = y = intStack.Pop( );

					while ( x > 1 )
					{
						r *= y;
						x--;
					}

					//cout << r << endl;

					intStack.Push( r );
					break;

				case '*':
					x = intStack.Pop( );
					y = intStack.Pop( );

					intStack.Push( x * y );
					break;

				case '/':
					x = intStack.Pop( );
					y = intStack.Pop( );

					intStack.Push( y / x );
					break;

				case '+':
					x = intStack.Pop( );
					y = intStack.Pop( );

					intStack.Push( x + y );
					break;

				case '-':				
					x = intStack.Pop( );
					y = intStack.Pop( );

					intStack.Push( y - x );
				}
			}
		}
		//////////////////////////////////////////////////////////////////

		cout << "result >> " << intStack.Pop( ) << endl;

		chStack.Clear( );
		intStack.Clear( );
	} 

	delete [] arPostFix;
	delete [] arInFix;

	return 0;
}