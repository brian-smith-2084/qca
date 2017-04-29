/*
 * File: 		edge.h
 * Function:	Header file for Edge class 
 *
 * Purpose:		This class represents an edge in a graph. 
 * 
 * Author: 		Brian S. Smith
 * Date: 		03/23/2004
 */
#ifndef EDGE_H_
#define EDGE_H_

#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

class Node;

class Edge 
{
	protected:
		Node* tailNode;
		Node* headNode;
		int iWeight;

	public:
		Edge( void );
		Edge( Node* tail, Node* head, int );
		Node* getTail( void ) const;
        Node* getHead( void ) const;
        int getWeight( void ) const;
        void setTail( Node* tail );
        void setHead( Node* head );
        void setWeight( int );
        void print( void ) const;
};

struct lessThanEdges
{
	bool operator()( Edge* base, Edge* compare ) const
	{
		return( base->getWeight() < compare->getWeight() );
	}
};
#endif //EDGE_H_
