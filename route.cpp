/*  Source file:         route.cpp 
 *
 *  Function:            route 
 *
 *  Purpose:            
 * 						
 *
 *  Author:              Brian S. Smith
 *  Date:                /2004
 *
 */
#include "route.h"
#include "doglegs.h"
//#define DEBUG true

void printInt( int i )
{
	cout << i << " ";
}

void printInts( vector<int>& ints )
{
	for_each( ints.begin(), ints.end(), printInt );
	cout << endl;
}

void printRows( vector< vector<int> > rows )
{
	for_each( rows.begin(), rows.end(), printInts );
}

void printContacts( void )
{
	cout << "CONTACT INFORMATION:" << endl;
	cout << "Top rows: " << top_rows.size() << endl;
	cout << "Bottom rows: " << bottom_rows.size() << endl;
	for( int i = 0; i < top_rows.size(); i++ )
	{
		cout << "Top Row " << i << ":" << endl;
		printInts( top_rows[i] );
		cout << "Bottom Row " << i << ":" << endl;
		printInts( bottom_rows[i] );
	}
}
		

/*
 * void readFromFile( string& infile )
 *
 * This function reads the Vertex and Edge object information from the
 * source file with the name matching the argument infile. It then reads
 * in the vertex and edge count from line 0, which will be used in DEBUG
 * code to compare to the vertex and edge count derived in the 
 * printStats() function.  It calls the initializeVertices() function, 
 * to generate the correct vertices, and then goes through the file, 
 * line by line, reading in edge information, and passing that 
 * information to the correct Vertex to create Edge objects.
 */
void readFromFile( const string& infile )
{
	string line;
	#ifdef DEBUG
		cout << "Opening file..." << endl;
	#endif
	ifstream openFile( infile.c_str() );
	#ifdef DEBUG
		cout << "File opened." << endl;
	#endif
	while( !openFile.eof() )
	{
		vector<int> currentTopRow;
		getline( openFile, line );
		#ifdef DEBUG
			cout << line << endl;
		#endif
		if( line.length() > 0 )
		{
			char cLine[ line.length() + 1 ];
			strcpy( cLine, line.c_str() );
			char* token = strtok( cLine, " " );
			while( token != 0 )
			{
				int contact = atoi( token );
				currentTopRow.push_back( contact );
				token = strtok( 0, " " );
			}
		}
			vector<int> currentBottomRow;
			getline( openFile, line );
		if( line.length() > 0 )
		{
			char cLine1[ line.length() + 1 ];
			#ifdef DEBUG
				cout << line << endl;
			#endif
			strcpy( cLine1, line.c_str() );
			char* token = strtok( cLine1, " " );
			while( token != 0 )
			{
				int contact = atoi( token );
				currentBottomRow.push_back( contact );
				token = strtok( 0, " " );
			}
		}
		getline( openFile, line );
		#ifdef DEBUG
			cout << line << endl;
		#endif
		if( (currentTopRow.size() > 0) && (currentBottomRow.size() > 0) )
		{
			top_rows.push_back( currentTopRow );
			bottom_rows.push_back( currentBottomRow );
		}
	}
	#ifdef DEBUG
		cout << "Closing file" << endl;
	#endif
	openFile.close();
	#ifdef DEBUG
		cout << "File closed" << endl;
	#endif
}

void initializeZones( vector<int>& top_row, vector<int>& bottom_row )
{
	vZones.clear();
	int zoneCount = 0;
	Zone* currentZone = new Zone( zoneCount );
	currentZone->setLeftEdge( 0 );
	zoneCount++;
	vZones.push_back( currentZone );
	for( int i = 0; i < top_row.size(); i++ )
	{
		#ifdef DEBUG
			cout << "doings zones, looking at column " << i << "...\n";
		#endif
		currentZone = vZones[ vZones.size() - 1 ];
		vector<Net*> columnNets = vcg.S( i );
		bool newZone = currentZone->zoneEnds( columnNets, i );
		if( newZone == true )
		{
			#ifdef DEBUG
				cout << "new Zone at contact " << i << endl;
			#endif
			Zone* newZone = new Zone( zoneCount );
			zoneCount++;
			newZone->setLeftEdge( i );
			newZone->addNets( columnNets );
			vZones.push_back( newZone );
			currentZone->setRightEdge( i - 1 );
			#ifdef DEBUG
				cout << "Setting Right Edge of Zone "; 
				cout << currentZone->getID() << " at contact ";
				cout << i - 1 << endl;
			#endif
		}
	}
	vZones[ vZones.size() - 1 ]->setRightEdge( top_row.size() - 1 );
}

