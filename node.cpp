#include "node.h"

//#define DEBUG true

sortModes Node::sortMode;

Node::Node( void )
{
	bIsSource = true;
	bIsSink = true;
	iU = -1;
	iD = -1;
	bVisited = false;
	iLeftEdge = INT_MAX;
	iRightEdge = -1;
	iTrack = -1;
}

Node::Node( Net* newNet )
{
	newNet->setNode( this );
	vNets.push_back( newNet );
	bIsSource = true;
	bIsSink = true;
	iU = -1;
	iD = -1;
	bVisited = false;
	iLeftEdge = newNet->getLeftEdge();
	iRightEdge = newNet->getRightEdge();
	iTrack = -1;
}

void Node::setSortMode( sortModes newMode )
{
	sortMode = newMode;
}

int Node::getID( void ) const
{
	int ID = -1;
	if ( vNets.size() > 0 )
	{
		ID = vNets[0]->getID();
	}
	return ID;
}

bool Node::operator<( Node compare )
{
	bool isLessThan = false;
	switch( sortMode )
	{
		case LEFTEDGE:
			if( iLeftEdge < compare.getLeftEdge() )
			isLessThan = true;
			break;
		case PARENTS:
			if( vParents.size() < compare.getParentCount() )
				isLessThan = true;
				break;
		default:
			if( vParents.size() < compare.getParentCount() )
				isLessThan = true;
				break;
	}
	return isLessThan; 
}

vector<Net*>& Node::getNets( void )
{
	return vNets;
}

vector<Node*>& Node::getParents( void )
{
	return vParents;
}

int Node::getParentCount( void ) const
{
	return vParents.size();
}

int Node::getChildCount( void ) const
{
	return vChildren.size();
}

vector<Node*>& Node::getChildren( void )
{
	return vChildren;
}

vector<EdgeKeyed*>& Node::getInterParents( void )
{
	return interParents;
}

int Node::getInterParentCount( void ) const
{
	return interParents.size();
}
 
vector<EdgeKeyed*>& Node::getInterChildren( void )
{
	return interChildren;
}

bool Node::isSource( void ) const
{
	return bIsSource;
}

bool Node::isSink( void ) const
{
	return bIsSink;
}

int Node::getU( void ) const
{
	return iU;
}

int Node::getD( void ) const
{
	return iD;
}

bool Node::isVisited( void ) const 
{
	return bVisited;
}

bool Node::isExplored( void ) const
{
	#ifdef DEBUG
		cout << "Checking Node " << getID() << " for explore...\n";
	#endif
	bool explored = true;
	for( int i = 0; (i < vChildren.size( )) && (explored); i++ )
	{
		if( vChildren[i]->isVisited() == false )
			explored = false;
	}
	#ifdef DEBUG
		if( explored )
			cout << "Node " << vChildren[i]->getID() << " is visited\n";
		else
		{
			cout << "Node " << vChildren[i]->getID() << " is NOT visited. ";
			cout << "Node " << getID() << "is not explored\n";
		}
	#endif
	return explored;
}
	

int Node::getLeftEdge( void ) const
{
	int leftEdge = INT_MAX;
	for( int i = 0; i < vNets.size(); i++ )
	{
		if( vNets[i]->getLeftEdge() < leftEdge )
		{
			leftEdge = vNets[i]->getLeftEdge();
		}
	}
	return leftEdge;
}

int Node::getRightEdge( void ) const
{
	int rightEdge = -1;
	for( int i = 0; i < vNets.size(); i++ )
	{
		if( vNets[i]->getRightEdge() > rightEdge )
		{
			rightEdge = vNets[i]->getRightEdge();
		}
	}
	return rightEdge;
}

int Node::getTrack( void ) const
{
	return iTrack;
}

int Node::getContactCount( void ) const
{
	int contacts = 0;
	for( int i = 0; i < vNets.size(); i++ )
	{
		Net* net = vNets[i];
		contacts = contacts + net->getTopContactCount();
		contacts = contacts + net->getBottomContactCount();
	}
	return contacts;
}

bool Node::hasParent( const Node* parent )
{
	bool found = false;
	vector<Node*>::const_iterator search = 
		find( vParents.begin(), vParents.end(), parent );
	if( search != vParents.end() )
		found = true;
	return found;
}

bool Node::hasInterParent( Node* interParent )
{
	bool found = false;
	for( vector<EdgeKeyed*>::iterator it = interParents.begin();
		 (it < interParents.end( )) && (!found); it++ 		)
	{
		Edge* currentEdge = (*it);
		Node* currentParent = currentEdge->getTail();
		if( currentParent == interParent )
			found = true;
	}
	return found;
}
	

bool Node::hasChild( Node* child ) const
{
	bool found = false;
	if( find( vChildren.begin(), vChildren.end(), child) != vChildren.end() )
		found = true;
	return found;
}

