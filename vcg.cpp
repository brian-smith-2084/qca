#include "vcg.h"
#include "cyclebreaker.h"

//#define DEBUG true

Vcg::Vcg( void )
{
};

Vcg::Vcg( int size )
{
	vNodes.resize( size );
}

void Vcg::clear( void )
{
	vNodes.clear();
	pseudoSource = 0;
	pseudoSink = 0;
}

int Vcg::getSize( void ) const
{
	return vNodes.size();
}

vector<Node*>& Vcg::getNodes( void )
{
	return vNodes;
}

Node* Vcg::getPseudoSource( void )
{
	return pseudoSource;
}

void Vcg::setSize( int size )
{
	vNodes.resize( size );
}

void Vcg::initialize( int size )
{
	vNodes.resize( size );
	for( int i = 0; i < size; i++ )
		vNodes[ i ] = new Node( new Net( i ) );
}

void Vcg::addNode( int node )
{
	if( node >= vNodes.size() )
	{
		int size = vNodes.size();
		vNodes.resize( node + 1 );
		for( int i = size; i < vNodes.size(); i++ )
		{ 
			vNodes[ i ] = new Node( new Net( i ) );
		}
	}
}

void Vcg::addNode( Net* net )
{
	vNodes.push_back( new Node(net) );
}

void Vcg::addNode( Node* node )
{
	vNodes.push_back( node );
}

void Vcg::addEdge( int tail, int head, int col )
{
	addNode( tail );
	addNode( head );
	vector<Net*> tailNets = vNodes[ tail ]->getNets();
	Net* tailNet = tailNets[ 0 ];
	tailNet->addTopContact( col );
	vector<Net*> headNets = vNodes [ head ]->getNets();
	Net* headNet = headNets [ 0 ];
	headNet->addBottomContact( col );
	if( (head != 0) && (tail != 0) )
	{
		vNodes[ tail ]->addChild( vNodes[head] );
		vNodes[ head ]->addParent( vNodes[tail] ); 
	}
}

void Vcg::addEdge( Node* parent, Node* child )
{
			if( (parent->getID() != 0) && (child->getID() != 0) )
			{
				parent->addChild( child );
				child->addParent( parent );
			}
}

void Vcg::exchangeParents( Node* oldChild, Node* newChild )
{
	vector<Node*> parents = oldChild->getParents();
	for( vector<Node*>::iterator it = parents.begin(); 
	     it < parents.end(); it++ )
	{
		(*it)->exchangeChild( oldChild, newChild );
	}
}

void Vcg::exchangeChildren( Node* oldParent, Node* newParent )
{
	vector<Node*> children = oldParent->getChildren();
	for( vector<Node*>::iterator it = children.begin();
		 it < children.end(); it++ )
	{
		(*it)->exchangeChild( oldParent, newParent );
	}
}

void Vcg::exchangeNode( Node* oldNode, Node* newNode )
{
	exchangeParents( oldNode, newNode );
	exchangeChildren( oldNode, newNode );
}

void Vcg::subNetParents( Node* oldChild, Node* newChild )
{
	/*
	#ifdef DEBUG
		cout << "In subNetParents..." << endl;
	#endif */
	vector<Node*> parents = oldChild->getParents();
	for( vector<Node*>::iterator it = parents.begin(); 
		 it < parents.end(); it++ )
	{
		vector<Net*> nets = (*it)->getNets();
		for( vector<Net*>::iterator nit = nets.begin(); 
			 nit < nets.end(); nit++ )
		{
			vector<Net*> newNets = newChild->getNets();
			for( vector<Net*>::iterator nnit = newNets.begin(); 
			     nnit < newNets.end(); nnit++ )
			{
				if( (*nit)->mustBeAbove(*nnit) )
					addEdge( (*it), newChild );
			}
		}
	}
	/* #ifdef DEBUG
		cout << "New Node after subNetParents:" << endl;
		newChild->print();
	#endif */
}