vector<Node*>& getTerminatingNodes( const Zone* zone )
{
	vector<Node*>* terminatingNodes = new vector<Node*>();
	int zoneEdge = zone->getRightEdge();
	vector<Net*> nets = zone->getNets(); 
	for( int i = 0; i < nets.size(); i++ )
	{
		int netEdge = nets[ i ]->getRightEdge();
		if( netEdge <= zoneEdge )
			terminatingNodes->push_back( nets[i]->getNode() );
	}
	return *terminatingNodes;
}

vector<Node*>& getStartingNodes( const Zone* zone )
{
	vector<Node*>* startingNodes = new vector<Node*>();
	int zoneEdge = zone->getLeftEdge();
	vector<Net*> nets = zone->getNets(); 
	for( int i = 0; i < nets.size(); i++ )
	{
		int netEdge = nets[ i ]->getLeftEdge();
		if( netEdge >= zoneEdge )
			startingNodes->push_back( nets[i]->getNode() );
	}
	return *startingNodes;
}

int getM( const vector<Node*>& Q, const int C )
{
	int fm = 0;
	int m = -1;
	for( int i = 0; i < Q.size(); i++ )
	{
		Node* currentNode = Q[ i ];
		int u = currentNode->getU();
		int d = currentNode->getD();
		int fm_temp = C*( u + d ) + max( u, d );
		if( fm_temp > fm )
		{
			fm = fm_temp;
			m = i;
		}
	}
	return m;
}
	
int getN( const vector<Node*>& P, Node* qm, const int C )
{
	int um = qm->getU();
	int dm = qm->getD();
	double gn = DBL_MAX;	
	int n = -1;
	for( int i = 0; i < P.size(); i++ )
	{
		Node* currentNode = P[ i ];
		#ifdef DEBUG
			cout << "current Q node is " << qm->getID() << endl;
			cout << "current node from P is node " << currentNode->getID();
			cout << endl;
		#endif
		if( !vcg.pathExists(qm, currentNode) )
		{
			#ifdef DEBUG
				cout << "Not in path:" << currentNode->getID() << endl;
			#endif
			int un = currentNode->getU();
			int dn = currentNode->getD();
			int hn = max( un, um ) + max( dn, dm ) 
				 - max( un + dn, um + dm );
			double gn_temp = C * hn - ( sqrt(um * un) + sqrt(dm * dn) );
			if ( gn_temp < gn )
			{
				n = i;
				#ifdef DEBUG
					cout << "new target for merge in P is ";
					cout << currentNode->getNets()[0]->getID();
					cout << ", " << gn_temp << " is less than " << gn << endl;
				#endif
				gn = gn_temp;
			}
		}
		#ifdef DEBUG
			else
			{
				cout << "Node " << qm->getID();
				cout << " is in the path of Node ";
				cout << currentNode->getID() << endl;
			}
		#endif

	}
	return n;
}


void printNodes( const vector<Node*>& nodes )
{
	for( int i = 0; i < nodes.size(); i++ )
	{
		nodes[ i ]->print();
	}
}

