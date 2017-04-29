#include "tokenizer.h"

void Tokenizer::setString( const char *str ) {
	buffer = str; // initialising buffer
	currentPosition = buffer.begin();
}

void Tokenizer::setDelimiter( const char *str ) {
	delim = str; // initialising delimiter variable
}

void Tokenizer::resetPosition(void) {
	currentPosition = buffer.begin();
}

void Tokenizer::setPosition( int pos ) {
	int thisPos = 0;
	while( thisPos <= pos && firstToken() != "" ) {
		thisPos++;
	}
}

bool Tokenizer::isDelim( char c ) {
	return delim.find(c) != string::npos;
}

void Tokenizer::skipDelimiters(void) {
	string::iterator iter = currentPosition;
	for( ; isDelim(*iter); ++iter );
	currentPosition = iter;
}

string Tokenizer::currentToken(void) {
	return thisToken;
}

string Tokenizer::firstToken(void) {
	thisToken.erase();
	skipDelimiters();
	string::iterator iter = currentPosition;
	for( ; *iter && !isDelim(*iter); ++iter ) {
		thisToken += *iter;
	}
	currentPosition = ++iter;
	return thisToken;
}

// token that comes after the first token
string Tokenizer::nextToken(void) {
	if(firstToken() != "") {
		return firstToken();
	}
	return thisToken;
}

string Tokenizer::lastToken(void) {
	string theLastToken;
	while(firstToken() != "") {
		theLastToken = thisToken;
	}
	// resets position back
	resetPosition();
	return theLastToken;
}

int Tokenizer::countTokens(void) {
	// reset position before starting to count tokens
	resetPosition();
	int tokenNumber;
	for( tokenNumber = 0; firstToken() != ""; tokenNumber++ );
	// resets position back once finished
	resetPosition(); 
	return tokenNumber;
}

// removes unwanted characters from a string
void Tokenizer::cleanString( char *str ) {
	string _str;
	vstring temp;
	tokenize(temp);
	int vec_size = temp.size();
	for( int i = 0; i < vec_size; ++i ) {
		_str += temp[i] + " ";
	}
	_str.erase( _str.length() - 1, 1 );
	strcpy(str, _str.c_str());
}

// tokenize's the string completely
// and puts the tokens into a vector
void Tokenizer::tokenize( vstring &v ) {
	// reset position before starting to tokenize
	resetPosition();
	for( ; firstToken() != ""; v.push_back(thisToken));
	resetPosition();
}




	
