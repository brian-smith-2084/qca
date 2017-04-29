#include <algorithm>
#include "doglegs.h"
#include "net.h"
#include "vcg.h"

using std::cin;

//#define DEBUG true

struct Dogleg
{
	int net;
	int column;
};

vector<Dogleg*> doglegs;



int Doglegs::getDoglegCrossCount( vector< vector<Net*> > vNets )
{
	int count = 0;
	for( vector<Dogleg*>::const_iterator it = doglegs.begin(); 
		 it < doglegs.end(); it++ )
	{
		int top = INT_MAX;
		int bottom = -1;
		int netID = (*it)->net;
		for( int i = 0; i < vNets[netID].size(); i++ )
		{
			Net* subnet = vNets[netID][i];
			if( subnet->getTrack() > bottom )
				bottom = subnet->getTrack();
			if( subnet->getTrack() < top )
				top = subnet->getTrack();
		}
		for( int i = 0; i < vNets.size(); i++ )
		{
			if( (i != 0) && (i != netID) )
			{
				for( int j = 0; j < vNets[i].size(); j++ )
				{
					if( vNets[i][j]->contains((*it)->column) )
					{
						int track = vNets[i][j]->getTrack();
						if( (track < bottom) && (track > top) )
							count++;
					}
				}
			}
		}
	}
	return count;
}

void Doglegs::subnet( Net* target, vector<int>& columns, 
					  vector<Net*>& subnets )
{
	#ifdef DEBUG
		cout << "Subnetting net " << target->getID() << "..." << endl;
		target->print();
	#endif
	int netID = target->getID();
	for( int i = 0; i < columns.size(); i++ )
	{
		int column = columns[i];
		#ifdef DEBUG
			cout << " at column " << column << "..." << endl;
		#endif
		Net* net = new Net( netID );
		const vector<int>& topContacts = target->getTopContacts();
		for( int j = 0; j < topContacts.size(); j++ )
		{
			#ifdef DEBUG
				cout << "Checking top contact " << topContacts[j];
				cout << "..." << endl;
			#endif
			if( topContacts[j] < column )
			{
				#ifdef DEBUG
					cout << "Adding top contact " << topContacts[j];
					cout << " to subnet " << i << endl;
				#endif
				net->addTopContact( topContacts[j] );	
				target->removeTopContact( topContacts[j] );
				j--;
			}
		}
		const vector<int>& bottomContacts = target->getBottomContacts();
		for( int j = 0; j < bottomContacts.size(); j++ )
		{
			#ifdef DEBUG
				cout << "Checking bottom contact " << bottomContacts[j];
				cout << "..." << endl;
			#endif
			if( bottomContacts[j] < columns[i] )
			{
				#ifdef DEBUG
					cout << "Adding bottom contact " << bottomContacts[j];
					cout << " to subnet " << i << endl;
				#endif
				net->addBottomContact( bottomContacts[j] );
				target->removeBottomContact( bottomContacts[j] );
				j--;
			}
		}
		net->setLeftEdge( target->getLeftEdge() );
		net->setRightEdge( columns[i] );
		target->setLeftEdge( columns[i] );
		subnets.push_back( net );
	}
	Net* net = new Net( netID );
	*net = *target;
	subnets.push_back( net );
	setSubnetIDs( subnets );
}

void Doglegs::getEdgeColumns( Net* parent, Net* child, vector<int>& columns )
{
	const vector<int>& topContacts = parent->getTopContacts();
	const vector<int>& bottomContacts = child->getBottomContacts();
	for( int i = 0; i < topContacts.size(); i++ )
	{
		for( int j = 0; j < bottomContacts.size(); j++ )
		{
			if( topContacts[i] == bottomContacts[j] )
			{
				columns.push_back( topContacts[i] );
			}
		}
	}
}

void Doglegs::modifyNetForDogleg( Net* net, int column )
{
	int leftEdge = net->getLeftEdge();
	if( leftEdge >= column )
	{
		net->setLeftEdge( leftEdge + 1 );
	}
	int rightEdge = net->getRightEdge();
	if( rightEdge >= column )
	{
		net->setRightEdge( rightEdge + 1 );
	}
	vector<int>& topContacts = net->getTopContacts();
	for( int i = 0; i < topContacts.size(); i++ )
	{
		if( topContacts[i] >= column )
		{
			#ifdef DEBUG
				cout << "top contact " << topContacts[i] << " becomes ";
				cout << "top contact " << topContacts[i]+1 << endl;
			#endif
			topContacts[i]++;
		}
	}
	vector<int>& bottomContacts = net->getBottomContacts();
	for( int i = 0; i < bottomContacts.size(); i++ )
	{
		if( bottomContacts[i] >= column )
		{
			#ifdef DEBUG
				cout << "bottom contact " << bottomContacts[i] << " becomes ";
				cout << "bottom contact " << bottomContacts[i]+1 << endl;
			#endif
			bottomContacts[i]++;
		}
	}
}

