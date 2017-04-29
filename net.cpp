#include "net.h"
#define DEBUG true

Net::Net( int id )
{
	iID = id;
	subID = 'a';
	iLeftEdge = INT_MAX;
	iRightEdge = -1;
	track = -1;
}

Net::Net( int id, int leftEdge )
{
	iID = id;
	subID = 'a';
	iLeftEdge = leftEdge;
	iRightEdge = -1;
	track = -1;
}

int Net::getID( void ) const
{
	return iID;
}

char Net::getSubID( void ) const
{
	return subID;
}

int Net::getLeftEdge( void ) const
{
	return iLeftEdge;
}

int Net::getRightEdge( void ) const
{
	return iRightEdge;
}

const vector<int>& Net::getTopContacts( void ) const
{
	return topContacts;
}

vector<int>& Net::getTopContacts( void )
{
	return topContacts;
}

const vector<int>& Net::getBottomContacts( void ) const
{
	return bottomContacts;
}

vector<int>& Net::getBottomContacts( void )
{
	return bottomContacts;
}

int Net::getTopContactCount( void ) const
{
	return topContacts.size();
}

int Net::getBottomContactCount( void ) const
{
	return bottomContacts.size();
}

bool Net::hasTopContact( int topContact ) const
{
	bool hasContact = false;
	vector<int>::const_iterator it = find( topContacts.begin(), 
										   topContacts.end(), topContact );
	if( it != topContacts.end() )
		hasContact = true;
	return hasContact;
}

bool Net::hasBottomContact( int bottomContact ) const
{
	bool hasContact = false;
	vector<int>::const_iterator it = find( bottomContacts.begin(),
										   bottomContacts.end(), 
										   bottomContact );
	if( it != bottomContacts.end() )
		hasContact = true;
	return hasContact;
}

int Net::getTrack( void ) const
{
	return track;
}

Node* Net::getNode( void )
{
	return node;
}

const vector<Zone*>& Net::getZones( void ) const
{
	return zones;
}

bool Net::contains( int col ) const
{
	bool bReturn = false;
	if( col >= iLeftEdge )
	{
		if( col <= iRightEdge ) 
			bReturn = true;
	}
	return bReturn;
}

bool Net::isColumnBetweenEdges( int col ) const
{
	return( (iLeftEdge < col) && (col < iRightEdge) );
}

void Net::setID( int id )
{
	iID = id;
}

void Net::setSubID( char id )
{
	subID = id;
}

void Net::setLeftEdge( int left )
{
	iLeftEdge = left;
}

void Net::setRightEdge( int right )
{
	iRightEdge = right;
}

void Net::addTopContact( int col )
{
	topContacts.push_back( col );
	checkEdges( col );
}

void Net::addTopContacts( vector<int>& newTopContacts )
{
	for( vector<int>::iterator it = newTopContacts.begin(); 
	     it < newTopContacts.end(); it++                   )
	{
		topContacts.push_back( *it );
		checkEdges( *it );
	}
}

void Net::addBottomContact( int col )
{
	bottomContacts.push_back( col );
	checkEdges( col );
}

void Net::addBottomContacts( vector<int>& newBottomContacts )
{
	for( vector<int>::iterator it = newBottomContacts.begin(); 
	     it < newBottomContacts.end(); it++ 		)
	{
		bottomContacts.push_back( *it );
		checkEdges( *it );
	}
}

void Net::modifyTopContact( int contact, int newContact )
{
	#ifdef DEBUG
		cout << "Old top contacts: ";
		for( int i = 0; i < topContacts.size(); i++ )
		{
			cout << topContacts[i] << " ";
		}
		cout << endl;
	#endif
	for( int i = 0; i < topContacts.size(); i++ )
	{
		if( topContacts[i] == contact )
		{
			topContacts[i] = newContact;
			checkEdges( newContact );
		}
	}
	#ifdef DEBUG
		cout << "New top contacts: ";
		for( int i = 0; i < topContacts.size(); i++ )
		{
			cout << topContacts[i] << " ";
		}
		cout << endl;
	#endif
}

void Net::modifyBottomContact( int contact, int newContact )
{
	#ifdef DEBUG
		cout << "Old bottom contacts: ";
		for( int i = 0; i < bottomContacts.size(); i++ )
		{
			cout << bottomContacts[i] << " ";
		}
		cout << endl;
	#endif
	for( int i = 0; i < bottomContacts.size(); i++ )
	{
		if( bottomContacts[i] == contact )
		{
			bottomContacts[i] = newContact;
			checkEdges( newContact );
		}
	}
	#ifdef DEBUG
		cout << "New bottom contacts: ";
		for( int i = 0; i < bottomContacts.size(); i++ )
		{
			cout << bottomContacts[i] << " ";
		}
		cout << endl;
	#endif
}

void Net::removeTopContact( int contact )
{
	topContacts.erase( remove(topContacts.begin(), topContacts.end(), contact),
					   topContacts.end() );
}

void Net::removeBottomContact( int contact )
{
	bottomContacts.erase( remove(bottomContacts.begin(), bottomContacts.end(), 
					      contact), bottomContacts.end() );
}

void Net::setTrack( int newTrack ) 
{
	track = newTrack;
}

void Net::setNode( Node* newNode )
{
	node = newNode;
}

void Net::addZone( Zone* zone )
{
	zones.push_back( zone );
}

