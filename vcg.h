#ifndef VCG_H_
#define VCG_H_

#include <iostream>
using std::cout;
using std::cin;
using std::cerr;
using std::endl;

#include <algorithm>
#include <iterator>

#include <vector>
using std::vector;

#include "min_rand.hpp"
using boost::min_rand;

#include <time.h>

#include "net.h"
#include "node.h"
#include "pathweights.h"


class Node;

class Vcg{

	vector<Node*> vNodes;
	Node* pseudoSource;
	Node* pseudoSink;

	public:
		Vcg( void );
		Vcg( int );
		void clear( void );
		int getSize( void ) const;
		vector<Node*>& getNodes( void );
		Node* getPseudoSource( void );
		void setSize ( int );
		void initialize( int );
		void addNode( int );
		void addNode( Net* );
		void addNode( Node* );
		void removeNode( Node* );
		void splitNode( Node*, Net* parent, Net* child );
		void splitNode( Node*, vector<Node*>& );
		void addEdge( int tail, int head, int col );
		void addEdge( Node* parent, Node* child);
		void removeEdge( Node* parent, Node* child);
		void removeEdgesTemp( Node* parent );
		void removeEdgeTemp( Node* parent, Node* child );
		void addInterEdge( Node* parent, Node* child, int savings );
		void removeInterEdge( Node*, Node* );
		void removeInterEdge( EdgeKeyed* );
		void removeInterEdgeTemp( Node* parent, Node* child );
		void restoreEdges(void );
		void addPseudoSource( void );
		void addPseudoSink( void );
		void addPseudoSourceSink( void );
		void removePseudoSource( void );
		void removePseudoSink( void );
		void removePseudoSourceSink( void );
		void exchangeParents( Node* oldChild, Node* newChild );
		void exchangeChildren( Node* oldParent, Node* newParent);
		void exchangeNode( Node* oldNode, Node* newNode );
		void subNetParents( Node* oldChild, Node* newChild );
		void subNetChildren( Node* newNode, Node* oldNode );
		bool subNet( Node* );
		void applySubnetting( void );
		void initializeVcg( vector< vector<int> >& top, 
							vector< vector<int> >& bottom );
		void initializeVcg( vector< vector<Net*> >&, 
							const vector< vector<int> >& top, 
							const vector< vector<int> >& bottom );
		vector<Net*>& S( int ) const;
		void merge( Node*, Node* );
		void print( void ) const;
		bool checkCycles( void );
		bool edgeExists( Node* parent, Node* child );
		bool interEdgeExists( Node* parent, Node* child );
		bool pathExists( Node*, Node* );
		void getPath( Node* from, Node* to, vector<Node*>& );
		void processInterEdges( void );
		void setVisitedFlags( bool );
		void topologicalDFSRand( void );
		void topologicalDFS( void );
		void topological( vector<Node*>& );
		void mergePrep( void );
		bool cycleBreaker( void );

	private:
		bool checkCycles( Node*, vector<Node*>& );
		void topologicalDFS( Node*, vector<Node*>& );
		void topologicalDFSRand( Node*, vector<Node*>&, min_rand& );
		void processTopological( vector<Node*>& );
		void pathsFinder( Node*, vector< vector<EdgeKeyed*>* >&, 
						  vector<EdgeKeyed*>&, vector<Node*>& );
		vector<EdgeKeyed*>::iterator findEdgeInPath( vector<EdgeKeyed*>&, 
													 Node* );
		Node* addInterPseudoSource( void );
		bool pathContainsEdge( EdgeKeyed*, vector<EdgeKeyed*>& );
		void topological( Node*, vector<Node*>&);
		bool getPathHelper( Node* from, Node* to, vector<Node*>& );
};

extern const int CONTACTS_PER_NET;
#endif //VCG_H_