void mergePQ( void )
{
	vector<Node*> terminatedNodes;
	for( int i = 0; i < vZones.size() - 1; i++ )
	{
		vector<Node*>& terminatingNodes = 
				getTerminatingNodes( vZones[i] );
		for( int j = 0; j < terminatingNodes.size(); j++ )
		{
			terminatedNodes.push_back( terminatingNodes[ j ] );
		}
		vector<Node*>& startingNodes = getStartingNodes( vZones[i+1] );
		#ifdef DEBUG
			cout << "TerminatedNodes:\n";
			for( int j = 0; j < terminatedNodes.size(); j++ )
			{
				cout << terminatedNodes[j]->getNets()[0]->getID();
				cout << " ";
			}
			cout << "\nStartingNodes:\n";
			for( int j = 0; j < startingNodes.size(); j++ ) 
			{
				 cout << startingNodes[j]->getNets()[0]->getID();
				 cout << " ";
			}
			cout << endl;
		#endif
		vector<Node*>* P = &terminatedNodes;
		vector<Node*>* Q = &startingNodes;
		if( terminatedNodes.size() < startingNodes.size() )
		{
			P = &startingNodes;
			Q = &terminatedNodes;
		}
		while( Q->size() > 0 )
		{

			vector<Node*> Pnodes = (*P);
			vector<Node*> Qnodes = (*Q);
			#ifdef DEBUG
				cout << "Q nodes:\n";
				for( int j = 0; j < Qnodes.size(); j++ )
				{
					cout << Qnodes[j]->getNets()[0]->getID();
					cout << " ";
				}
				cout << "\nP nodes:\n";
				for( int j = 0; j < Pnodes.size(); j++ ) 
				{
			 		cout << Pnodes[j]->getNets()[0]->getID();
			 		cout << " ";
				}
				cout << endl;
			#endif
			int m = getM( Qnodes, CINF );
			int n = getN( Pnodes, Qnodes[m], CINF );
			#ifdef DEBUG
				cout << "N = " << n << endl;
			#endif
			if( n > - 1 )
			{
				Node* Pnode = Pnodes[n];
				Node* Qnode = Qnodes[m];
				#ifdef DEBUG
					cout << "Merging nodes ";
					cout << Pnode->getNets()[0]->getID() << " and ";
					cout << Qnode->getNets()[0]->getID() << endl;
				#endif
				int Pnet = Pnode->getNets()[0]->getID();
				int Qnet = Qnode->getNets()[0]->getID();
				if( Pnet < Qnet ) 
					vcg.merge( (*P)[n], (*Q)[m] );
				else
					vcg.merge( (*Q)[m], (*P)[n] );
				Q->erase( Q->begin() + m );
				P->erase( P->begin() + n );
			}
			else
			{
				#ifdef DEBUG
					cout << "n = -1\n";
				#endif
				Q->erase( Q->begin() + m );
			}
				
		}  
	}
}



void leftEdgeFirst( void )
{
	int track = 0;
	int rightEdge = -1;
	vcg.addPseudoSourceSink();
	vector<Node*> nodes = vcg.getPseudoSource()->getChildren();
	nodes.erase( nodes.begin() );
	vector<Node*> added;
	while( nodes.size() > 0 )
	{
		sort( nodes.begin(), nodes.end(), lessThan() );
		for( int i = 0; i < nodes.size(); i++ )
		{
			Node* node = nodes[i];
			if( node->getLeftEdge() > rightEdge )
			{
				node->setTrack( track );
				rightEdge = node->getRightEdge();
				added.push_back( node );
				nodes.erase( nodes.begin() + i );
				#ifdef DEBUG
					cout << "Assigned track " << track << " to node ";
					cout << node->getNets()[0]->getID() << endl;
					cout << "Right edge is " << rightEdge << endl;
				#endif
				vector<Node*> children = node->getChildren();
				for( int j = 0; j < children.size(); j++ )
				{
					Node* childNode = children[j];
					if( nodesIncludeNodes( added, childNode->getParents( )) )
					{
						nodes.push_back( childNode );
						#ifdef DEBUG
							cout << "Adding node ";
							cout << childNode->getNets()[0]->getID();
							cout << " to node list\n";
						#endif
					}
				}
				i--;
			}
		}
		track++;
		rightEdge = -1;
	}
}

int getTrackCount( void )
{
	vector<Node*>& nodes = vcg.getNodes();
	int lastTrack = 0;
	for( int i = 0; i < nodes.size(); i++ )
	{
		int currentTrack = nodes[ i ]->getTrack();
		if( currentTrack > lastTrack )
		{
			lastTrack = currentTrack;
		}
	}
	return( lastTrack + 1 );	
}

