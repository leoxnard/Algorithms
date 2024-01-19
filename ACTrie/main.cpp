#include <string>
#include <iostream>
#include <vector>


#include "ACTrie.hpp"

using namespace std;

void printHits(const vector<Hit>& vec, const vector<string>& needles){
    for (Hit h : vec){
    	cout << "pos " << h.pos << ", " << needles[h.index] << "\n";
    }
}

int main(int argc, const char* argv[]){
	if (argc < 3){throw invalid_argument{"Nicht genügend Eingaben!"};}
	vector<Hit> hitList{};
	vector<string> needles{};
	for (size_t i = 2; i<argc; ++i){needles.push_back(argv[i]);}
	ACTrie neu(needles);
	neu.setQuery(argv[1]);
	bool b{true};
	cout << "Hits (position in query, pattern) :\n";
	while (b){
		b=neu.next(hitList);
		printHits(hitList, needles);
	}
	cout << neu.getTree() << endl;
}