void Doglegs::modifyColumnsForDogleg( vector<int>& top_row, 
									  vector<int>& bottom_row, 
									  vector< vector<Net*> >& vNets, 
									  vector<int>& columns )
{
	for( int i = 0; i < columns.size(); i++ )
	{
		top_row.insert( top_row.begin() + columns[i], 0 );
		bottom_row.insert( bottom_row.begin() + columns[i], 0 );	
		for( int j = 0; j < vNets.size(); j++ )
		{
			for( int k = 0; k < vNets[j].size(); k++ )
			{
				#ifdef DEBUG
					cout << "Modifying nets " << vNets[j][0]->getID() << endl;
				#endif
				modifyNetForDogleg( vNets[j][k], columns[i] );
			}
		}
		Net* zero = vNets[0][0];
		zero->addTopContact( columns[i] );
		zero->addBottomContact( columns[i] );
		#ifdef DEBUG
			cout << "Net contacts modified." << endl;
		#endif
		for( int j = 0; j < doglegs.size(); j++ )
		{
			if ( doglegs[j]->column >= columns[i] )
			{
				doglegs[j]->column = doglegs[j]->column + 1;
			}		
		}
		#ifdef DEBUG
			cout << "dogleg contacts modifed." << endl;
		#endif
		for( int j = i + 1; j < columns.size(); j++ )
		{
			if( columns[j] >= columns[i] )
			columns[j]++;
		}
	}
}

void Doglegs::getColumnsToDogleg( Net* target, Net* parent, Net* child, 
								  vector<int>& columnsToDogleg )
{
	vector<int> parentColumns;
	getEdgeColumns( parent, target, parentColumns );
	vector<int> childColumns;
	getEdgeColumns( target, child, childColumns );
	enum NetMostLeft { PARENT, CHILD };
	enum NetMostLeft netMostLeft;
	if( parentColumns.front() < childColumns.front() )
	{
		netMostLeft = PARENT;
	}
	else
	{
		netMostLeft = CHILD;
	}
	while( !parentColumns.empty() && !childColumns.empty() )
	{
		switch( netMostLeft )
		{
			case PARENT:
			{
				if( childColumns.front() < parentColumns.front() )
				{
					netMostLeft = CHILD;
					columnsToDogleg.push_back( childColumns.front() );
					childColumns.erase( childColumns.begin() );
				}	
				else
				{
					parentColumns.erase( parentColumns.begin() );
				}
				break;
			}
			case CHILD:
			default:
			{
				if( parentColumns.front() < childColumns.front() )
				{
					netMostLeft = PARENT;
					columnsToDogleg.push_back( parentColumns.front() );
					parentColumns.erase( parentColumns.begin() );
				}	
				else
				{
					childColumns.erase( childColumns.begin() );
				}
				break;
			}
		}
	}
	if( !parentColumns.empty() )
		columnsToDogleg.push_back( parentColumns.front() );
	else
	{
		if( !childColumns.empty() )
		{
			columnsToDogleg.push_back( childColumns.front() );
		}
	}
}

