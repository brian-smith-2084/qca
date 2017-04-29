#ifndef NODE_H_
#define NODE_H_

#include <iostream>
using std::cout;
using std::endl;

#include <limits.h>

#include <algorithm>

#include <vector>
using std::vector;
#include "net.h"
#include "edgekeyed.h"

class Net;
class EdgeKeyed;

enum sortModes{ LEFTEDGE, PARENTS};

class Node
{
	vector<Net*> vNets;
	vector<Node*> vParents;
	vector<Node*> vChildren;
	vector<EdgeKeyed*> interParents;
	vector<EdgeKeyed*> interChildren;
	bool bIsSource;
	bool bIsSink;
	int iU;
	int iD;
	bool bVisited;
	bool bExplored;
	int iLeftEdge;
	int iRightEdge;
	int iTrack;
	static sortModes sortMode;
	static int theman;
	
	

	public:
		Node( void );
		Node( Net* );
		bool operator<(Node);
		static void setSortMode( sortModes );
		int getID( void ) const;
		vector<Net*>& getNets( void );
		vector<Node*>& getParents( void );
		int getParentCount( void ) const;
		int getChildCount( void ) const;
		vector<Node*>& getChildren( void );
		vector<EdgeKeyed*>& getInterParents( void );
		int getInterParentCount( void ) const;
		vector<EdgeKeyed*>& getInterChildren( void );
		bool isSource( void ) const;
		bool isSink( void ) const;
		int getU( void ) const;
		int getD( void ) const;
		bool isVisited( void ) const;
		bool isExplored( void ) const;
		int getLeftEdge( void ) const;
		int getRightEdge( void ) const;
		int getTrack( void ) const;
		int getContactCount( void ) const;
		bool hasParent( const Node* );
		bool hasInterParent( Node* );
		bool hasChild( Node* ) const;
		bool hasInterChild( Node* );
		bool isInPath( Node* );
		bool isInPath( vector<Node*>& );
		bool hasOverlap( Node* ) const;
		void checkEdge( int );
		void addNet( Net* );
		void addNets( vector<Net*>& );
		void addParent( Node* );
		void removeParent( const Node* );
		void exchangeParent( Node* oldParent, Node* newParent );
		void addParents( vector<Node*>& );
		void addPseudoParent( Node* );
		void addChild( Node* );
		void removeChild( const Node* );
		void exchangeChild( Node* oldChild, Node* newChild );
		void addChildren( vector<Node*>& );
		void addPseudoChild( Node* );
		void addInterParent( EdgeKeyed* );
		void addInterChild( EdgeKeyed* );
		void removeInterParent( Node* );
		void removeInterParent( EdgeKeyed* );
		void removeInterChild( Node* );
		void removeInterChild( EdgeKeyed* );
		void processInterEdges( void );
		void setSource( bool );
		void setSink( bool );
		void setU( int );
		void updateU( int );
		void updateD( int );
		void setD( int );
		void setVisited( bool );
		void setTrack( int );
		void print( void ) const;

	private:
		bool isInPathParent( vector<Node*>& );
		bool isInPathParent( Node* );
		bool isInPathChild( vector<Node*>& );
		bool isInPathChild( Node* );
		void checkEdges( Net* );
};

bool isDescendant( const Node*, Node* );
bool isAncestor( const Node*, Node* );
bool nodesIncludeNode( const vector<Node*>&, const Node* );
bool nodesIncludeNodes( const vector<Node*>&, const vector<Node*>& );

struct lessThan
{
	bool operator()( Node* base, Node* compare ) const
	{
		return( base->getLeftEdge() < compare->getLeftEdge() );
	}
};
		

struct greaterThan
{
	bool operator()( Node* base, Node* compare ) const
	{
		bool greaterThan = false;
		if( base->getParents() > compare->getParents() )
			greaterThan = true;
		else
		{
			if( base->getParents() == compare->getParents() )
			{
				if( base->getInterParents() > compare->getInterParents() )
					greaterThan = true;
			}
		}
		return greaterThan;
	}
};
#endif //NODE_H_