void Vcg::subNetChildren( Node* oldNode, Node* newNode )
{
	/* #ifdef DEBUG
		cout << "In subNetChildren..." << endl;
		cout << "Old Node:" << endl;
		oldNode->print();
		cout << "New Node" << endl;
		newNode->print();
	#endif */
	vector<Node*> children = oldNode->getChildren();
	for( vector<Node*>::iterator it = children.begin(); it < children.end(); it++ )
	{
		/* #ifdef DEBUG
			cout << "Current Child:" << endl;
			(*it)->print();
		#endif */
		vector<Net*> nets = (*it)->getNets();
		for( vector<Net*>::iterator nit = nets.begin(); nit < nets.end(); nit++ )
		{
			/* #ifdef DEBUG
				cout << "Current Net:" << endl;
				(*nit)->print();
			#endif */
			vector<Net*> newNets = newNode->getNets();
			for( vector<Net*>::iterator nnit = newNets.begin(); 
				 nnit < newNets.end(); nnit++ )
			{
				/* #ifdef DEBUG
					cout << "New Net:" << endl;
					(*nnit)->print();
				#endif */
				if( (*nit)->mustBeBelow(*nnit) )
				{
					/* #ifdef DEBUG
						cout << "Adding edge for subnet child..." << endl;
					#endif */
					addEdge( newNode, (*it) );
				}
				/* #ifdef DEBUG
					else
					{
						cout << "No edge added for subnet child" << endl;
					}
				#endif */
			}
		}
	/* #ifdef DEBUG
		cout << "subNetChildren End." << endl;
	#endif */
	}
	/* #ifdef DEBUG
		cout << "New Node after SubNetChildren:" << endl;
		newNode->print();
	#endif */
}

bool Vcg::subNet( Node* node )
{
	bool didSubNet = false;
	Net* net = node->getNets()[0];
	vector<int> topContacts = net->getTopContacts();
	vector<int> bottomContacts = net->getBottomContacts();
	if( topContacts.size() + bottomContacts.size() > CONTACTS_PER_NET )
	{
		#ifdef DEBUG
			cout << "Subnetting Node " << node->getID() << "..." << endl;
		#endif
		int i = 0;
		while( topContacts.size() + bottomContacts.size() 
			   >= CONTACTS_PER_NET ) 
		{
			vector<int> newTopContacts, newBottomContacts;
			for( int j = 0; j < CONTACTS_PER_NET; j++ )
			{
				int top = 1000; 
				if( !topContacts.empty() )		
					top = topContacts.front();
				int bottom = 1000; 
				if( !bottomContacts.empty() )
					bottom = bottomContacts.front();
				if( top < bottom )
				{
					newTopContacts.push_back( top );
					if( j == 0 )
						topContacts.erase( topContacts.begin() );
				}
				else
				{
					newBottomContacts.push_back( bottom );
					if( j == 0 )
						bottomContacts.erase( bottomContacts.begin() );
				}
			}
			#ifdef DEBUG
				cout << "New Top Contacts: ";
				for( vector<int>::iterator it = newTopContacts.begin(); 
					 it < newTopContacts.end(); it++ )
				{
					cout << *it << " ";
				}
				cout << endl;
				cout << "New Bottom Contacts: ";
				for( vector<int>::iterator it = newBottomContacts.begin(); 
					 it < newBottomContacts.end(); it++ )
				{
					cout << *it << " ";
				}
				cout << endl;
			#endif
			Net* newNet = new Net( net->getID()*100 + i++ );
			Node* newNode = new Node( newNet );
			newNet->addTopContacts( newTopContacts );
			newNet->addBottomContacts( newBottomContacts );	
			#ifdef DEBUG
				cout << "New Node:" << endl;
				newNode->print();
			#endif
			subNetParents( node, newNode );
			subNetChildren( node, newNode );
			vNodes.push_back( newNode );
		}
		#ifdef DEBUG
			cout << "Removing node " << node->getID() << endl;
		#endif
		removeNode( node );
		#ifdef DEBUG
			cout << "Node removed." << endl;
		#endif
		didSubNet = true;
	}
	return didSubNet;
}

