#pragma once

template < typename T, int arrSize >
class Queue
{
private :
	T * arr;
	int size;
	int num;
	int head;
	int tail;

	bool is_empty( );
	bool is_full( );

public :
	Queue( );
	~Queue( );

	bool Enqueue( const T & val );
	bool Dequeue( T & val );
};

template < typename T, int arrSize >
Queue< T, arrSize >::Queue( )
: size( arrSize )
{
	arr = new T[ size * sizeof( T ) ];

	head = 0;
	tail = 0;
	num = 0;
}

template < typename T, int arrSize >
Queue< T, arrSize >::~Queue( )
{
	if ( arr != NULL )
		delete [] arr;
}

template < typename T, int arrSize >
bool Queue< T, arrSize >::is_empty( )
{
	if ( num == 0 )
		return true;
	else
		return false;
}

template < typename T, int arrSize >
bool Queue< T, arrSize >::is_full( )
{
	if ( num >= size )
		return true;
	else
		return false;
}

template < typename T, int arrSize >
bool Queue< T, arrSize >::Enqueue( const T & val )
{
	if ( is_full( ) )
		return false;

	arr[ tail ] = val;

	if ( tail == (size - 1) )
		tail = 0;
	else
		tail ++;

	num ++;

	return true;
}

template < typename T, int arrSize >
bool Queue< T, arrSize >::Dequeue( T & val )
{
	if ( is_empty( ) )
		return false;

	val = arr[ head ];

	if ( head == (size - 1 ) )
		head = 0;
	else
		head ++;

	num --;

	return true;
}