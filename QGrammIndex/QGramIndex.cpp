#include <vector>
#include <string>
#include <iostream>
#include <algorithm>

#include "QGramIndex.hpp"
#include "util.hpp"

using namespace std;

const string& QGramIndex::getText() const{return *this->text;} // member soll immer aktuell bleiben => Pointer

uint8_t QGramIndex::getQ() const{return this->q;} // länge der zu suchenden Wörter

uint32_t QGramIndex::hash(const string& qgram) const{ // |alph| = 4 (a = 00, c = 01...)
	if(qgram.size() != this->q){throw invalid_argument("Invalid q-gram. Wrong length!");}
	uint32_t hashValue{0};
	for (const char c : qgram){
		hashValue <<= 2; 			// (x << 2) == (x * 4)
		hashValue |= ordValue(c);  // (x |= y) == (x + y)
	}
	return hashValue;
}

uint32_t QGramIndex::hashNext(const uint32_t prev_hash, const char new_pos) const{
	uint32_t hashValue{prev_hash};
	unsigned mask = ~0 << 2*q; // ~ => !, 2*q => 2 Bits pro Char
	hashValue <<= 2;
	hashValue &= ~mask; // nur die 2*q Bits von rechts benötigt
	hashValue |= ordValue(new_pos); // +=
	return hashValue;
}

QGramIndex::QGramIndex(const string& text, const uint8_t q){
	// throw
	if (q<1 || q>13){throw invalid_argument("Invalid q!");}
	
	// 0. Vorbereitung
	this->text = &text; // reference!
	this->q = q;
	size_t textSize{text.size()};
	unsigned dirSize{static_cast<unsigned>(1 << (2*q))}; // == 2^q
	
	// 1. Zähler auf 0 setzen
	vector<uint32_t> dir(dirSize, 0);
	vector<uint32_t> sa(textSize-q,0);
	
	// 2. Zähle g-Grams
	uint32_t firstHashValue{this->hash(text.substr(0,q))};
	uint32_t hashValue{firstHashValue};
	for (unsigned i = 0; i<textSize-q; i++){
		dir[hashValue]++;
		hashValue = this->hashNext(hashValue, text[i+q]);
	}
	
	// 3. kommulative Summe
	for (unsigned i=1; i<dirSize; i++){
		dir[i] += dir[i-1];
	}
	
	// 4. vorkommen einsortieren
	hashValue = firstHashValue;
	for (unsigned i = 0; i<textSize-q; i++){
		dir[hashValue]--; // erste vorkommen => Index 0
		sa[dir[hashValue]]=i; // zeigt das wievielte Suffix (z.b falls 0 -> kein suffix kleiner -> index 0)
		hashValue = this->hashNext(hashValue, text[i+q]);
	}
	
	// 5. Speichern
	dir.push_back(textSize-q); // für getHits() (eigentlich +1 aber length ist bereits +1 ??)
	this->dir = dir;
	this->sa = sa;
}

vector<uint32_t> QGramIndex::getHits(const uint32_t h) const{
	//cout << dir[dir.size()-1] << endl;
	if (h>this->dir.size()-2){throw invalid_argument("Invalid hash!");} // -2 wegen Anhang
	vector<uint32_t> hits{};
	for (unsigned i = this->dir[h]; i<this->dir[h+1]; i++){ // dir[last] = textSize - q
		hits.push_back(sa[i]);
	}
	return hits;
}