void Vcg::applySubnetting( void )
{
	int nodeCount = vNodes.size();
	for( int i = 1; i < nodeCount; i++ )
	{
		#ifdef DEBUG
			cout << "Trying to subnet " << vNodes[i]->getID();
			cout << "..." << endl;
		#endif
		bool didSubNet = subNet( vNodes[i] );
		if( didSubNet )
		{
			#ifdef DEBUG
				cout << "Decrementing i in applySubnetting" << endl;
			#endif
			i--;
		}
	}
	#ifdef DEBUG
		cout << "Done with subnets" << endl;
	#endif	
}	


void Vcg::removeEdge( Node* parent, Node* child )
{
	parent->removeChild( child );
	child->removeParent( parent );
}

void Vcg::removeNode( Node* node )
{
	vector<Node*> parents = node->getParents();
	for( vector<Node*>::iterator it = parents.begin(); it < parents.end(); it++ )
	{
		(*it)->removeChild( node );
	}
	vector<Node*> children = node->getChildren();
	for( vector<Node*>::iterator it = children.begin(); it < children.end(); it++ )
	{
		(*it)->removeParent( node );
	}
	vNodes.erase( remove( vNodes.begin(), vNodes.end(), node ), vNodes.end() );
	delete( node );
}

void Vcg::splitNode( Node* node, Net* net1, Net* net2 )
{
	Node* node1 = new Node( net1 );
	Node* node2 = new Node( net2 );
	vector<Node*>& parents = node->getParents();
	for( int i = 0; i < parents.size(); i++ )
	{
		const vector<int>& topContacts = 
									parents[i]->getNets()[0]->getTopContacts();
		for( int j = 0; j < topContacts.size(); j++ )
		{
			if( net1->hasBottomContact(topContacts[j]) )
			{
				node1->addParent( parents[i] );
			}
			if( net2->hasBottomContact(topContacts[j]) )
			{
				node2->addParent( parents[i] );
			}
		}
	}	
	vector<Node*>& children = node->getChildren();
	for( int i = 0; i < children.size(); i++ )
	{
		const vector<int>& bottomContacts = 
								children[i]->getNets()[0]->getBottomContacts();
		for( int j = 0; j < bottomContacts.size(); j++ )
		{
			if( net1->hasTopContact(bottomContacts[j]) )
			{
				node1->addChild( children[i] );
			}
			if( net2->hasTopContact(bottomContacts[j]) )
			{
				node2->addChild( children[i] );
			}
		}
	}
	vNodes.push_back( node1 );
	vNodes.push_back( node2 );
	removeNode( node );
	addPseudoSourceSink();
}

void Vcg::splitNode( Node* target, vector<Node*>& newNodes )
{
	for( int i = 0; i < newNodes.size(); i++ )
	{
		Node* newNode = newNodes[i];
		Net* newNet = newNode->getNets()[0];
		vector<Node*>& parents = target->getParents();
		for( int j = 0; j < parents.size(); j++ )
		{
			Net* parentNet = parents[j]->getNets()[0];
			const vector<int>& topParentContacts = parentNet->getTopContacts();
			const vector<int>& bottomContacts = newNet->getBottomContacts();
			for( int k = 0; k < topParentContacts.size(); k++ )
			{
				for( int l = 0; l < bottomContacts.size(); l++ )
				{
					if( topParentContacts[k] == bottomContacts[l] )
					{
						newNode->addParent( parents[j] );	
						parents[j]->addChild( newNode );
					}	
				}	
			}
		}
		vector<Node*>& children = target->getChildren();
		for( int j = 0; j < children.size(); j++ )
		{
			Net* childNet = children[j]->getNets()[0];
			const vector<int>& bottomChildContacts = 
												childNet->getBottomContacts();
			const vector<int>& topContacts = newNet->getTopContacts();
			for( int k = 0; k < bottomChildContacts.size(); k++ )
			{
				for( int l = 0; l < topContacts.size(); l++ )
				{
					if( bottomChildContacts[k] == topContacts[l] )
					{
						newNode->addChild( children[j] );
						children[j]->addParent( newNode );
					}
				}
			}
		}
		vector<Node*>::iterator it = find( vNodes.begin(), vNodes.end(), 
										   target );
		vNodes.insert( it, newNode );
	}
	removeNode( target );
	addPseudoSourceSink();
} 

