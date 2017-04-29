/*  File:	Net.h
 */

#ifndef NET_H_
#define NET_H_

#include <iostream>
using std::cout;
using std::endl;

#include <limits.h>

#include <vector>
using std::vector;

#include "node.h"
#include "zone.h"

class Node;
class Zone;

class Net{

	int iID;
	char subID;
	int iLeftEdge;
	int iRightEdge;
	vector<int> topContacts;
	vector<int> bottomContacts;
	int track;
	Node* node;
	vector<Zone*> zones;	

	public:
		Net( int );
		Net( int id, int leftEdge);
		int getID( void ) const;
		char getSubID( void ) const;
		int getLeftEdge( void ) const;
		int getRightEdge( void ) const;
		const vector<int>& getTopContacts( void ) const;
		vector<int>& getTopContacts( void );
		const vector<int>& getBottomContacts( void ) const;
		vector<int>& getBottomContacts( void );
		int getTopContactCount( void ) const;
		int getBottomContactCount( void ) const;
		bool hasTopContact( int ) const;
		bool hasBottomContact( int ) const;
		int getTrack( void ) const;
		Node* getNode( void );
		const vector<Zone*>& getZones( void ) const;
		bool contains( int ) const;
		bool isColumnBetweenEdges( int ) const;
		void setID( int );
		void setSubID( char );
		void setLeftEdge( int );
		void setRightEdge( int );
		void addTopContact( int );
		void addTopContacts( vector<int>& );
		void addBottomContact( int );
		void addBottomContacts( vector<int>& );
		void modifyTopContact( int, int );
		void modifyBottomContact( int, int );
		void removeTopContact( int );
		void removeBottomContact( int );
		void setTrack( int );
		void setNode( Node* );
		void addZone( Zone* );
		bool isOverlap( Net* );
		int getLeftOverlapEdge( Net* );
		int getRightOverlapEdge( Net* );
		int intersectCountIfAbove( Net* );
		int intersectCountIfBelow( Net* );
		bool mustBeAbove( Net* );
		bool mustBeBelow( Net* );
		void print( void ) const;
		void printIDs( void ) const;
		

	private:
		void checkLeftEdge( int );
		void checkRightEdge( int );
		void checkEdges( int );
};

const int CONTACTS_PER_NET = 2;

#endif //NET_H_
