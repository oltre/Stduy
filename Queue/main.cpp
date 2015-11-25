#include <iostream>

#include "Stack.h"
#include "Person.h"
#include "MyArray.h"
#include "Queue.h"

using namespace std;

template < typename T >
void Swap( T & n, T & m )
{
	T temp;
	temp = n;
	n = m;
	m = temp;
}

int main ( )
{
	//Stack<int, 10> mydata;
	//int n;

	//for ( int i = 0; i < 10; i++ )
	//{
	//	if ( mydata.push( i * 10 ) )
	//		cout << "i : " << i << ", data : " << i * 10 << ", Pushed!!" << endl;
	//}

	//for ( int i = 0; i < 10; i++ )
	//{
	//	if ( mydata.pop( n ) )
	//		cout << "i : " << i << ", data : " << n << ", Pop!!" << endl;
	//}

	//int a = 10;
	//int b = 20;
	//Swap<int> ( a, b );
	//cout << a << ", " << b << endl;

	//double dba = 10.4;
	//double dbb = 20;

	//Swap<double> (dba, dbb);
	//cout << dba << ", " << dbb << endl;

	//Person p1( "hong", "123", 16 );
	//Person p2( "asdf", "234", 24 );

	//p1.print();
	//p2.print();

	//Swap<Person> (p1, p2);

	//p1.print();
	//p2.print();

	//system( "cls" );
	//MyArray<int, 100> obj;
	//obj.Add(10); 
	//obj.Add(20); 
	//obj.Add(30); 
	//obj.Add(40);
	//obj.Print();

	//system( "cls" );
	Queue< int, 10 > o;

	int val;
	for ( int i = 0; i < 100; i ++ )
	{
		if ( i % 5 )
		{
			if ( o.Enqueue( i ) )
				cout << "Enqueue >> " << i << endl;
			else
				cout << "Enqueue Full!" << endl;				
		}
		else
		{
			if ( o.Dequeue( val ) )
				cout << "Dequeue >> " << val << endl;
		}
	}

	while ( true )
	{
		if ( o.Dequeue( val ) )
			cout << "Dequeue >> " << val << endl;
		else
		{
			cout << "Dequeue empty!" << endl;
			break;
		}
	}

	return 0;
}
