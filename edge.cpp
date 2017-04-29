/*  File:    	edge.cpp 
 *
 *  Class:		Edge 
 *
 *  Purpose:    Edge class to represent an edge, with tail Vertex, head
 *              Vertex, and weight. 
 *
 *  Author:              Brian S. Smith
 *  Date:                03/23/2004
 *
 */
#include "edge.h"
#include "node.h"

//#define DEBUG true	//DEBUG definition constnat

/*
 * Edge::Edge( void )
 * 
 * Default constructor.
 */
Edge::Edge( void )
{
	tailNode = 0;
	headNode = 0;
	iWeight = 0;
}

/*
 * Edge::Edge( Vertex* tail, Vertex* head, int weight )
 * 
 * Constructor, uses arguments to define data.
 */
Edge::Edge( Node* tail, Node* head, int weight )
{
	tailNode = tail;
	headNode = head;
	iWeight = weight;
}

/*
 * type Edge::getX( void ) const
 *
 * The get functions return the appropriate data.
 */
Node* Edge::getTail( void ) const
{
	return tailNode;
}

Node* Edge::getHead( void ) const
{
	return headNode;
}

int Edge::getWeight( void ) const
{
	return iWeight;
}

/*
 * void Edge::setX( type )
 * 
 * The set functions modify the appropriate data.
 */
void Edge::setTail( Node* tail )
{
	tailNode = tail;
}

void Edge::setHead( Node* head )
{
	headNode = head;
}

void Edge::setWeight( int weight )
{
	iWeight = weight;
}

/*
 * void Edge::print( void ) const
 * 
 * Prints out the information of the Edge on the screen.
 */
void Edge::print( void ) const
{
	cout << "I am an edge.";
	if( tailNode != 0 )
		cout << ", Tail: " << tailNode->getID();
	if( headNode != 0 )
		cout << " Head: " << headNode->getID();
	cout << ", Weight: " << iWeight << endl;
}

#ifdef DEBUG
	/*
	 * int main( void )
  	 *
	 * DEBUG main, used only if DEBUG is defined for debugging.
 	 */
	int main( void )
	{
		Node testV;
		testV.setID( 1 );
		Edge edge;
		edge.setHead( &testV );
		edge.setWeight( 7 );
		edge.print();
	}
#endif