int getIntersectCount( vector<int>& top_row, vector<int>& bottom_row )
{
	int count = 0;
	/*#ifdef DEBUG
		cout << "INTERSECT COUNT MODE:" << endl;
	#endif*/
	for( int i = 0; i < top_row.size(); i++ )
	{
		/*#ifdef DEBUG
			cout << "Column Position " << i << endl;
		#endif	*/
		int top = top_row[i];
		int lowtrack = -1;
		int bottom = bottom_row[i];
		int hightrack = INT_MAX;
		for( vector< vector<Net*> >::iterator it = vNets.begin(); 
			 it < vNets.end(); it++ )
		{
			for( vector<Net*>::iterator nit = it->begin(); 
				 nit < it->end(); nit++ )
			{
				if( ( top != 0 ) &&
					(( *nit )->getID( ) == top) && 
					 ( *nit )->hasTopContact( i ) &&
					(( *nit )->getTrack() > lowtrack) )
				{
						lowtrack = (*nit)->getTrack();
				}
				if( ( bottom != 0 ) &&
						(( *nit )->getID() == bottom) &&
						 ( *nit )->hasBottomContact( i ) &&
					    (( *nit )->getTrack() < hightrack) )
				{
						hightrack = (*nit)->getTrack();
				}
			}
		}
		/*#ifdef DEBUG
			cout << "Top contact is Net " << top << " at level ";
			cout << lowtrack << endl;
			cout << "Bottom contact is Net " << bottom << " at level ";
			cout << hightrack << endl;
		#endif*/
		for( vector< vector<Net*> >::iterator it = vNets.begin();
			 it < vNets.end(); it++ )
		{
			for( vector<Net*>::iterator nit = it->begin(); nit < it->end(); 
				 nit++ )
			{
				int columnCount = 0;
				if( ( top != 0 ) && (( *nit )->getID() != 0 ) &&
					(( *nit )->getID( ) != top) && ( *nit )->contains( i ) &&
					(( *nit )->getTrack() <= lowtrack) )
				{
					columnCount++;
					/*#ifdef DEBUG
						cout << "Net " << (*nit)->getID() << " crosses net ";
						cout << top << " at column " << i << "." << endl;
					#endif*/
				}
				if( ( bottom != 0 ) && (( *nit )->getID() != 0 ) &&
					(( *nit )->getID( ) != bottom) && 
					 ( *nit )->contains( i ) &&
				    (( *nit )->getTrack() >= hightrack) )
				{
					columnCount++;
				/*#ifdef DEBUG
					cout << "Net " << (*nit)->getID() << " crosses net ";
					cout << bottom << " at column " << i << "." << endl;
				#endif*/
				}
				count = count + columnCount;
			}
		}
	}
	return count;
}

void addIntersectionEdgesbyZone( void )
{
	for( int i = 0; i < vZones.size(); i++ )
	{
		Zone* zone = vZones[ i ];
		#ifdef DEBUG
			cout << "In zone " << i << "...\n";
		#endif

		const vector<Net*>& nets = zone->getNets();
		for( int j = 0; j < nets.size() - 1; j++ )	
		{
			Net* currentNet = nets[ j ];
			#ifdef DEBUG
				cout << "currentNet is " << currentNet->getID() << endl;
			#endif
			for( int k = j + 1; k < nets.size(); k++ )
			{
				Net* testNet = nets[ k ];
				int intersectIfAbove = 
				 currentNet->intersectCountIfAbove( testNet );	
				int intersectIfBelow = 
				 currentNet->intersectCountIfBelow( testNet );
				if( intersectIfAbove > intersectIfBelow )
				{
					
					Node* currentNode = currentNet->getNode();
					Node* testNode = testNet->getNode();
						vcg.addInterEdge( testNode, currentNode, 
										 intersectIfAbove - intersectIfBelow );
					#ifdef DEBUG
						cout << "Net " << currentNet->getID();
						cout << " has less intersections if placed ";
						cout << "below Net " << testNet->getID();
						cout << ".\n";
					#endif
				}
				else if( intersectIfAbove < intersectIfBelow )
				{
					
					Node* currentNode = currentNet->getNode();
					Node* testNode = testNet->getNode();
						vcg.addInterEdge( currentNode, testNode,
										 intersectIfBelow - intersectIfAbove );
					#ifdef DEBUG
						cout << "Net " << currentNet->getID();
						cout << " has less intersections if placed ";
						cout << "above Net " << testNet->getID();
						cout << ".\n";
					#endif
				}
				#ifdef DEBUG
					else
					{
						cout << "Net " << currentNet->getID();
						cout << " and Net " << testNet->getID();
						cout << " have no order preference in terms of";
						cout << " intersection count.\n";
					}
				#endif
			}
		}
	}
}

void addIntersectionEdges( void )
{
	vector<Node*>& nodes = vcg.getNodes();
	for( int i = 1; i < nodes.size() - 1; i++ )
	{
		Node* currentNode = nodes[ i ];
		Net* currentNet = currentNode->getNets()[0];
		for( int j = i + 1; j < nodes.size(); j++ )	
		{
			Node* testNode = nodes[ j ];
			Net* testNet = testNode->getNets()[0];	
			int intersectIfAbove = 
			 currentNet->intersectCountIfAbove( testNet );	
			int intersectIfBelow = 
			 currentNet->intersectCountIfBelow( testNet );
			if( intersectIfAbove > intersectIfBelow )
			{
				int intersectSavings = 
								intersectIfAbove - intersectIfBelow;
				vcg.addInterEdge( testNode, currentNode, 
								  intersectSavings );
				#ifdef DEBUG
					cout << "Net " << currentNet->getID();
					cout << " has less intersections if placed ";
					cout << "below Net " << testNet->getID();
					cout << ".\n";
				#endif
			}
			else if( intersectIfAbove < intersectIfBelow )
			{
				int intersectSavings = 
								intersectIfBelow - intersectIfAbove;
				vcg.addInterEdge( currentNode, testNode,
								  intersectSavings );
				#ifdef DEBUG
					cout << "Net " << currentNet->getID();
					cout << " has less intersections if placed ";
					cout << "above Net " << testNet->getID();
					cout << ".\n";
				#endif
			}
			#ifdef DEBUG
				else
				{
					cout << "Net " << currentNet->getID();
					cout << " and Net " << testNet->getID();
					cout << " have no order preference in terms of";
					cout << " intersection count.\n";
				}
			#endif
		}
	}
}

