//
// Concordance problem
//

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>

using namespace std;

class WordCounter
{
public:
	int value;
	vector<int> sentences;
    WordCounter() : value( 0 ) {}
	
    void operator++ (int) { value++; }
	void push_back (int sentenceNum) { sentences.push_back( sentenceNum ); }
};

ostream& operator<<( ostream& os, WordCounter& wc )
{
	os << "{" << wc.value << ":";
	
	for ( int i=0; i < wc.sentences.size(); i++ ) {
		if ( i >= 1 ) os << ",";
		os << wc.sentences[i];
	}
	
	os << "}";
	
	return os;
}

// Remove unwanted characters from a string  
bool filter(char c)
{
    return isalpha(c) == 0;
}

int main (int argc, char * const argv[]) {
	
    if ( argc != 2 )
    {
        cout << "Usage: " << argv[0] << " filename" << endl;
        return 1;
    }
	
	// Read input file
    ifstream inFile( argv[1] );
    if ( !inFile )
    {
        cout << "Cannot open file: " << argv[1] << endl;
        return 1;
    }
	
	string s;
	map<string, WordCounter> concordance;
	int sentenceNum = 1;
	
	while ( inFile >> s )
	{
		// If token contains a ? or !, it's the end of a sentence.		
		if ( s.npos != s.find_last_of( "?!" ) )
		{
			sentenceNum++;
		}
		// If token contains a period, need special handling to consider abbreviations.
		// We must assume proper grammar and punctuation.
		// If token has one period only,
		//   has no other punctuation,
		//   and first letter not capitalized (like Mr.),
		//   it's the end of a sentence.
		else if ( s.npos != s.find_last_of( "." ) &&
				  s.npos == s.find_last_of( "(){}[]<>,;:-" ) &&
				  !isupper( s.at(0) ) )
		{
			sentenceNum++;
		}
		
		// Remove non-alpha characters from string
        s.resize( remove_if( s.begin(), s.end(), filter ) - s.begin() );
		
		// Convert token to lowercase and insert
		transform( s.begin(), s.end(), s.begin(), ::tolower );
		
		concordance[s]++;
		concordance[s].push_back( sentenceNum );
	}
	
	// Output results
    map<string, WordCounter>::iterator it;
	
	for ( it = concordance.begin(); it != concordance.end(); it++ ) {
		cout << it->first << "\t" << it->second << endl;
	}
	
    return 0;
}