bool Node::hasInterChild( Node* interChild )
{
	bool found = false;
	for( vector<EdgeKeyed*>::iterator it = interChildren.begin();
		 (it < interChildren.end( )) && (!found); it++      )
	{
		Edge* currentEdge = *it;
		Node* currentChild = currentEdge->getHead();
		if( currentChild == interChild )
			found = true;
	}
	return found;
}
	

bool Node::isInPathParent( vector<Node*>& nodes )
{
	#ifdef DEBUG
		cout << "In method isInPathParent at node ";
		cout << getNets()[0]->getID() << endl;
	#endif
	bool isInPath = false;
	for( int i = 0; ( i < vParents.size() ) && ( !isInPath ); i++ )
	{
		for( int j = 0; ( j < nodes.size() ) && ( !isInPath ); j++ )
		{
			#ifdef DEBUG
				cout << "comparing nodes ";
				cout << vParents[i]->getNets()[0]->getID();
				cout << " and ";
				cout << nodes[j]->getNets()[0]->getID() << endl;
			#endif
			if( nodes[j] == vParents[i] )
			{
				isInPath = true;
				#ifdef DEBUG
					cout << "Parent is ";
					cout << vParents[i]->getNets()[0]->getID();
					cout << endl;	
				 	cout << "Node " << getNets()[0]->getID();
					cout << " is in path with node ";
					cout << nodes[j]->getNets()[0]->getID() << endl;
				#endif
			}
		}
		if( isInPath == false )
			isInPath = vParents[i]->isInPathParent( nodes );
	}
	return isInPath;
}

bool Node::isInPathParent( Node* node) 
{
	#ifdef DEBUG
		cout << "In isInPathParent of " << getID() << endl;
		cout << "searching for " << node->getID() << endl;
		print();
	#endif
	bool isInPath = false;
	for( int i = 0; (i < vParents.size()) && (!isInPath); i++ ) 
	{
		if( node == vParents[i] )
		{
			isInPath = true;
			#ifdef DEBUG
				cout << "node found." << endl;
			#endif
		}
		else
		{
			#ifdef DEBUG
				cout << "vparents[i] (next parent)  = ";
				cout << vParents[i]->getID() << endl;
			#endif
			isInPath = vParents[i]->isInPathParent( node );
		}
	}
	return isInPath;
}


bool Node::isInPathChild( vector<Node*>& nodes )
{
	#ifdef DEBUG
		cout << "In method isInPathChild at node ";
		cout << getNets()[0]->getID() << endl;
	#endif
	bool isInPath = false;
	for( int i = 0; ( i < vChildren.size() ) && ( !isInPath ); i++ )
	{
		for( int j = 0; ( j < nodes.size() ) && ( !isInPath ); j++ )
		{
			#ifdef DEBUG
				cout << "comparing nodes ";
				cout << vChildren[i]->getNets()[0]->getID();
				cout << " and ";
				cout << nodes[j]->getNets()[0]->getID() << endl;
			#endif
			if( nodes[j] == vChildren[i] )
			{
				isInPath = true;
				#ifdef DEBUG
					cout << "Child is ";
					cout << vChildren[i]->getNets()[0]->getID();
					cout << endl;	
					cout << "Node " << getNets()[0]->getID();
					cout << " is in path with node ";
					cout << nodes[j]->getNets()[0]->getID() << endl;
				#endif
			}
		}
		if( isInPath == false )
			isInPath = vChildren[i]->isInPathChild( nodes );
	}
	return isInPath;
}

bool Node::isInPathChild( Node* node )
{
	bool isInPath = false;
	for( int i = 0; (i < vChildren.size()) && (!isInPath); i++ )
	{
		if( node == vChildren[i] )
		{
			isInPath = true;
		}
		else
		{
			isInPath = vChildren[i]->isInPathChild( node );
		}
	}
	return isInPath;
}

bool Node::isInPath( vector<Node*>& nodes )
{
	bool isInPath = isInPathParent( nodes );
	if( !isInPath )
		isInPath = isInPathChild( nodes );
	return isInPath;
}

bool Node::isInPath( Node* node )
{
	/*bool isInPath = isInPathParent( node );
	if( !isInPath )
		isInPath = isInPathChild( node ); */
	bool isInPath = isDescendant( node, this );
	if ( !isInPath )
		isInPath = isAncestor( node, this );
	return isInPath;
}

bool Node::hasOverlap( Node* node ) const
{
	bool overlap = false;
	int leftEdge = node->getLeftEdge();
	int rightEdge = node->getRightEdge();
	if( (iLeftEdge < rightEdge) && (leftEdge < iRightEdge) )
		overlap = true;
	return overlap;
}