void addIntersectionEdgesLong( void )
{
	vector<Node*>& nodes = vcg.getNodes();
	for( int i = 1; i < nodes.size() - 1; i++ )
	{
		Node* currentNode = nodes[ i ];
		Net* currentNet = currentNode->getNets()[0];
		for( int j = i + 1; j < nodes.size(); j++ )	
		{
			Node* testNode = nodes[ j ];
			Net* testNet = testNode->getNets()[0];	
			int intersectIfAbove = 
			 currentNet->intersectCountIfAbove( testNet );	
			int intersectIfBelow = 
			 currentNet->intersectCountIfBelow( testNet );
			if( intersectIfAbove > intersectIfBelow )
			{
				if( !vcg.pathExists(currentNode, testNode) )
					vcg.addEdge( testNode, currentNode );
				#ifdef DEBUG
					cout << "Net " << currentNet->getID();
					cout << " has less intersections if placed ";
					cout << "below Net " << testNet->getID();
					cout << ".\n";
				#endif
			}
			else if( intersectIfAbove < intersectIfBelow )
			{
					if( !vcg.pathExists(currentNode, testNode) )
						vcg.addEdge( currentNode, testNode );
				#ifdef DEBUG
					cout << "Net " << currentNet->getID();
					cout << " has less intersections if placed ";
					cout << "above Net " << testNet->getID();
					cout << ".\n";
				#endif
			}
			#ifdef DEBUG
				else
				{
					cout << "Net " << currentNet->getID();
					cout << " and Net " << testNet->getID();
					cout << " have no order preference in terms of";
					cout << " intersection count.\n";
				}
			#endif
		}
	}
}

void initializeNets( const vector<int>& top, const vector<int>& bottom )
{
	vNets.clear();
	if( top.size() == bottom.size() )
	{
		for( int i = 0; i < top.size(); i++ )
		{
			int column = i;
			int topNet = top[i];
			int size = vNets.size();
			if( topNet >= size )
			{
				vNets.resize( topNet + 1 );
				for( int j = size; j < vNets.size(); j++ ) 
				{
					vNets[j].push_back( new Net(j) );
				}
			}
			int bottomNet = bottom[i];
			size = vNets.size();
			if( bottomNet >= size )
			{
				vNets.resize( bottomNet + 1 );
				for( int j = size; j < vNets.size(); j++ )
				{
					vNets[j].push_back( new Net(j) );
				}
			}
			/*#ifdef DEBUG
				cout << "Adding contact " << column << " to net ";
				cout << vNets[ topNet ][0]->getID() << " and ";
				cout << vNets[ bottomNet ][0]->getID() << "..." << endl;
			#endif*/
			vNets[ topNet ][0]->addTopContact( column );
			vNets[ bottomNet ][0]->addBottomContact( column );
		}
	}
	else
	{
		cerr << "ERROR: There are a different number of top and bottom ";
		cerr << "contacts.  Exiting..." << endl;
		exit(1);
	}
}

void printNets( void )
{
	cout << "Nets:" << endl;
	for( vector< vector<Net*> >::iterator it = vNets.begin(); it < vNets.end();
 		 it++ )
	{
		for( vector<Net*>::iterator nit = it->begin(); nit < it->end(); nit++ )
		{
			(*nit)->print();
			cout << endl;
		}
	}
	cout << "End of Nets." << endl;
}