void Vcg::addInterEdge( Node* parent, Node* child, int savings )
{
	if( !edgeExists(parent, child) && !edgeExists(child, parent) )
	{
		EdgeKeyed* edge = new EdgeKeyed( parent, child, savings );
		parent->addInterChild( edge );
		child->addInterParent( edge );
	}
}

void Vcg::removeInterEdge( Node* parent, Node* child )
{
	parent->removeInterChild( child );
	child->removeInterParent( parent );
}

void Vcg::removeInterEdge( EdgeKeyed* edge )
{
	edge->getTail()->removeInterChild( edge );
	edge->getHead()->removeInterParent( edge );
}

void Vcg::addPseudoSource( void )
{
	if( pseudoSource != 0 )
		delete pseudoSource;
	pseudoSource = new Node( new Net(-1) );
	for( int i = 0; i < vNodes.size(); i++ )
	{
		Node* current = vNodes[i];
		if( current->isSource() )
			
			pseudoSource->addChild( current );
	}
	if ( pseudoSource->getChildCount() < 2 )
	{
		if( vNodes.size() > 1 )
			pseudoSource->addChild( vNodes[1] );
	}
}

void Vcg::addPseudoSink( void )
{
	if( pseudoSource != 0 )
		delete pseudoSink;
	pseudoSink = new Node ( new Net(-2) );
	for( int i = 0; i < vNodes.size(); i++ )
	{
		if( vNodes[i]->isSink() )
			pseudoSink->addParent( vNodes[i] );
	}
	if ( pseudoSink->getParentCount() < 2 )
	{
		if( vNodes.size() > 1 )
			pseudoSink->addParent( vNodes[ vNodes.size() - 1 ] );
	}
}

void Vcg::addPseudoSourceSink( void )
{
	addPseudoSource();
	addPseudoSink();
}

void Vcg::removePseudoSource( void )
{
	if( pseudoSource != 0 )
	{
		vector<Node*>& children = pseudoSource->getChildren();
		for( int i = 0; i < children.size(); i++ )
		{
			removeEdge( pseudoSource, children[i] );
		}
	}
}

void Vcg::removePseudoSink( void )
{
	if( pseudoSink != 0 )
	{	
		vector<Node*>& parents = pseudoSink->getParents();
		for( int i = 0; i < parents.size(); i++ )
		{
			removeEdge( parents[i], pseudoSink );
		}
	}
}

void Vcg::removePseudoSourceSink( void )
{
	removePseudoSource();
	removePseudoSink();
}

bool Vcg::checkCycles( Node* current, vector<Node*>& path )
{ 
	bool cycles = false;
	if( current->isVisited() )
	{
		vector<Node*>::iterator search = find( path.begin(), path.end(),
											   current );
		if( search != path.end() ) 
		{
			cycles = true;
			#ifdef DEBUG
				cout << "Cycle detected at Node " << current->getID() << endl;
				for( ; 
					 search < path.end(); search++ )
				{
					cout << (*search)->getID() << " ";
				}
				cout << current->getID() << endl;
			#endif
		}
	}
	else
	{
		current->setVisited( true );
		path.push_back( current );
		vector<Node*> children = current->getChildren();
		for( vector<Node*>::iterator it = children.begin(); 
			 (it < children.end( )) && (!cycles); it++ )
		{
			cycles = checkCycles( *it, path );
		}
		path.pop_back();
	}
	return cycles;
}

/*bool Vcg::checkCycles( void )
{
	addPseudoSourceSink();
	if( pseudoSource->getChildren().empty() )
	{
		cerr << "Graph has no source.  Cycle must exist.  Exiting..." << endl;
		exit(1);
	}	
	setVisitedFlags( false );
	#ifdef DEBUG
		cout << "Checking for cycle in enforced edges..." << endl;
	#endif
	vector<Node*> path;
	bool cycles = checkCycles( pseudoSource, path );
	#ifdef DEBUG
		cout << "done checking for cycles." << endl;
	#endif
	setVisitedFlags( false );
	return cycles;
}*/

