#ifndef DOGLEG_H_
#define DOGLEG_H_

#include <vector>
using std::vector;

struct Dogleg;
class Net;
class Vcg;

namespace Doglegs
{
	int getDoglegCrossCount( vector< vector<Net*> > );
	void subnet( Net*, vector<int>&, vector<Net*>& );
	void getEdgeColumns( Net* parent, Net* child, vector<int>& );
	void modifyNetForDogleg( Net*, int );
	void modifyColumnsForDogleg( vector<int>&, vector<int>&, 
								 vector< vector<Net*> >&, vector<int>& );
	void getColumnsToDogleg( Net* target, Net* parent, Net* child,
							 vector<int>& );
	void addDogleg( Net* target, Net* parent, Net* child, vector<int>&, 
					vector<int>&, vector< vector<Net*> >&, Vcg& );
	void removeContactCycles( vector<int>&, vector<int>&, Vcg&, 
							  vector< vector<Net*> >& );
	void setSubnetIDs( vector<Net*>& );
//	void applyDogleg( const Dogleg* dogleg, vector< vector<Net*> >& );
//	void applyDoglegs( vector< vector<Net*> >& );		
}

#endif //DOGLEG_H_
