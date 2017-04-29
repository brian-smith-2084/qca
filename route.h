/*  Source file:          
 *
 *  Function:             
 *
 *  Purpose:  
 *
 *  Author:              Brian S. Smith
 *  Date:                2004
 *
 */
#ifndef ROUTE_H_
#define ROUTE_H_
#include <fstream>		//Necessary for File I/O
using std::ifstream;

#include <iostream>		//Necessary for screen output
using std::cout;
using std::cin;
using std::cerr;
using std::endl;

#include <time.h>		//Necessary for Run Time calculation

#include <math.h>

#include <limits.h>

#include <string>       
using std::string;

#include <vector>
using std::vector;

#include <list>
using std::list;

#include <stdlib.h>

#include <algorithm>
using std::sort;
using std::max;


#include "net.h"
#include "node.h"
#include "vcg.h"
#include "zone.h"


//#define DEBUG true
#define MAX_ROWS 2
#define LINELENGTH 1000

#define CINF 50

vector< vector<int> > top_rows;
vector< vector<int> > bottom_rows;
vector<Zone*> vZones;
vector< vector<Net*> > vNets;
Vcg vcg;

void printContacts( void );
void readFromFile( const string& );
void initializeZones( vector<int>& top_row, vector<int>& bottom_row );
vector<Node*>& getTerminatingNodes( const Zone* );
vector<Node*>& getStartingNodes( const Zone* );
int getM( const vector<Node*>&, const int );
int getN( const vector<Node*>&, Node*, const int );
void printNodes( const vector<Node*>& );
void mergePQ( void );
void leftEdgeFirst( void );
int getTrackCount( void );
int getIntersectionCount( void );
void addIntersectionEdgesbyZone( void );
void addIntersectionEdges( void );
void addIntersectionEdgesLong( void );
void initializeNets( void );
void topologicalPruning( void );
bool verifyTracks( vector<int>& top_row, vector<int>& bottom_row );
void printNets( void );

#endif //ROUTE_H_
