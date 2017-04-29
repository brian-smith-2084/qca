#ifndef QUICKSORT_H_
#define QUICKSORT_H_

#include <vector>
using std::vector;

template<class T> int partition( vector<T*>& vOfTs, int low, int high )
{
	int pivot = low;
	T* pivotT = vOfTs[ low ];
	vector<T*> lowTs;
	vector<T*> highTs;
	for( int i = low + 1; i <= high ; i++ )
	{
		if( *vOfTs[i] < *pivotT )
			lowTs.push_back( vOfTs[i] );
		else
			highTs.push_back( vOfTs[i] );
	}
	int i = low;
	for( int j = 0; j < lowTs.size(); j++ )
	{
			vOfTs[ i ] = lowTs[ j ];
			i++;
	}
	vOfTs[ i ] = pivotT;
	pivot = i;
	i++;
	for( int j = 0; j < highTs.size(); j++ )
	{
		vOfTs[ i ] = highTs[ j ];
		i++;
	}
	return pivot;
}

template<class T> void quickSort( vector<T*>& nodes, int low, int high )
{
	int pivot;
	if( low < high )
	{
		pivot = partition( nodes, low, high );
		quickSort( nodes, low, pivot - 1 );
		quickSort( nodes, pivot + 1, high );
	}
}

template<class T> void quickSort( vector<T*>& vOfTs )
{
	int low = 0;
	int high = vOfTs.size() - 1;
	quickSort( vOfTs, low, high );
}

#endif //QUICKSORT_H_
