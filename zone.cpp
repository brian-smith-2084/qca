/*  File:    zone.cpp 
 *
 *  Class:	 Zone 
 *
 *  Purpose:	To represent a Zone object, with included nets, depth, 
				leftmost, and rightmost columns.
 *
 *  Author:              Brian S. Smith
 *  Date:                05/14/2004
 *
 */
#include "zone.h"

/*
 * Zone::Zone( void )
 * 
 * This is the default constructor, setting default values to the data.
 */
Zone::Zone( void )
{
	iID = -1;
	iLeftEdge = -1;
	iRightEdge = -1;
}

Zone::Zone( int id )
{
	iID = id;
	iLeftEdge = -1;
	iRightEdge = -1;
}

/*
 * type Zone::getX( void )
 *
 * These get functions return the values in the Vertex data.
 */

int Zone::getID( void ) const
{
	return iID;
}

int Zone::getLeftEdge( void ) const
{
	return iLeftEdge;
}

int Zone::getRightEdge( void ) const
{
	return iRightEdge;
}

int Zone::getDepth( void ) const
{
	return( vNets.size() );
}

const vector<Net*>& Zone::getNets( void ) const
{
	return vNets;
}

/*
 * bool Zone::contains( int ) const
 *
 * This function determines if a certain net is contained in the Zone.
 */
bool Zone::contains( Net* testNet ) const
{
	bool bReturn = false;
	for( int i = 0; (i < vNets.size()) && (!bReturn); i++ )
	{
		if( vNets[i] == testNet )
			bReturn = true;
	}
	return bReturn;
}

void Zone::setID( int id )
{
	iID = id;
}

void Zone::setLeftEdge( int left )
{
	iLeftEdge = left;
}

void Zone::setRightEdge( int right )
{
	iRightEdge = right;
}

void Zone::addNet( Net* newNet )
{
	if( !(contains( newNet )) )
	{
		vNets.push_back( newNet );
		newNet->addZone( this );
	}
}

void Zone::addNets( vector<Net*>& nets )
{
	for( int i = 0; i < nets.size(); i++ )
	{
		addNet( nets[ i ] );
	}
}

bool Zone::setIncludes( Net* testNet, vector<Net*>& setNets )
{
	bool includes = false;
	for( int i = 0; (i < setNets.size()) && !includes; i++ )
	{
		if( testNet == setNets[ i ] )
			includes = true;
	}
	#ifdef DEBUG
		cout << "net " << testNet->getID() << " is " << includes;
		cout << endl;
		cout << "ptr = " << testNet << endl;;
	#endif
	return includes;
}

bool Zone::setIncludesSet( vector<Net*>& testNets, 
						   vector<Net*>& setNets )
{
	bool includes = true;
	for( int i = 0; (i < testNets.size()) && includes; i++ ) 
	{
		#ifdef DEBUG
			cout << "Current net in setIncludesSet = ";
			cout << testNets[i]->getID() << endl;
			cout << "ptr = " << testNets[i] << endl;
		#endif
		includes = setIncludes( testNets[i], setNets );
	}
	#ifdef DEBUG
		cout << "In setIncludesSet...\n testNets are:\n";
		for( int i = 0; i < testNets.size(); i++ )
		{
			cout << testNets[i]->getID() << " ";
		}
		cout << endl;
		cout << "setNets are:\n";	
		for( int i = 0; i < setNets.size(); i++ )
		{
			cout << setNets[i]->getID() << " ";
		}
		cout << endl;
		cout << "setIncludesSet returns " << includes << endl;
	#endif
	return includes;
}


bool Zone::zoneEnds( vector<Net*>& columnNets, int column )
{
	bool zoneEnds = true;
	if( vNets.size() < columnNets.size() )
	{
		if( setIncludesSet( vNets, columnNets ) == true )
		{
			zoneEnds = false;
			addNets( columnNets );
		}
	}
	else
	{
		if( setIncludesSet( columnNets, vNets ) == true )
			zoneEnds = false;
	}
	return zoneEnds;
}

		

/*
 * void Zone::print( void ) const
 *
 * This function prints the status and nets of the Zone.
 */
void Zone::print( void ) const
{
	cout << "I am Zone " << iID << ". My left edge is " << iLeftEdge;
	cout << " and my right edge is " << iRightEdge << ".\n";
	cout << "I have these nets:\n" ;
	for( int i = 0; i < vNets.size(); i++ )
	{
		cout << vNets[i]->getID() << " ";
	}
	cout << endl;
}

/*
 * int main( void )
 *
 * DEBUG main
 */
/*
int main ( void )
{
	Zone* z1 = new Zone();
	z1->setID( 1 );
	z1->setLeftEdge( 1 );
	z1->setRightEdge( 1 );
	z1->addNet( new Net(1) );
	z1->addNet( new Net(2) );
	Zone* z2 = new Zone();
	z2->setID( 2 );
	z2->setLeftEdge( 2 );
	z2->setRightEdge( 2 );
	z2->addNet( new Net(3) );
	z2->addNet( new Net(4) );
	z2->print();
	z1->print();
	z2->print();
}
*/
