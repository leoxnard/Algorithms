//#pragma once
#include <string>
#include <iostream>
#include <math.h>
#include <unordered_map>
#include <algorithm>

#include "Alignment.hpp"

using namespace std;

Alignment::Alignment(const std::string& seq_v, const std::string& seq_h){
    this -> seqX = seq_v;
    this -> seqY = seq_h;
    this -> sizeX = seq_v.size();
    this -> sizeY = seq_h.size();
}

void Alignment::compute(const int match, const int mismatch, const int gap, const bool local_align)
{
    if (!local_align) { //GLOBAL ALIGNMENT
        //Erstelle Matrizen
        vector<int32_t> matrix((this -> sizeX + 1) * (this -> sizeY + 1), 0); // randgaps => +1
        vector<Trace> traceVec((this -> sizeX + 1) * (this -> sizeY + 1), Trace::NONE);
        this->traceback = traceVec; // traceback in get_alignment
        
        //Befüllt die erste Zeile der Matrix
        for (uint32_t i = 1; i <= this -> sizeX; i++)
        {
            matrix[i] = i * gap;
            this->traceback[i] = Trace::HORIZONTAL;
        }

        //Befüllt die erste Spalte der Matrix
        for (uint32_t j = 1; j <= this -> sizeY ; j++)
        {
            matrix[j * (this -> sizeX + 1)] = j * gap;
            this->traceback[j * (this -> sizeX + 1)] = Trace::VERTICAL;
        }
        
        //Befüllt Matrix pro Zeile
        int32_t l{}; //speichere Werte in Umgebung
        int32_t o{};
        int32_t d{};
        for (uint32_t y=1; y <= this->sizeY; y++){
            for (uint32_t x=1; x <= this->sizeX; x++){
                l = matrix[(y*(this->sizeX+1)) + (x - 1)] + gap;
                o = matrix[(y-1)*(this->sizeX+1) + x]+gap;
                d = matrix[(y-1)*(this->sizeX+1) + x - 1];
                if (this->seqX[x-1] == this->seqY[y-1]){ // d + score Funktion
                    d += match;
                }else{
                    d += mismatch;
                }
                int32_t maxVal{max(max(l,o), d)}; // max wert
                matrix[y*(this->sizeX+1) + x] = maxVal; // befülle Eintrag
                if (maxVal==d){ // finde traceback
                    if (this->seqX[x-1]==this->seqY[y-1]) { // match
                    	this->traceback[y*(this->sizeX+1) + x] = Trace::DIAGONAL_MATCH;
                	}else { // mismatch
                		this->traceback[y*(this->sizeX+1) + x] = Trace::DIAGONAL_MISMATCH;
                	}
                }else if (maxVal == l){
                    this->traceback[y*(this->sizeX+1) + x] = Trace::HORIZONTAL;
                }else if (maxVal == o){
                    this->traceback[y*(this->sizeX+1) + x] = Trace::VERTICAL;
                }
            }
        }
        this->score = matrix[(this->sizeX+1)*(this->sizeY+1)-1];
        this->maxPos = (this->sizeX+1)*(this->sizeY+1)-1; // index von score (wichtig für local)
    }else{ //LOCAL ALIGNMENT
        //Erstelle Matrizen
        vector<int32_t> matrix((this->sizeX+1)*(this->sizeY+1), 0);
        vector<Trace> traceVec((this->sizeX+1)*(this->sizeY+1), Trace::NONE);
        this->traceback = traceVec;
        
        //Befüllt mAtrix pro Zeile
        int32_t l{};
        int32_t o{};
        int32_t d{};
        for (uint32_t y=1; y <= this->sizeY; y++){
            for (uint32_t x=1; x <= this->sizeX; x++){
                l = matrix[(y*(this->sizeX+1)) + (x - 1)] + gap;
                o = matrix[(y-1)*(this->sizeX+1) + x]+gap;
                d = matrix[(y-1)*(this->sizeX+1) + x - 1];
                if (this->seqX[x-1] == this->seqY[y-1]){
                    
                    d += match;
                }
                else{
                    d += mismatch;
                }
                int32_t maxVal{max(max(max(l,o), d), 0)};
                matrix[y*(this->sizeX+1) + x] = maxVal;
                if (maxVal==d){
                    if (this->seqX[x-1]==this->seqY[y-1]) {
                    	this->traceback[y*(this->sizeX+1) + x] = Trace::DIAGONAL_MATCH;
                	}else {
                		this->traceback[y*(this->sizeX+1) + x] = Trace::DIAGONAL_MISMATCH;
                	}
                }else if(maxVal == l){
                    this->traceback[y*(this->sizeX+1) + x] = Trace::HORIZONTAL;
                }else if (maxVal == o){
                    this->traceback[y*(this->sizeX+1) + x] = Trace::VERTICAL;
                }
            }
        }
        this->score = *std::max_element(matrix.begin(), matrix.end());
        this->maxPos = std::max_element(matrix.begin(),matrix.end()) - matrix.begin();
    }
}

int Alignment::getScore() const{
	if (this->traceback.size() == 0){
		throw std::invalid_argument("Error: compute first!");
	}else{
		return this->score;
	}
}

void Alignment::getAlignment (string& a1, string& gaps, string& a2) const{
    if (this->traceback.size() == 0){
    	throw std::invalid_argument("Error: compute first!");
    }else{
		//setze a1, a2, gap leer
    	a1="";
    	a2="";
    	gaps = "";
		//traceback von max position bis F(0,0) // F(x,y) == 0 (local)
		size_t pos{this->maxPos};
		while (this->traceback[pos] != Trace::NONE) { // Ende
		    if (this->traceback[pos] == Trace::DIAGONAL_MATCH) {
		        a1 += this->seqX[(pos%(this->sizeX+1))-1];
		        a2 += this->seqY[(pos/(this->sizeX+1))-1];
		        gaps += '|';
		        pos -= this->sizeX+2;
		    } else if (this->traceback[pos] == Trace::DIAGONAL_MISMATCH) {
		    	a1 += this->seqX[(pos%(this->sizeX+1))-1];
		        a2 += this->seqY[(pos/(this->sizeX+1))-1];
		        gaps += ' ';
		        pos -= this->sizeX+2;
		    } else if (this->traceback[pos] == Trace::VERTICAL) {
		        a1 += '-';
		        a2 += this->seqY[(pos/(this->sizeX+1))-1];
		        gaps += ' ';
		        pos -= this->sizeX+1;
		    } else {
		        a1 += this->seqX[(pos%(this->sizeX+1))-1];
		        a2 += '-';
		        gaps += ' ';
		        pos -= 1;
		    }
		}
		reverse(a1.begin(), a1.end());
		reverse(a2.begin(), a2.end());
		reverse(gaps.begin(), gaps.end());
    }
}
