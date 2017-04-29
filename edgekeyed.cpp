#include "edgekeyed.h"
#include "node.h"

EdgeKeyed::EdgeKeyed( void )
{
}

EdgeKeyed::EdgeKeyed( Node* parent, Node* child, int weight )
	: Edge( parent, child, weight )
{
	keyValue = -1;
}

int EdgeKeyed::getKey( void ) const
{
	return keyValue;
}

void EdgeKeyed::setKey( int key )
{
	keyValue = key;
}

void EdgeKeyed::print( void ) const
{
	cout << "I am an edge.";
	if( tailNode != 0 )
		cout << " Tail: " << tailNode->getID();
	if( headNode != 0 )
		cout << ", Head: " << headNode->getID();
	cout << ", Weight: " << iWeight;
	cout << ", key: " << keyValue << endl;
}