void Doglegs::addDogleg( Net* target, Net* parent, Net* child, 
						 vector<int>& top_row, vector<int>& bottom_row, 
						 vector< vector<Net*> >& vNets, Vcg& vcg )
{
	#ifdef DEBUG
		cout << "ADDING DOGLEG:" << endl;
		cout << "Parent:" << endl;
		parent->print();
		cout << "Target:" << endl;
		target->print();
		cout << "Child:" << endl;
		child->print();
	#endif
	vector<int> columnsToDogleg;
	getColumnsToDogleg( target, parent, child, columnsToDogleg );
	#ifdef DEBUG
		cout << "Columns to dogleg:" << endl;
		for( int i = 0; i < columnsToDogleg.size(); i++ )
		{
			cout << columnsToDogleg[i] << " ";
		}
		cout << endl;
	#endif	
	modifyColumnsForDogleg( top_row, bottom_row, vNets, columnsToDogleg );
	#ifdef DEBUG
		cout << "Columns modified." << endl;
	#endif
	for( int i = 0; i < columnsToDogleg.size(); i++ )
	{
		Dogleg* dogleg = new Dogleg();
		dogleg->net = target->getID();
		dogleg->column = columnsToDogleg[i];
		doglegs.push_back( dogleg );
	}
	vector<Net*> subnets;
	#ifdef DEBUG
		cout << "Creating subnets..." << endl;
	#endif
	subnet( target, columnsToDogleg, subnets );
	#ifdef DEBUG
		cout << "Subnets created." << endl;
		cout << "Target net:" << target << endl;
		target->print();
		cout << "Columns to dogleg:";
		for( int i = 0; i < columnsToDogleg.size(); i++ )
		{
			cout << columnsToDogleg[i] << " ";
		}
		cout << endl;
		cout << "Subnets:" << endl;
		for( int i = 0; i < subnets.size(); i++ )
		{
			cout << subnets[i] << endl;
			subnets[i]->print();
		}
	#endif
	vector<Net*>& nets = vNets[ target->getID() ];
	vector<Node*> nodes;
	for( int i = 0; i < subnets.size(); i++ )
	{
		vector<Net*>::iterator it = find( nets.begin(), nets.end(), target);
		#ifdef DEBUG
			if( it == nets.end() )
			{
				cout << "target net not found in nets at netID." << endl;
			}
			cout << "Inserting new nets..." << endl;
		#endif
		#ifdef DEBUG
			cout << "i = " << i << endl;
			subnets[i]->print();
		#endif
		nets.insert(it, subnets[i] );
		#ifdef DEBUG
			cout << "Net inserted." << endl;
		#endif
		Node* node = new Node( subnets[i] );
		nodes.push_back( node );
	}
	#ifdef DEBUG
		cout << "Erasing old net..." << endl;
	#endif
	nets.erase( remove(nets.begin(), nets.end(), target), nets.end() );
	#ifdef DEBUG
		cout << "Splitting Node..." << endl;
	#endif
	setSubnetIDs( nets );
	vcg.splitNode( target->getNode(), nodes );
}
	 
void Doglegs::removeContactCycles( vector<int>& top_row, 
								   vector<int>& bottom_row, 
						  		   Vcg& vcg, vector< vector<Net*> >& vNets  )
{
	doglegs.clear();
	bool cycles = true;
	#ifdef DEBUG
		int counter = 0;
	#endif
	while( cycles )
	{
		#ifdef DEBUG
			cout << "Counter = " << counter << endl;
			counter++;
		#endif
		cycles = false;
		vector<Node*> topOrder;
		vcg.topological( topOrder );
		#ifdef DEBUG
			cout << "In removeContactCycles, Topological Order:" << endl;
			for( int i = 0; i < topOrder.size(); i++ )
			{
				cout << topOrder[i]->getID() << " ";	
			}
			cout << endl;
		#endif
		for( int i = 1; (i < topOrder.size( )) && (!cycles); i++ )
		{
			for( int j = 0; (j < i) && (!cycles); j++ )
			{
				if( vcg.edgeExists(topOrder[i], topOrder[j]) )
				{
					cycles = true;
					#ifdef DEBUG
						cout << "cycle exists at nets ";
						cout << topOrder[i]->getNets()[0]->getID();
						cout << topOrder[i]->getNets()[0]->getSubID();
						cout << " and ";
						cout << topOrder[j]->getNets()[0]->getID();
						cout << topOrder[j]->getNets()[0]->getSubID();
						cout << endl;
					#endif
					vector<Node*> path;
					vcg.getPath( topOrder[j], topOrder[i], path );
					#ifdef DEBUG
						cout << "Path found:" << endl;
						for( int k = 0; k < path.size(); k++ )
						{
							cout << path[k]->getID() << "->";
						}		
						cout << endl;
					#endif
					Net* target = topOrder[j]->getNets()[0];
					Net* parent = topOrder[i]->getNets()[0];
					Net* child = path[1]->getNets()[0];
					addDogleg( target, parent, child, top_row, bottom_row, 
							   vNets, vcg );
					#ifdef DEBUG
						if( cycles )
						{
							cout << "cycles remains true." << endl;
						}
						else
						{
							cout << "cycles is false." << endl;
						}
					#endif
				}
			}
		}
	}
}

void Doglegs::setSubnetIDs( vector<Net*>& subnets )
{
	char ID = 'a';
	for( int i = 0; i < subnets.size(); i++ )
	{
		subnets[i]->setSubID( ID++ );
	}
}