bool Vcg::checkCycles( void )
{
	bool cycle = false;
	vector<Node*> topOrder;
	topological(topOrder);
	for( int i = 0; (i < topOrder.size( )) && (!cycle); i++ )
	{
		for( int j = i + 1; (j < topOrder.size( )) && (!cycle); j++ )
		{
			cycle = edgeExists( topOrder[j], topOrder[i] );
			#ifdef DEBUG
				if( cycle )
				{
					cout << "Cycle detected from node ";
					cout << topOrder[j]->getID();	
					cout << " to node ";
					cout << topOrder[i]->getID();
					cout << "." << endl;
				}
			#endif
		}
	}
	return cycle;
}
		
			
void Vcg::initializeVcg( vector< vector<int> >& top, 
						 vector< vector<int> >& bottom )
{
	vNodes.clear();
	if( top.size() == bottom.size() )
	{
		for( int i = 0; i < top.size(); i++ )
		{
			vector<int>& topNets = top[i];
			vector<int>& bottomNets = bottom[i];
			for( vector<int>::iterator it = topNets.begin(); it < topNets.end();
				 it++ )
			{
				int tail = *it;
				for( vector<int>::iterator bit = bottomNets.begin(); 
					 bit < bottomNets.end(); bit++ )
				{
					int head = *bit;	
					addEdge( tail, head, i );
				}
			}
		}
	}
	else
	{
		cerr << "ERROR: There is a different number of top and bottom ";
		cerr << "contacts.  Exiting..." << endl;
		exit(1);
	} 
}

void Vcg::initializeVcg( vector< vector<Net*> >& nets, 
						 const vector< vector<int> >& top, 
						 const vector< vector<int> >& bottom )
{
	vNodes.clear();
	for( vector< vector<Net*> >::iterator it = nets.begin(); it < nets.end(); 
		 it++ )
	{
		for( vector<Net*>::iterator nit = it->begin(); nit < it->end(); nit++ )
			addNode( *nit );
	}
	/*if( top.size() == bottom.size() )
	{
		for( int i = 0; i < top.size(); i++ )
		{
			addEdge( vNodes[top[ i ]], vNodes[ bottom[ i ]] );
		}
	}*/
}


vector<Net*>& Vcg::S( int col ) const
{
	vector<Net*>* vReturn = new vector<Net*>();
	for( int i = 0; i < vNodes.size(); i++ )
	{
		vector<Net*> nets = vNodes[i]->getNets();
		for( int j = 0; j < nets.size(); j++ )
		{
			Net* net = nets[j];
			if( (net->getID() != 0) && (net->contains( col )) )
				vReturn->push_back( net );
		}
	}
	return *vReturn;
}

void Vcg::merge( Node* first, Node* second )
{
	#ifdef DEBUG
		vector<Net*> firstNets = first->getNets();
		vector<Net*> secondNets = second->getNets();
		cout << "Merging nets ";
		for( int i = 0; i < firstNets.size(); i++ )
			cout << firstNets[i]->getID() << " ";
		cout << "with ";
		for( int i = 0; i < secondNets.size(); i++ )
			cout << secondNets[i]->getID() << " ";
		cout << endl;
	#endif
	vector<Node*> secondParents = second->getParents();
	vector<Node*> secondChildren = second->getChildren();
	first->addParents( secondParents );
	first->addChildren( secondChildren );
	first->addNets( second->getNets() );
	for( int i = 0; i < secondParents.size(); i++ )
		secondParents[ i ]->exchangeChild( second, first );
	for( int i = 0; i < secondChildren.size(); i++ )
		secondChildren[ i ]->exchangeParent( second, first );
	bool found = false;
	vector<Node*>::iterator it;
	vNodes.erase( remove( vNodes.begin(), vNodes.end(), second ), 
				  vNodes.end() );
	/* #ifdef DEBUG
		cout << "AFTER A MERGE:" << endl;
		print();
	#endif */
}

bool Vcg::edgeExists( Node* parent, Node* child )
{
	bool edgeExists = false;
	if( parent->hasChild(child) )
	{
		if( child->hasParent(parent) )
		{
			edgeExists = true;
		}
		else
		{
			cerr << "ERROR: INCONSISTENT VCG.  A parent has a child,";
			cerr << " but that child does not have the parent.\n";
		}
	}
	return edgeExists;
}
	

