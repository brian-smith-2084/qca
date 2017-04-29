/*  File:    	zone.h 
 *
 *  Function:	Zone header 
 *
 *  Purpose:    to declare the Zone class variables and functions.
 *
 *  Author:              Brian S. Smith
 *  Date:                05/14/2004
 *
 */

#ifndef ZONE_H_
#define ZONE_H_

#include <vector>
using std::vector;

#include "net.h"

class Net;

class Zone 
{
	int iID;			//Zone ID
	int iLeftEdge;		//leftmost contact column number
	int iRightEdge;		//rightmost contact column number
	vector<Net*> vNets;	//collection of nets in this zone

	public:
		Zone( void );
		Zone( int );
		int getID( void ) const;
		int getLeftEdge( void ) const;
		int getRightEdge( void ) const;
		int getDepth() const;
		const vector<Net*>& getNets( void ) const;
		bool contains( Net* ) const;
		void setID( int );
		void setLeftEdge( int );
		void setRightEdge( int );
		void addNet( Net* );
		void addNets( vector<Net*>& );
		bool zoneEnds( vector<Net*>&, int );
		void print( void ) const;	

	private:
		bool setIncludes( Net*, vector<Net*>& );
		bool setIncludesSet( vector<Net*>&, vector<Net*>& );
};

#endif //ZONE_H_
