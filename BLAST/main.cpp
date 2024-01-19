#include <iostream>
#include <vector>
#include <string>
#include <omp.h>

#include "BLAST_Neighborhood.hpp"

using namespace std;

template <typename T>
void printVec(vector<T> const & vec){
    if (vec.size()==0){
        cout << "{}" << endl;
    }else{
        cout << "{";
        for (size_t i=0; i<vec.size()-1; i++){
            cout << vec[i] << ", ";
        }
        cout << vec[vec.size()-1] << "}\n";
    }
}

void printPairVec(vector<NHResult> const & vec){
	if (vec.size()!=0){
        for (NHResult result : vec){
            cout << result.infix << ": ";
            for (pair<string, int> p : result.neighbors){
            	cout << "(" << p.first << ", " << p.second << ") ";
            }
            cout << endl;
        }
    }
}


int main(int argc, const char* argv[]){
	if (argc!=6){throw invalid_argument("falsche Anzahl an Argumenten!");}
	ScoreMatrix matrix;
	matrix.load(argv[2]);
	BLAST_Neighborhood neu;
	double timeStart{omp_get_wtime()};
	vector<NHResult> vec(neu.generateNeighborhood(argv[1], matrix, stoi(argv[3]), stoi(argv[4]), stoi(argv[5])));
	double timeStop{omp_get_wtime()};
	printPairVec(vec);
	cout << "time: " << timeStop-timeStart << "s" << endl;
}