void Node::checkEdges( Net* net )
{
	int newLeft = net->getLeftEdge();
	int newRight = net->getRightEdge();
	if( newLeft < iLeftEdge )
		iLeftEdge = newLeft;
	if( newRight > iRightEdge )
		iRightEdge = newRight;
}

void Node::checkEdge( int column )
{
	if( column < iLeftEdge )
		iLeftEdge = column;
	if( column > iRightEdge )
		iRightEdge = column;
}
	
void Node::addNet( Net* newNet )
{
	vNets.push_back( newNet );
	newNet->setNode( this );
	checkEdges( newNet );
}

void Node::addNets( vector<Net*>& newNets )
{
	for( int i = 0; i < newNets.size(); i++ )
	{
		#ifdef DEBUG
			cout << "adding net " << newNets[i]->getID() << " to node ";
			cout << getNets()[0]->getID() << endl;
		#endif
		this->addNet( newNets[ i ] );
	}
}

void Node::addParent( Node* parent )
{
	if( !hasParent(parent) )
	{
		vParents.push_back( parent );
		bIsSource = false;
	}
}

void Node::removeParent( const Node* parent )
{
	vParents.erase( remove(vParents.begin(), vParents.end(), parent), 
					vParents.end() );
	if( vParents.empty() )
		bIsSource = true;
}

void Node::exchangeParent( Node* oldParent, Node* newParent )
{
	vParents.erase( remove(vParents.begin( ), vParents.end( ), oldParent), 
	                vParents.end() );
	addParent( newParent );
}

void Node::addParents( vector<Node*>& parents )
{
	for( int i = 0; i < parents.size(); i++ )
		addParent( parents[i] );
}

void Node::addPseudoParent( Node* pseudo )
{
	vParents.push_back( pseudo );
}

void Node::addChild( Node* child )
{
	if( !hasChild(child) )
	{
		vChildren.push_back( child );
		bIsSink = false;
	}
}

void Node::removeChild( const Node* child )
{
	vChildren.erase( remove(vChildren.begin( ), vChildren.end( ), child), 
					 vChildren.end( ) ); 
	if( vChildren.empty() )
		bIsSink = true;
}

void Node::exchangeChild( Node* oldChild, Node* newChild )
{
	vChildren.erase( remove(vChildren.begin( ), vChildren.end( ), oldChild), 
					 vChildren.end() );
	addChild( newChild );
} 

void Node::addChildren( vector<Node*>& children )
{
	for( int i = 0; i < children.size(); i++ )
	{
		addChild( children[i] );
	}
}

void Node::addPseudoChild( Node* pseudo )
{
	vChildren.push_back( pseudo );
}

void Node::addInterParent( EdgeKeyed* edge )
{
	interParents.push_back( edge );
}

void Node::addInterChild( EdgeKeyed* edge )
{
	interChildren.push_back( edge );
}

void Node::removeInterParent( Node* parent )
{
	for( vector<EdgeKeyed*>::iterator it = interParents.begin( );
		 it < interParents.end( ); it++ 				    )
	{
		Edge* edge = *it;
		Node* currentParent = edge->getTail();
		if( parent == currentParent )
			interParents.erase( it );
	}
}

void Node::removeInterParent( EdgeKeyed* edge )
{
	interParents.erase( remove(interParents.begin(), interParents.end(), 
						edge), interParents.end() );
}
	

void Node::processInterEdges( void )
{
	for( vector<EdgeKeyed*>::iterator it = interParents.begin();
		 it < interParents.end(); it++						)
	{
		addParent( (*it)->getTail() );
	}
	interParents.clear();
	for( vector<EdgeKeyed*>::iterator it = interChildren.begin();
		 it < interChildren.end(); it++						)
	{
		addChild( (*it)->getHead() );
	}
	interChildren.clear();
} 

void Node::removeInterChild( Node* child )
{
	for( vector<EdgeKeyed*>::iterator it = interChildren.begin( );
		 it < interChildren.end( ); it++    			     )
	{
		Edge* edge = *it;
		Node* currentChild = edge->getHead();
		if( child == currentChild )
			interChildren.erase( it );
	}
}

void Node::removeInterChild( EdgeKeyed* edge )
{
	#ifdef DEBUG
		cout << "Removing interchild " << edge->getTail()->getID() << "->";
		cout << edge->getHead()->getID() << " from interChildren in ";
		cout << getID() << "..." << endl;
	#endif
	interChildren.erase( remove(interChildren.begin(), interChildren.end(), 
						edge), interChildren.end() );
	#ifdef DEBUG
		cout << "After interchild removal:" << endl;
		print();
	#endif
}

void Node::setSource( bool isSource )
{
	bIsSource = isSource;
}

void Node::setSink( bool isSink )
{
	bIsSink = isSink;
}