bool Net::isOverlap( Net* net )
{
	bool overlap = false;
	int leftEdge = net->getLeftEdge();
	int rightEdge = net->getRightEdge();
	if( (leftEdge < iRightEdge) && (iLeftEdge < rightEdge) )
		overlap = true;
	return overlap;
}

int Net::getLeftOverlapEdge( Net* net )
{
	int edge = -1;
	if( isOverlap(net) )
	{
		edge = iLeftEdge;
		int leftEdge = net->getLeftEdge();
		if( leftEdge > edge )
			edge = leftEdge;
	}
	return edge;
}

int Net::getRightOverlapEdge( Net* net )
{
	int edge = -1;
	if( isOverlap(net) )
	{
		edge = iRightEdge;
		int rightEdge = net->getRightEdge();
		if( rightEdge < edge )
			edge = rightEdge;
	}
	return edge;
}
	

int Net::intersectCountIfAbove( Net* net )
{
	int count = 0;
	if( isOverlap(net) )
	{
		int leftOverlap = getLeftOverlapEdge( net );
		int rightOverlap = getRightOverlapEdge( net );
		for( int i = 0; i < bottomContacts.size(); i++ )
		{
			int contact = bottomContacts[i];
			if( (contact >= leftOverlap) && (contact <= rightOverlap) )
				count++;
		}
		const vector<int>& topContacts = net->getTopContacts();
		for( int i = 0; i < topContacts.size(); i++ )
		{
			int contact = topContacts[i];
			if( (contact >= leftOverlap) && (contact <= rightOverlap) )
				count++;
		}
	}
	return count;
}	

int Net::intersectCountIfBelow( Net* net )
{
	int count = 0;
	if( isOverlap(net) )
	{
		int leftOverlap = getLeftOverlapEdge( net );
		int rightOverlap = getRightOverlapEdge( net );
		for( int i = 0; i < topContacts.size(); i++ )
		{
			int contact = topContacts[i];
			if( (contact >= leftOverlap) && (contact <= rightOverlap) )
				count++;
		}
		const vector<int>& bottomContacts = net->getBottomContacts();
		for( int i = 0; i < bottomContacts.size(); i++ )
		{
			int contact = bottomContacts[i];
			if( (contact >= leftOverlap) && (contact <= rightOverlap) )
				count++;
		}
	}
	return count;
}

void Net::checkLeftEdge( int contact )
{
	if( (iLeftEdge < 0) || (contact < iLeftEdge) )
		iLeftEdge = contact;
}

void Net::checkRightEdge( int contact )
{
	if( (iRightEdge < 0) || (contact > iRightEdge) )
		iRightEdge = contact;
}

void Net::checkEdges( int contact )
{
	checkLeftEdge( contact );
	checkRightEdge( contact );
	//node->checkEdge( contact );
}

bool Net::mustBeAbove( Net* net )
{
	bool mustBeAbove = false;
	const vector<int>& bottomContacts = net->getBottomContacts();
	for( vector<int>::iterator it = topContacts.begin(); 
		 (it < topContacts.end( )) && (!mustBeAbove); it++ )
	{
		int target = *it;
		vector<int>::const_iterator search = find( bottomContacts.begin(), bottomContacts.end(), 
							target );
		if( search != bottomContacts.end() )
			mustBeAbove = true;
	}
	return mustBeAbove;
}

bool Net::mustBeBelow( Net* net )
{
	#ifdef DEBUG
		cout << "Seeing if net " << getID() << " must be below ";
		cout  << net->getID() << endl;
	#endif
	bool mustBeBelow = false;
	const vector<int>& topContacts = net->getTopContacts();
	for( vector<int>::iterator it = bottomContacts.begin(); 
		 (it < bottomContacts.end( )) && (!mustBeBelow); it++ )
	{
		#ifdef DEBUG
			cout << "Bottom contact of net " << getID() << " is ";
			cout << *it << endl;
		#endif
		vector<int>::const_iterator search 
			= find( topContacts.begin(), topContacts.end(), *it );
		if( search != topContacts.end() )
		{
			mustBeBelow = true;
			#ifdef DEBUG
				cout << "Net " << getID() << " must be below Net ";
				cout << net->getID() << endl;
			#endif
		}
		#ifdef DEBUG
			else
			{
				cout << "Net " << getID() << " doesn't need to be below Net ";
				cout << net->getID() << endl;
			}
		#endif
	}
	return mustBeBelow;
}
	
			 

void Net::print( void ) const
{
	cout << "I am net " << iID << subID << ". My left edge is " << iLeftEdge;            
	cout << " and my right edge is " << iRightEdge << ".\n";
	cout << "I belong on track " << track << "." << endl; 
	cout << "My top contacts are:";
	for( int i = 0; i < topContacts.size(); i++ )
	{
		cout << topContacts[ i ] << " ";
	}
	cout << endl << "My bottom contacts are:";
	for( int i = 0; i < bottomContacts.size(); i++ )
	{
		cout << bottomContacts[ i ] << " ";
	}
	cout << "\nI am in zone(s) ";
	for( int i = 0; i < zones.size(); i++ )
	{
		cout << zones[i]->getID() << " ";
	}
	cout << endl;
}

void Net::printIDs( void ) const
{
	cout << iID << subID;
}

/*
int main( void )
{
	Net* newNet = new Net( 1 );
	newNet->addTopContact( 2 );
	newNet->addBottomContact( 3 );
	newNet->addTopContact( 4 );
	newNet->addBottomContact( 5 );
	newNet->print();
}
*/