bool Vcg::interEdgeExists( Node* interParent, Node* interChild )
{
	bool interEdgeExists = false;
	if( interParent->hasInterChild(interChild) )
	{
		if( interChild->hasInterParent(interParent) )
		{
			interEdgeExists = true;
		}
		else
		{
			cerr << "ERROR: INCONSISTENT VCG.\n";
			cerr << "A Parent has a child which does not have that parent in ";
			cerr << "its parent list.\n";
		}
	}
	return interEdgeExists;
}


bool Vcg::pathExists( Node* node1, Node* node2 )
{
	setVisitedFlags( false );
	return( node1->isInPath(node2) );
}

void Vcg::getPath( Node* from, Node* to, vector<Node*>& path )
{
	setVisitedFlags( false );
	getPathHelper( from, to, path );
}

bool Vcg::getPathHelper( Node* from, Node* to, vector<Node*>& path )
{
	bool found = false;
	from->setVisited( true );
	path.push_back( from );
	if( from != to )
	{
		vector<Node*>& children = from->getChildren();
		for( int i = 0; (i < children.size( )) && (!found); i++ )
		{
			if( !children[i]->isVisited() )
			{
				found = getPathHelper( children[i], to, path );
			}
		}
		if( !found )
		{
			path.pop_back();
		}
	}
	else
	{
		found = true;
	}
	return found;
}

void Vcg::processInterEdges( void )
{
	for( int i = 0; i < vNodes.size(); i++ )
	{
		vNodes[i]->processInterEdges();
	}
}

void Vcg::print( void ) const
{
	cout << "I AM A VCG.  I have " << vNodes.size() << " nodes:\n";
	for( int i = 0; i < vNodes.size(); i++ )
	{
		vNodes[ i ]->print();
	}
	cout << "My pseudoSource:\n";
	if( pseudoSource != 0 )
		pseudoSource->print();
	cout << endl;
	cout << "My pseudoSink:\n";
	if( pseudoSink != 0 )
		pseudoSink->print();
	cout << endl;
}

void Vcg::setVisitedFlags( bool isVisited )
{
	for( int i = 0; i < vNodes.size(); i++ )
	{
		vNodes[i]->setVisited( isVisited );
	}
	if( pseudoSource != 0 )
		pseudoSource->setVisited( false );
	if( pseudoSink != 0 )
		pseudoSink->setVisited( false );
}

void Vcg::topologicalDFSRand( Node* node, vector<Node*>& topOrderRev, 
							  min_rand& minRand )
{
	node->setVisited( true );
	vector<Node*> children = node->getChildren();
	vector<int> indices;
	for( int i = 0; i < children.size(); i++ ) 
	{
		indices.push_back( i );
	}
	random_shuffle( indices.begin(), indices.end(), minRand );
	while( !indices.empty() )
	{
		topologicalDFSRand( children[ indices.back() ], topOrderRev, minRand );
		indices.pop_back();
	}
	if( node->isExplored() )
	{
		#ifdef DEBUG
			cout << "Node " << node->getID() << " is explored\n";
		#endif
		vector<Node*>::const_iterator search = find( topOrderRev.begin(), topOrderRev.end(),
							  node );
		if( search == topOrderRev.end() )
		{
			for( int i = 0; i < topOrderRev.size(); i++ )
			{
				#ifdef DEBUG
					cout << "Looking for interEdge between Nodes ";
					cout << topOrderRev[i]->getID() << " and " << node->getID();
					cout << "...\n";
				#endif
				if(	interEdgeExists(topOrderRev[i], node) )
				{
					#ifdef DEBUG
						cout << "Found edge, removing...\n";
					#endif
					removeInterEdge( topOrderRev[i], node );
				}
			}
			topOrderRev.push_back( node );
		}
	}
}

