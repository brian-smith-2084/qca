#include <vector>
using std::vector;

class EdgeKeyed;
class Vcg;

namespace CycleBreaker
{
	void setEdgeKeys( vector< vector<EdgeKeyed*>* >& ); 
	void setEdgeKeys( vector<EdgeKeyed*>&, vector< vector<EdgeKeyed*>* >& );
	void setEdgeKey( EdgeKeyed*, vector< vector<EdgeKeyed*>* >& ); 
	int getEdgeKey( EdgeKeyed*, vector<EdgeKeyed*>& ); 
	void breakCycles( Vcg* vcg, vector< vector<EdgeKeyed*>* >& );
}
