#ifndef EDGEKEYED_H_
#define EDGEKEYED_H_

#include<iostream>
using std::cout;
using std::cin;

#include "edge.h"

class Node;

class EdgeKeyed : public Edge
{
	int keyValue;

	public:
		EdgeKeyed( void );
		EdgeKeyed( Node*, Node*, int );
		int getKey( void ) const;
		void setKey( int );
		void print( void ) const;
};

struct lessThanKeyed
{
	bool operator()( const EdgeKeyed* base, const EdgeKeyed* compare ) const
	{
		return( base->getKey() < compare->getKey() );
	}
};
#endif //EDGEKEYED_H_