void Vcg::topologicalDFSRand( void )
{
	addPseudoSource();
	vector<Node*> topOrder;
	time_t sysClockSeed = time( 0 );
	min_rand minRand( sysClockSeed );
	topologicalDFSRand( pseudoSource, topOrder, minRand );
	#ifdef DEBUG
		cout << "Reverse Topological Order:\n";
		for( int i = 0; i < topOrder.size(); i++ )
		{
			cout << topOrder[i]->getID() << " ";
		}
		cout << endl;
	#endif
	setVisitedFlags( false );
	#ifdef DEBUG
		print();
	#endif
}

void Vcg::topologicalDFS( Node* node, vector<Node*>& topOrderReverse )
{
	node->setVisited( true );
	vector<Node*> children = node->getChildren();
	for( int i = 0; i < children.size(); i++ )
	{
		topologicalDFS( children[i], topOrderReverse );
	}
	if( node->isExplored() )
	{
		#ifdef DEBUG
			cout << "Node " << node->getID() << " is explored\n";
		#endif
		vector<Node*>::const_iterator search = find( topOrderReverse.begin(), topOrderReverse.end(),
							  node );
		if( search == topOrderReverse.end() )
		{
			for( int i = 0; i < topOrderReverse.size(); i++ )
			{
				if(	interEdgeExists(topOrderReverse[i], node) )
				{
					removeInterEdge( topOrderReverse[i], node );
				}
			}
			topOrderReverse.push_back( node );
		}
	}
}

void Vcg::topologicalDFS( void )
{
	addPseudoSource();
	vector<Node*> topOrder;
	topologicalDFS( pseudoSource, topOrder );
	#ifdef DEBUG
		cout << "Reverse Topological Order:\n";
		for( int i = 0; i < topOrder.size(); i++ )
		{
			cout << topOrder[i]->getID() << " ";
		}
		cout << endl;
	#endif
	setVisitedFlags( false );
}

void Vcg::topological( Node* node, vector<Node*>& topOrderReverse )
{
		node->setVisited( true );
		vector<Node*> children = node->getChildren();
		for( int i = 0; i < children.size(); i++ )
		{
			if( !children[i]->isVisited() )
				topological( children[i], topOrderReverse );
		}
		topOrderReverse.push_back( node );
}

void Vcg::topological( vector<Node*>& topOrder )
{
	addPseudoSourceSink();
	setVisitedFlags( false );
	topological( pseudoSource, topOrder );
	topOrder.pop_back();
	reverse( topOrder.begin(), topOrder.end() );
	#ifdef DEBUG
		cout << "Topological Order:\n";
		for( int i = 0; i < topOrder.size(); i++ )
		{
			cout << topOrder[i]->getID() << " ";
		}
		cout << endl;
	#endif
	setVisitedFlags( false );
}

void Vcg::mergePrep( void )
{
	#ifdef DEBUG
		cout << "In mergePrep()...\n";
	#endif
	addPseudoSourceSink();
	vector<Node*> topOrder;
	topological( topOrder );
	#ifdef DEBUG
		cout << "Top Order:" << endl;
		for( vector<Node*>::iterator it = topOrder.begin(); 
			 it < topOrder.end(); it++ )
		{
			cout << (*it)->getID() << " ";
		}
		cout << endl;
	#endif
	longestPaths( pseudoSource, pseudoSink, topOrder );
	#ifdef DEBUG
		cout << "Leaving mergePrep()...\n";
	#endif
}

Node* Vcg::addInterPseudoSource( void )
{
	Node* node = new Node( );
	for( vector<Node*>::iterator it = vNodes.begin() + 1; it < vNodes.end(); 
		 it++ )
	{
		int parents = (*it)->getParentCount() + (*it)->getInterParentCount();
		if( parents == 0 )
		{
			#ifdef DEBUG
				cout << "InterPseudoSource child added: " << (*it)->getID();
				cout << endl;
			#endif
			node->addChild( *it );
		}
	}
	return node;
}

bool Vcg::pathContainsEdge( EdgeKeyed* edge, vector<EdgeKeyed*>& path )
{
	bool contains = false;
	vector<EdgeKeyed*>::const_iterator search = find( path.begin(), path.end(),
				      								  edge ); 
	if( search != path.end() )
		contains = true;
	return contains;
}
	
	