void Node::setU( int u )
{
	iU = u;
}

void Node::updateU( int u )
{
	if( u > iU )
		iU = u;
}

void Node::updateD( int d )
{
	if( d > iD )
		iD = d;
}

void Node::setD( int d )
{
	iD = d;
}

void Node::setVisited( bool visited )
{
	bVisited = visited;
}

void Node::setTrack( int track )
{
	iTrack = track;
	for( vector<Net*>::iterator it = vNets.begin(); it < vNets.end(); it++ )
	{
		(*it)->setTrack( track );
	} 
}

void Node::print( void ) const
{
	cout << "I am node " << getID() << ". My U is " << iU;
	cout << " and my D is " << iD << ".\n";
	if( bIsSource == true )
		cout << "I am a source.\n";
	if( bIsSink == true )
		cout << "I am a sink.\n";
	cout << "My left edge is " << getLeftEdge() << " and my right edge is ";
	cout << getRightEdge() << endl;
	cout << "I belong on track " << iTrack << ".\n";
	cout << "I contain these nets:\n ";
	for( int i = 0; i < vNets.size(); i++ )
	{
		vNets[i]->print();
	}
	cout << "I am to be placed below these nodes:\n";
	for( int i = 0; i < vParents.size(); i++ )
	{
		vector<Net*> parentNets = vParents[i]->getNets();
		cout << "Node " << parentNets[0]->getID();
		cout << parentNets[0]->getSubID()<< endl;
		cout << "        Nets ";
		for( int j = 0; j < parentNets.size(); j++ )
		{
			cout << parentNets[j]->getID() << " ";
		}	
		cout << endl;
	}
	cout << endl;
	cout << "I am to be placed above these nodes:\n";
	for( int i = 0; i < vChildren.size(); i++ )
	{
		vector<Net*> childNets = vChildren[i]->getNets();
		cout << "Node " << childNets[0]->getID();
		cout << childNets[0]->getSubID() << endl;
		cout << "        Nets ";
		for( int j = 0; j < childNets.size(); j++ )
		{
			cout << childNets[j]->getID() << " ";
		}	
		cout << endl;
	}
	cout << endl;
	cout << "These are my intersection parents:\n";
	for( int i =0; i < interParents.size(); i++ )
	{
		cout << interParents[i]->getTail()->getID() << " ";
	}
	cout << endl;
	cout << "These are my intersection children:\n";
	for( int i = 0; i < interChildren.size(); i++ )
	{
		cout << interChildren[i]->getHead()->getID() << " ";
	}
	cout << endl << endl;
}

bool isDescendant( const Node* target, Node* node )
{
	#ifdef DEBUG
		cout << "Entering isDescendant..." << endl;
		cout << "Searching node " << node->getID() << " for target ";
		cout << target->getID() << endl;
	#endif
	node->setVisited( true );
	bool descendant = false;
	if( node == target )
		descendant = true;
	else
	{
		vector<Node*> children = node->getChildren();
		for( vector<Node*>::iterator it = children.begin(); 
		 	 (it < children.end( )) && (!descendant); it++ )
		{
			if( !(*it)->isVisited() )
			{
				descendant = isDescendant( target, *it );
			}
		}
	}
	return descendant;
}	
	
bool isAncestor( const Node* target, Node* node )
{
	#ifdef DEBUG
		cout << "Entering isAncestor..." << endl;
	#endif
	node->setVisited( true );
	bool ancestor = false;
	if( node == target )
	{
		ancestor = true;
	}
	else
	{
		vector<Node*> parents = node->getParents();
		for( vector<Node*>::iterator it = parents.begin(); 
			 (it < parents.end( )) && (!ancestor); it++ )
		{
			if( !(*it)->isVisited() )
			{
				ancestor = isAncestor( target, *it );
			}
		}
	}
	return ancestor;
}

bool nodesIncludeNode( const vector<Node*>& nodes, const Node* test )
{
	bool included = false;
	for( int i = 0; (i < nodes.size( )) && (!included); i++ )
	{
		if( nodes[i] == test )
			included = true;
	}
	return included;
}

bool nodesIncludeNodes( const vector<Node*>& nodes, const vector<Node*>& test )
{
	bool includes = true;
	for( int i = 0; (i < test.size()) && (includes); i++ )
	{
		includes = nodesIncludeNode( nodes, test[i] );
	}
	return includes;
}
	
/* 
 * Debugging main
 */
/*
int main( void )
{
	Node* n1 = new Node( new Net( 1 ) );
	Node* n2 = new Node( new Net( 2 ) );
	Node* n3 = new Node( new Net( 3 ) );
	n2->addParent( n1 );
	n2->addChild( n3 );
	n1->print();
	n2->print();
	n3->print();
}
*/
