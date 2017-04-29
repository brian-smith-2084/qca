/*  File:	Net.h
 */

#ifndef SUBNET_H_
#define SUBNET_H_

#include <iostream>
using std::cout;
using std::endl;

#include "net.h"


class SubNet : public Net
{
	char subnetID;

	public:
		SubNet( void );
		SubNet( int );
		SubNet( const Net* );
		SubNet( char, const Net* );
		char getSubNetID( void ) const;
		void setSubNetID( char );
		void print( void ) const;
};
#endif //SUBNET_H_
