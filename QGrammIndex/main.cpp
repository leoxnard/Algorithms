#include <string>
#include <string.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

#include "QGramIndex.hpp"
#include "util.hpp"

using namespace std;

template <typename T>
void printVec(vector<T> const & vec){
	if (vec.size()==0){cout << "{}" << endl; return;}
   cout << "{";
   for (size_t i=0; i<vec.size()-1; i++){
       cout << vec[i] << ", ";
   }
   cout << vec[vec.size()-1] << "}\n";
}

uint32_t hash_main(const string& qgram) {
	uint32_t hashValue{0};
	for (const char c : qgram){
		hashValue <<= 2; 				// (x << 2) == (x * 4)
		hashValue |= ordValue(c);  // (x |= y) == (x + y)
	}
	return hashValue;
}

int main(int argc, const char* argv[]){
	if (argc!=3){throw invalid_argument("falsche Anzahl an Argumenten! (./aufgabe5_main <GENOM_FILE> <QUERY>)");}
	uint8_t q = static_cast<uint8_t>(strlen(argv[2]));
	
	ifstream input_file(argv[1]);
	if (!input_file.good()){throw invalid_argument("Genomfile existiert nicht!");}
	stringstream buffer;
	buffer << input_file.rdbuf();
	string input_str = buffer.str();
	
	
	QGramIndex class1(input_str,q);
	
	vector<uint32_t> hits{class1.getHits(hash_main(argv[2]))};
	
	cout << argv[2] << ": ";
	for (uint32_t elem : hits){
		cout << elem << " ";
	}
	cout << '\n';
}
