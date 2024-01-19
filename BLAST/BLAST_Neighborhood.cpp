#include <vector>
#include <string>
#include <omp.h>
#include <algorithm>

#include "BLAST_Neighborhood.hpp"

using namespace std;

char BLAST_Neighborhood::toAA(unsigned x){ // index => Aminosäurekürzel
	vector<char> list{'A', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'K', 'L', 'M', 'N', 'P', 'Q', 'R', 'S', 'T', 'V', 'W', 'Y'};
	return list[x];
}

void BLAST_Neighborhood::all_variations(vector<string> & stVec, string & st, uint n, uint k){
    if (k==0){ // => länge des Wortes ist erreicht
        stVec.push_back(st);
        return;
    }
	for (unsigned i=0; i<n; i++){
	    st += toAA(i);
	    this->all_variations(stVec, st, n, k-1); // i=0 => alle wörter die mit 'A' anfangen, fehlende buchstaben k=k-1
	    st.pop_back();
	}
}

vector<string> BLAST_Neighborhood::all_variations_help(uint n, uint k){ // n=|alph|, k=|wort|
    vector<string> vecVec{};
    string vec{};
    this->all_variations(vecVec,vec,n,k); //k = fehlende buchstaben in vec (bzw string)
    return vecVec;
}

vector<string> BLAST_Neighborhood::infixe(string const & query, int word_size){ // alle Teilwörter aus query der Länge word_size
    vector<string> resultVec{};
    for (unsigned i=0; i<= query.size()-word_size; i++){
        string infix{};
        for (int j=0; j<=word_size-1; j++){
            infix +=(query[i+j]);
        }
        resultVec.push_back(infix);
    }
    return resultVec;
}

std::vector<NHResult> BLAST_Neighborhood::generateNeighborhood(const std::string& query, const ScoreMatrix& matrix, const int word_size, const int score_threshold, const int threads){
	//corner cases
	if (word_size < 1){throw invalid_argument("<WORD_SIZE> must be greater then 0!");}
	if ((int)query.size() < word_size){return {};}
	if (threads < 1){throw invalid_argument("<THRESHOLD> must be greater then 0!");}
	//nur ein Thread
    if (threads==1){ //error vermeiden (warum auch immer der kommt)
		vector<NHResult> resultVec{};
		//vector mit allen Variationen
		vector<string> variations{this->all_variations_help(20, word_size)};
		//vector mit allen infixen (in Reihenfolge)
		vector<string> infixVec{this->infixe(query, word_size)};
		//für jeden infix..
		for (unsigned i = 0; i<infixVec.size(); ++i){
			NHResult newEntry{}; //vollständige Neighbor Liste
			vector<pair<string, int>> pairVec{}; //Neighbor sammeln
			//..prüfe alle Variationen..
			for (unsigned j=0; j<variations.size(); ++j){
				int newScore{0};
				//..berechne den Score..
				for (int k=0; k<word_size; k++){
					newScore += matrix.score(infixVec[i][k],variations[j][k]); // i=infix, j=var
				}
				//..prüfe den Score auf den Threshold..
				if (newScore >= score_threshold){
					pair<string, int> newPair{variations[j],newScore};
					pairVec.push_back(newPair);
				}
			}
			//..füge ein Paar mit (Infix, (Neighbor, score; ..)) an Resultatvektor an
			newEntry.infix = infixVec[i];
			newEntry.neighbors = pairVec;
			resultVec.push_back(newEntry);
		}
		return resultVec;
    }else{
    	vector<NHResult> resultVec{};
    	vector<string> variations{this->all_variations_help(20, word_size)};
    	vector<string> infixVec{this->infixe(query, word_size)};
    	#pragma omp parallel num_threads(threads)
		{
			#pragma omp for
			for (unsigned i = 0; i<infixVec.size(); ++i){
				NHResult newEntry{};
				vector<pair<string, int>> pairVec{};
				for (unsigned j=0; j<variations.size(); ++j){
					int newScore{0};
					for (int k=0; k<word_size; k++){
						newScore += matrix.score(infixVec[i][k],variations[j][k]);
					}
					if (newScore >= score_threshold){
						pair<string, int> newPair{variations[j],newScore};
						pairVec.push_back(newPair);
					}
				}
				newEntry.infix = infixVec[i];
				newEntry.neighbors = pairVec;
				#pragma omp critical (add_critical)
				resultVec.push_back(newEntry);
			}
		}
		sort(resultVec.begin(), resultVec.end(), [& infixVec](NHResult r1, NHResult r2){return (find(infixVec.begin(), infixVec.end(), r1.infix)<find(infixVec.begin(), infixVec.end(), r2.infix));}); // Neighbors sollen sortiert sein (durch Parallelisierung durcheinander)
		return resultVec;
    }
}






















