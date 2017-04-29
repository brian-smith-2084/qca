//#define DEBUG true
#include <algorithm>
using std::sort;
#include "cyclebreaker.h"
#include "edgekeyed.h"
#include "node.h"
#include "vcg.h"

vector<EdgeKeyed*> keyedEdges;
	
void CycleBreaker::setEdgeKeys( vector< vector<EdgeKeyed*>* >& edgePaths )
{
	keyedEdges.clear();
	for( int i = 0; i < edgePaths.size(); i++ )
	{
		setEdgeKeys( *edgePaths[i], edgePaths );
	}	
	sort( keyedEdges.begin(), keyedEdges.end(), lessThanKeyed() );
}

void CycleBreaker::setEdgeKeys( vector<EdgeKeyed*>& edgePath,
								vector< vector<EdgeKeyed*>* >& edgePaths )
{
	for( int i = 0; i < edgePath.size(); i++ )
	{
		setEdgeKey( edgePath[i], edgePaths );
	}
}

void CycleBreaker::setEdgeKey( EdgeKeyed* edge, 
							   vector< vector<EdgeKeyed*>* >& edgePaths )
{
	vector<EdgeKeyed*>::const_iterator search = find( keyedEdges.begin(),
													  keyedEdges.end(),
													  edge                );
	if( search == keyedEdges.end() )
	{
		int key = 0;
		for( int i = 0; i < edgePaths.size(); i++ )
		{
			vector<EdgeKeyed*>::const_iterator search = 
			 find ( edgePaths[i]->begin(), edgePaths[i]->end(), edge );
			if( search != edgePaths[i]->end() )
			 	key = key + getEdgeKey( edge, *edgePaths[i] );
		}
		edge->setKey( key - edge->getWeight() );
		keyedEdges.push_back( edge );
	}
}

int CycleBreaker::getEdgeKey( EdgeKeyed* edge, vector<EdgeKeyed*>& edgePath )
{
	int key = 0;
	for(int i = 0; i < edgePath.size(); i++ )
	{
		if( edgePath[i] != edge )
		{
			key = key + edgePath[i]->getWeight();
		}
	}
	return key;
}

void CycleBreaker::breakCycles( Vcg* vcg, 
								vector< vector<EdgeKeyed*>* >& edgePaths )
{
	#ifdef DEBUG
		cout << "Entering breakCycles." << endl;
		cout << "Paths found:" << endl;
		for( int i = 0; i < edgePaths.size(); i++ )
		{
			vector<EdgeKeyed*>& edgePath = *edgePaths[i];
			for( int j = 0; j < edgePath.size(); j++ )
			{
				EdgeKeyed* edge = edgePath[j];
				cout << edge->getTail()->getID() << "->";
				cout << edge->getHead()->getID() << " ";
			}
			cout << endl;
			
		}
	#endif
	setEdgeKeys( edgePaths );
	while( !edgePaths.empty() )
	{
		#ifdef DEBUG
			cout << "EdgePaths remaining = " << edgePaths.size()<< endl;
			cout << "keyedEdges: " << endl;
			for( int j = 0; j < keyedEdges.size(); j++ )
			{
				keyedEdges[j]->print();
			}
			cout << "Target edge:" << endl;
			if( keyedEdges.size() > 0 )
				keyedEdges.back()->print();
		#endif
		for( int i = 0; i < edgePaths.size(); i++ )
		{
			vector<EdgeKeyed*>::const_iterator search = 
			 find( edgePaths[i]->begin(), edgePaths[i]->end(), 
				   keyedEdges.back() );
			if( search != edgePaths[i]->end() )
			{
				edgePaths.erase( edgePaths.begin() + i );  
				i--;
			}
			vcg->removeInterEdge( keyedEdges.back()->getTail(),
								  keyedEdges.back()->getHead() );
		}
		setEdgeKeys( edgePaths );
	}
}