bool Vcg::cycleBreaker( void )
{
	#ifdef DEBUG
		cout << "In cycleBreaker..." << endl;
	#endif
	Node* source = addInterPseudoSource();
	addPseudoSource();
	vector< vector<EdgeKeyed*>* > edgePaths;
	vector<EdgeKeyed*> pathEdges;
	vector<Node*> pathNodes;
	pathsFinder( source, edgePaths, pathEdges, pathNodes );
	#ifdef DEBUG
		cout << "Paths found:" << endl;
		int i = 0;
		for( vector< vector<EdgeKeyed*>* >::iterator it = edgePaths.begin();
			 it < edgePaths.end(); it++ )
		{
			vector<EdgeKeyed*>* edgePath = *it;
			cout << "Path " << i++ << ": ";
			for( vector<EdgeKeyed*>::iterator jt = edgePath->begin(); 
				 jt < edgePath->end(); jt++ )
			{
				Edge* edge = *jt;
				cout << edge->getTail()->getID() << "->";
				cout << edge->getHead()->getID() << " ";
			}
			cout << endl;
		}
	#endif
	bool broken = false;
	if( !edgePaths.empty() )
		broken = true;
	while( edgePaths.size() > 0 )
	{
		Vcg* vcg = this;
		CycleBreaker::breakCycles( vcg, edgePaths );
	}
	setVisitedFlags( false );
	#ifdef DEBUG
		cout << "Leaving cycleBreaker..." << endl;
	#endif
	return broken;
}

vector<EdgeKeyed*>::iterator Vcg::findEdgeInPath( vector<EdgeKeyed*>& path, 
												   Node* node )
{
	vector<EdgeKeyed*>::iterator itReturn = 0;
	bool found = false;
	for( vector<EdgeKeyed*>::iterator it = path.begin(); 
		 (it < path.end( )) && (!found); it++  	    )
	{
		if( (*it)->getTail() == node )
		{
			found = true;
			itReturn = it;
		}
	}
	return itReturn;
}

void Vcg::pathsFinder( Node* node, vector< vector<EdgeKeyed*>* >& edgePaths, 
					  vector<EdgeKeyed*>& pathEdges, vector<Node*>& pathNodes )
{
	if( node->isVisited() )
	{	
		vector<Node*>::iterator it = find( pathNodes.begin(), pathNodes.end(), 
										   node                              );
		bool found = false;
		while( (it < pathNodes.end( )) && (!found) )
		{
			vector<EdgeKeyed*>::iterator edge = findEdgeInPath( pathEdges,
																 *it );
			if( edge == 0 )
				it++;
			else
			{
				found = true;
				vector<EdgeKeyed*>* newPath = new vector<EdgeKeyed*>;
				copy ( edge, pathEdges.end(), back_inserter(*newPath) );
				edgePaths.push_back( newPath );
			}
		}
	}
	else
	{
		node->setVisited( true );
		pathNodes.push_back( node );
		for( vector<EdgeKeyed*>::iterator it = 
			  node->getInterChildren().begin(); 
			 it < node->getInterChildren().end(); it++ )
		{
			pathEdges.push_back( *it );
			pathsFinder( (*it)->getHead(), edgePaths, pathEdges, pathNodes );
			pathEdges.pop_back();
		}
		for( vector<Node*>::iterator it = node->getChildren().begin(); 
		 	 it < node->getChildren().end(); it++ )
		{
			pathsFinder( *it, edgePaths, pathEdges, pathNodes );
		}
		pathNodes.pop_back();
		//node->setVisited( false );
	}
}

/*
int main( void )
{
	vector<int> top = new vector<int>;
	top.push_back (1);
	top.push_back (17);
	top.push_back (4);
	top.push_back (20);
	top.push_back (19);
	top.push_back (12);
	top.push_back (12);
	top.push_back (15);
	top.push_back (8);
	top.push_back (14);
	top.push_back (10);
	top.push_back (7);





	
	Vcg* vcg = new Vcg( );
	vcg->initialize( 20 );
	vcg->print();
	vcg->addEdge( 25, 28, 12 );
	vcg->print();
}
*/
