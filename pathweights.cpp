#include "pathweights.h"
//#define DEBUG true

void initializeSourceSink( Node* source, Node* sink, 
						   vector<Node*>& nodes )
{
	source->setU( 0 );
	sink->setD( 0 );
	for( vector<Node*>::iterator it = nodes.begin(); it < nodes.end();
		 it++ )
	{
		(*it)->setU( -1 );
		(*it)->setD( - 1 );
	}
	vector<Node*> sourceChildren = source->getChildren();
	for( vector<Node*>::iterator it = sourceChildren.begin();
		 it < sourceChildren.end(); it++ )
	{
		(*it)->setU( 1 );
	}
	vector<Node*> sinkParents = sink->getParents();
	for( vector<Node*>::iterator it = sinkParents.begin();
		 it < sinkParents.end(); it++ )
	{
		(*it)->setD( 1 );
	}
}

void longestSources( vector<Node*>& nodes )
{
	for( vector<Node*>::iterator it = nodes.begin();
		 it < nodes.end(); it++ )
	{
		longestSource( *it );
	}
}


void longestSource( Node* source )
{
	int maxU = source->getU( );
	#ifdef DEBUG
		cout << "In node " << source->getID() << endl;
		cout << "Current u is " << maxU << endl;
	#endif
	vector<Node*> parents = source->getParents();
	for( vector<Node*>::iterator it  = parents.begin(); 
		 it < parents.end(); it++ )
	{
		int u = ( *it )->getU( ) + 1;
		#ifdef DEBUG
			cout << "New compared u is " << u << endl;
		#endif
		if( u > maxU )
		{
			#ifdef DEBUG
				cout << "Setting u to " << u + 1 << endl;
			#endif
			maxU = u;
		}
	}
	source->setU( maxU );
}

void longestSinks( vector<Node*>& nodes )
{
	for( vector<Node*>::iterator it = nodes.begin(); it < nodes.end(); it++ )
	{
		longestSink( *it );
	}
} 

void longestSink( Node* sink )
{
	int maxD = sink->getD( );
	#ifdef DEBUG
		cout << "In node " << sink->getID() << endl;
		cout << "Current d is " << maxD << endl;
	#endif
	vector<Node*> children = sink->getChildren();
	for( vector<Node*>::iterator it  = children.begin(); 
		 it < children.end(); it++ )
	{
		int d = ( *it )->getD( ) + 1;
		#ifdef DEBUG
			cout << "New compared d is " << d << endl;
		#endif
		if( d > maxD )
		{
			#ifdef DEBUG
				cout << "Setting d to " << d + 1 << endl;
			#endif
			maxD = d;
		}
	}
	sink->setD( maxD );
}

void longestPaths( Node* source, Node* sink, vector<Node*>& nodes )
{
	initializeSourceSink( source, sink, nodes );
	#ifdef DEBUG
		cout << "going into longestSource...\n";
		for( vector<Node*>::iterator it = nodes.begin(); it < nodes.end();
			 it++ )
		{
			cout << (*it)->getID() << " ";
		}
		cout << endl;
	#endif
	longestSources( nodes );
	reverse( nodes.begin(), nodes.end() );	
	#ifdef DEBUG
		cout << "going into longestSink...\n";
		for( vector<Node*>::iterator it = nodes.begin(); it < nodes.end();
			 it++ )
		{
			cout << (*it)->getID() << " ";
		}
		cout << endl;
	#endif
	longestSinks( nodes );
}
	
