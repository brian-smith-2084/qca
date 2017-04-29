///////////////////////////////////////////////////////////////////////////////
// String Tokenizer class
// This class is a very covenient class for manipulating tokens inside a string.
// It offers many functions for manipulating tokens.
// With all this functions,token manipulations becomes very easy!
// This code is copyrighted, author: Gonzales Cenelia
/////////////////////////////////////////////////////////////////////////
#ifndef __TOKENIZER_H__
#define __TOKENIZER_H__


#pragma warning(disable:4786)

#include <string>
#include <vector>
using std::string;
using std::vector;

typedef vector<string> vstring;




class Tokenizer {
public:
	Tokenizer() {};
	~Tokenizer() {};
	void setPosition( int pos );
	void resetPosition(void);
	void setString( const char *str );
	void setDelimiter( const char *str );
	void cleanString( char *str );
	void tokenize( vstring &v );
	int countTokens(void);
	string currentToken(void);
	string firstToken(void);
	string nextToken(void);
	string lastToken(void);
private:
	bool isDelim( char c );
	void skipDelimiters(void);
	string::iterator currentPosition;
	string thisToken;
	string delim;
	string buffer;
};


#endif


