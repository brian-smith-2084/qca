#ifndef PATHWEIGHTS_H_
#define PATHWEIGHTS_H_

#include <iostream>
using std::cout;
using std::cin;
using std::cerr;
using std::endl;

#include <vector>
using std::vector;

#include "net.h"
#include "node.h"

void initializeSourceSink( Node*, Node*, vector<Node*>& ); 
void longestSource( Node* );
void longestSources( vector<Node*>& );
void longestSinks( vector<Node*>& );
void longestSink( Node* );
void longestPaths( Node*, Node*, vector<Node*>& );

#endif //PATHWEIGHTS_H_