void subnet( Net* net )
{
	vector<int> topContacts = net->getTopContacts();
	vector<int> bottomContacts = net->getBottomContacts();
	if( topContacts.size() + bottomContacts.size() > CONTACTS_PER_NET )
	{
		char subNetID = 'a';
		while ( topContacts.size() + bottomContacts.size() >= CONTACTS_PER_NET )
		{
			vector<int> newTopContacts, newBottomContacts;
			for( int j = 0; j < CONTACTS_PER_NET; j++ )
			{
				int top = INT_MAX;
				if( !topContacts.empty() )
					top = topContacts.front();
				int bottom = INT_MAX;
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
			Net* subNet = new Net( net->getID() );
			subNet->setSubID( subNetID );
			subNet->addTopContacts( newTopContacts );
			subNet->addBottomContacts( newBottomContacts );
			vNets[ net->getID() ].push_back( subNet );	
		}
		vNets[ net->getID() ].erase( vNets[net->getID( )].begin() );
	}
}

void applySubNetting( void )
{
	for( vector< vector<Net*> >:: iterator it = vNets.begin(); it < vNets.end();
		 it++ )
	{
		subnet( (*it).front() );
	}
}

void initializeEdges( vector<int>& top_row, vector<int>& bottom_row )
{
	for( int i = 0; i < top_row.size(); i++ )
	{
		int top = top_row[i];
		int bottom = bottom_row[i];
		if( top != bottom )
		{
			for( vector< vector<Net*> >::iterator it = vNets.begin(); 
			 	 it < vNets.end(); it++ )
			{
				for( vector<Net*>::iterator nit = it->begin(); nit < it->end();
					 nit++ )
				{
					if( (*nit)->getID() == top )
					{
						for( vector< vector<Net*> >::iterator jt = 
							 vNets.begin(); jt < vNets.end(); jt++ )
						{
							for( vector<Net*>::iterator njt = jt->begin(); 
							 	 njt < jt->end(); njt++ )
							{
								if( (*njt)->getID() == bottom )
								{
									const vector<int>& topContacts = 
													(*nit)->getTopContacts();
									const vector<int>& bottomContacts = 
												(*njt)->getBottomContacts();
									for( vector<int>::const_iterator itc = 
									 	 topContacts.begin(); 
									 	 itc < topContacts.end(); itc++ )
									{
										for( vector<int>::const_iterator ibc = 
										     bottomContacts.begin(); 
										    ibc < bottomContacts.end(); ibc++ )
										{
											if ( *itc == *ibc )
											{
												vcg.addEdge( (*nit)->getNode(), 													             (*njt)->getNode() );
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
}

void initializeVcg( vector<int>& top_row, vector<int>& bottom_row )
{
	vcg.clear();
	for( vector< vector<Net*> >::iterator it = vNets.begin(); it < vNets.end();
		 it++ )
	{
		for( vector<Net*>::iterator nit = it->begin(); nit < it->end(); nit++ )
		{
			Node* node = new Node( *nit );
			vcg.addNode( node );
		}
	}
	initializeEdges( top_row, bottom_row );
}

void setSubnetIDs( vector<Net*>& subnets )
{
	char ID = 'a';
	for( int i = 0; i < subnets.size(); i++ )
	{
		subnets[i]->setSubID( ID++ );
	}
}

void topologicalPruning( void )
{
	vector<Node*> topOrder;
	vcg.topological( topOrder );
	for( int i = 1; i < topOrder.size(); i++ )
	{
		for( int j = 0; j < i; j++ )
		{ 
			if( vcg.interEdgeExists(topOrder[i], topOrder[j]) )
			{
				#ifdef DEBUG
					cout << "Removing inter edge between ";
					cout << topOrder[i]->getID() << " and ";
					cout << topOrder[i-1]->getID() << endl;
				#endif
				vcg.removeInterEdge( topOrder[i], topOrder[j] );
			}
		}
	}
}

bool verifyTracks( vector<int>& top_row, vector<int>& bottom_row )
{
	bool verified = true;
	if( top_row.size() == bottom_row.size() )
	{
		for( int i = 0; (i < top_row.size( )) && verified; i++ )
		{
			int top = top_row[i];
			int bottom = bottom_row[i];
			if( (top != 0) && (bottom != 0) && (top != bottom) )
			{
				Net* topNet = 0;
				for( vector<Net*>::iterator it = vNets[top].begin();
					 (it < vNets[top].end()) && (topNet == 0); it++ )
				{
					/*#ifdef DEBUG
						cout << "Searching for contact " << top;
						cout << ".  Current net searching for top contact: ";
						cout << (*it)->getID() << endl;
						(*it)->print();
					#endif*/
					if( (*it)->hasTopContact(i) )	
					{
						/*#ifdef DEBUG
							cout << "contact found." << endl;
						#endif*/
						topNet = *it;
					}
				}
				if( topNet == 0 )
				{
					cerr << "ERROR (Verification): Top row contact not found ";
					cerr << "in corresponding net: Contact position " << i;
					cerr << ", in net " << top << "." << endl;
					exit(1);
				}
				Net* bottomNet = 0;
				for( vector<Net*>::iterator it = vNets[bottom].begin();
					 (it < vNets[bottom].end()) && (bottomNet == 0); it++ )
				{
					if( (*it)->hasBottomContact(i) )	
					{
						bottomNet = *it;
					}
				}
				if( bottomNet == 0 )
				{
					cerr << "ERROR (Verification): Bottom row contact not ";
					cerr << "found in corresponding net: Contact position "; 
					cerr << i << ", in net " << bottom << ".";
					cerr  << endl;
					exit(1);
				}
				if( topNet->getTrack() >= bottomNet->getTrack() )
				{
					verified = false;
					cerr << "ERROR (Verification): ";
					cerr << "Track vertical orientation ";
					cerr << "is incorrect.  Net " << topNet->getID();
					cerr << " should be above Net " << bottomNet->getID();
					cerr << ", but it is not" << endl;
				}
				
			}	
		}
	}
	else
	{
		cerr << "ERROR (Verification): Top row contact count does not equal ";
		cerr << "bottom row contact count." << endl;
	}
	return verified;
}

void printTracks( void )
{
	cout << "Track Assignments:" << endl;
	for( int i = 0; i < vNets.size(); i++ )
	{
		for( int j = 0; j < vNets[i].size(); j++ )
		{
			cout << vNets[i][j]->getID() << vNets[i][j]->getSubID();
			cout << ": " << vNets[i][j]->getTrack();
			cout << " ";
		}
	}
	cout << endl;
}

/*
 * int main( int argc, char* argv[] ) 
 *
 * The main function calculates the run time, as well as calling the 
 * functions in the correct order to find a solution.  Here is the 
 * order:
 * readFromFile(), addPseudoEdges(), dijkstra, printStats()
 * It is also responsable for calculating the run time of the entire
 * solution.
 */
int main( int argc, char* argv[] )
{
	enum intersectModes{ NONE, BASIC, CYCLEPREV, CYCLEMIN, CYCLE2, CYCLEB };
	intersectModes intersectMode = NONE;
	clock_t clockstart = clock();
	string sourceFile = "dr1";
	string filearg = "-i";
	string interarg = "-imin";
	string ilongarg = "-ilong";
	string izonearg = "-izone";
	string icycarg = "-icyc";
	string icyc2arg = "-icyc2";
	string cycbarg= "-cb";
	string ykarg = "-yk";
	string varg = "-v";
	bool yk = false;
	bool verbose = false;
	int errorCount = 0;
	
	string subNetArg = "-sub";
	bool subNets = false;
	string currentarg;
	for( int i = 0; i < argc; i++ )
	{
		currentarg = argv[i];
		#ifdef DEBUG
			cout << "currentarg is:" << currentarg << endl;
		#endif
		if( currentarg == filearg )
			sourceFile = argv[i+1];
		if( currentarg == interarg )
			intersectMode = BASIC;
		if( currentarg == ilongarg )
			intersectMode = CYCLEPREV;
		if( currentarg == icycarg )
			intersectMode = CYCLEMIN;
		if( currentarg == icyc2arg )
			intersectMode = CYCLE2;
		if( currentarg == cycbarg )
			intersectMode = CYCLEB;
		if( currentarg == subNetArg )
			subNets = true;
		if( currentarg == ykarg )
			yk = true;
		if( currentarg == varg )
			verbose = true;
	}
	readFromFile( sourceFile );
	#ifdef DEBUG
		printContacts();
	#endif
	if ( top_rows.size() != bottom_rows.size() )
	{
		cerr << "ERROR: Top row count does not match bottom row count.";
		cerr << "  Exiting..." << endl;
		exit(1);
	}
	else
	{
		int totalTracks = 0;
		int totalIntersects = 0;
		double totalTime = 0;
		int totalDoglegIntersects = 0;
		for( int i = 0; i < top_rows.size(); i++ )
		{
			bool cycles = true;
			cout << "Top Row " << i << ":" << endl;
			printInts( top_rows[i] );
			cout << "Bottom Row " << i << ":" << endl;
			printInts( bottom_rows[i] );
			initializeNets( top_rows[i], bottom_rows[i] );
			#ifdef DEBUG
				printNets();
			#endif
			if( subNets )
			{
				applySubNetting();
				#ifdef DEBUG
					printNets();
				#endif
			}
			initializeVcg( top_rows[i], bottom_rows[i] );
			#ifdef DEBUG
			//	vcg.print();
			#endif
			Doglegs::removeContactCycles( top_rows[i], bottom_rows[i], vcg,
										  vNets ); 
			cycles = vcg.checkCycles();
			#ifdef DEBUG
				cout << "DEBUG: Top Row " << i << ":" << endl;
				printInts( top_rows[i] );
				cout << "Bottom Row " << i << ":" << endl;
				printInts( bottom_rows[i] );
				printNets();
				if( cycles )
					cout << "Cycles still detected." << endl;	
				else
					cout << "No cycles detected." << endl;
			#endif
			if( !cycles )
			{
				initializeZones( top_rows[i], bottom_rows[i] );
				#ifdef DEBUG
					cout << "Zone information: There are " << vZones.size();
					cout << " zones:\n";
					for( int i = 0; i < vZones.size(); i++ )
					{
						vZones[ i ]->print();
					}
					cout << "INITIAL VCG:\n";
					vcg.print();
					cout << endl;
				#endif
				switch( intersectMode )
				{
				case NONE:
					break;
				case BASIC:
					addIntersectionEdges( );
					vcg.processInterEdges();
					break;
				case CYCLEPREV:	
					addIntersectionEdgesLong( );
					#ifdef DEBUG
						cout << "AFTER INTERSECTION EDGES:\n";
					#endif
					vcg.processInterEdges();
					break;
				case CYCLEMIN:
					addIntersectionEdges();
					topologicalPruning();
					vcg.processInterEdges();
					break;
				case CYCLE2:
					addIntersectionEdges();
					vcg.topologicalDFSRand();
					vcg.processInterEdges();
					break;
				case CYCLEB:
					addIntersectionEdges();
					while( vcg.cycleBreaker() );
					vcg.processInterEdges();
					break;
				default:
					break;
				}
			}
			#ifdef DEBUG
				cout << "AFTER INTERSECTION EDGES PROCESSING VCG:\n";
				vcg.print();
			#endif
			cycles = vcg.checkCycles();
			if( !cycles )
			{
				if( yk )
				{
					vcg.mergePrep();
					#ifdef DEBUG
						cout << "After depths calculated:" << endl;
						vcg.print();
					#endif
					mergePQ();
				}
				leftEdgeFirst();
				clock_t clockend = clock();
				double runtime = 
								double( clockend - clockstart )/CLOCKS_PER_SEC;
				runtime = runtime - totalTime;
				#ifdef DEBUG
					cout << "AFTER LEFT EDGE FIRST:\n";
					vcg.print();
				#endif
				int trackCount = getTrackCount();
				int doglegIntersects = Doglegs::getDoglegCrossCount( vNets );
				totalDoglegIntersects = totalDoglegIntersects + 
															doglegIntersects;
				int intersectCount = getIntersectCount( top_rows[i],
									 					bottom_rows[i] ) 
									 + doglegIntersects;
									 
				totalTracks = totalTracks + trackCount;
				totalIntersects = totalIntersects + intersectCount;
				totalTime = totalTime + runtime;	
				cout << "Run time: " << runtime << " sec\n";
				cout << "Track count: " << trackCount << endl;
				cout << "Intersection count: ";
				cout << intersectCount << endl;
				cout << "Intersections due to doglegging:" << doglegIntersects;
				cout << endl;
				cout << endl;
				if( verbose )
					printTracks();
				bool verified = verifyTracks( top_rows[i], bottom_rows[i] );
				if( verified )
				{
					cout << "Track orientations passed verification." << endl;
				}
				else
				{
					cout << "Track orientations did not pass verification";
					errorCount++;
				}
				cout << endl;
			}
			else
			{
				clock_t clockend = clock();
				double runtime = 
								double( clockend - clockstart )/CLOCKS_PER_SEC;
				cout << "Run time: " << runtime << " sec\n";
				cout << "VCG includes cycles after intersection minimization.";
				cout << endl;
				errorCount++;
			}
		}
		cout << "Total Time: " << totalTime << endl;
		cout << "Total Tracks: " << totalTracks << endl;
		cout << "Total Intersections: " << totalIntersects << endl;	
		cout << "Total Dogleg Intersections: " << totalDoglegIntersects << endl;
		cout << "Errors detected: " << errorCount << endl;
	}
	return(0);
}

