#include "subnet.h"

SubNet::SubNet( void )
	: Net( -1 )
{
	subnetID = 'z';
}

SubNet::SubNet( int ID ) 
	: Net( ID )
{
}

SubNet::SubNet( const Net* net )
	: Net( net->getID() )
{
	subnetID = 'z';
}

SubNet::SubNet( char subID, const Net* net )
	: Net( net->getID() )
{
	subnetID = subID;
}

char SubNet::getSubNetID( void ) const
{
	return subnetID;
}

void SubNet::setSubNetID( char subID )
{
	subnetID = subID;
}

void SubNet::print( void ) const
{
	cout << "I am subnet " << getID() << subnetID << ". My left edge is ";
	cout << getLeftEdge() << " and my right edge is " << getRightEdge() << ".";
	cout << endl;
	cout << "I belong on track " << getTrack() << "." << endl;
	cout << "My top contacts are: ";
	for( vector<int>::const_iterator it = getTopContacts().begin(); 
		 it < getTopContacts().end(); it++ )
	{
		cout << *it << " ";
	}
	cout << endl << "My bottom contacts are: ";
	for( vector<int>::const_iterator it = getBottomContacts().begin();
		 it < getBottomContacts().end(); it++ )
	{
		cout << *it << " ";
	}
	cout << endl << "I am in zone(s) ";
	for( vector<Zone*>::const_iterator it = getZones().begin(); 
		 it < getZones().end(); it++ )
	{
		cout << (*it)->getID() << " ";
	}
	cout << endl;
}
