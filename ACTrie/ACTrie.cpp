#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <unordered_map>

#include "ACTrie.hpp"

using namespace std;

void ACTrie::setQuery(const string& haystack){
	this->haystack_ = haystack;
	currentTrieNode_ = 0;
	currentHaystackPos_ = 0;
}

void ACTrie::createLinks(const size_t nodePos, const char currentChar) {
	size_t currentNode{nodes_[nodePos].parentLink};
	//SuffixLink
	if (nodes_[nodePos].depth < 2){ //current node = parent Node von anfrage
		nodes_[nodePos].suffixLink = 0;
	}else{
		while (1){
			currentNode = nodes_[currentNode].suffixLink; // current node = folge suffixLink
			if (nodes_[currentNode].children.find(currentChar) != nodes_[currentNode].children.end()){ //Prüfe auf match
				nodes_[nodePos].suffixLink = nodes_[currentNode].children[currentChar];
				break;
			}else{
				if (currentNode == 0){ //prüfe auf Wurzel
					nodes_[nodePos].suffixLink = 0;
					break;
				}
			}
		}
	}
	//OutputLink
	currentNode = nodes_[nodePos].suffixLink;
	while (1) {
		if (currentNode == 0){ //Suffixlink ist Wurzel
			break;
		}else if (nodes_[currentNode].needleIndeces.size() > 0){ //Suffixlink hat Hit
			nodes_[nodePos].outputLink = currentNode;
			break;
		}else{ 												// Suffixlink hat keinen Hit
			currentNode = nodes_[currentNode].suffixLink;	// gehe zum nächsten Suffixlink
		}
	}
	// Rekursion (Breitensuche)
	for (const auto& [character, index] : nodes_[nodePos].children) { //rekursiv alle Kinder
	    createLinks(index, character);
	}
	
}

ACTrie::ACTrie(const vector<string>& needles){
	if (needles.empty()){throw logic_error{"Es muss mindestens ein Pattern gegeben sein!"};}
	this->needles_ = needles;
	vector<ACNode> nodes{{0,0,0,{},{},0}}; // Wurzel
	size_t currentPos{0};  // NodePos
	for (size_t s = 0; s<needles.size(); ++s){ //für jeden Needle
		for (size_t c = 0; c<needles[s].size(); ++c){//Für jeden Buchstaben aus Needle
			if (needles[s][c]<65 || needles[s][c]>90){throw logic_error{"nur Großbuchstaben sind zulässig!"};} // Nur Großbuchstaben
			if (nodes[currentPos].children.find(needles[s][c]) != nodes[currentPos].children.end()){ //Wenn Knoten existiert
				if (c == needles[s].size()-1){ //Wenn letzter Buchstabe
					nodes[nodes[currentPos].children[needles[s][c]]].needleIndeces.push_back(s); //Needle Index hinzufügen
					currentPos = 0;
				}else{currentPos = nodes[currentPos].children[needles[s][c]];}
			}else{
				ACNode newNode;//Erstelle neuen Knoten
				newNode.parentLink = currentPos;//parenLink angeben
				newNode.outputLink = 0;
				newNode.depth = nodes[currentPos].depth + 1;// depth +1
				nodes[currentPos].children[needles[s][c]] = nodes.size(); //Node als children von parentLink hinzufügen
				if (c == needles[s].size()-1){ //falls letzter Buchstabe
					currentPos = 0; //seitze currenponter auf Wurzel
					newNode.needleIndeces.push_back(s); //markiere newNode als Needlehitter
				}else{currentPos = nodes.size();} //seitzte Currentpos auf Position des neuen Knoten}
				nodes.push_back(newNode);//Füge neuen Knoten dem Knotenvektor an
			}
		}
	}
	this->nodes_ = nodes;
	createLinks(0, '0');
}

bool ACTrie::next(vector<Hit>& hits){
	hits = {};
	size_t outputPos{};
	if (haystack_.empty()){return false;}
	while(1){
		if (currentHaystackPos_ >= haystack_.size()){return false;}
		//currentHaystackPos_++;
		while (1){
			if (nodes_[currentTrieNode_].children.find(haystack_[currentHaystackPos_]) != nodes_[currentTrieNode_].children.end()){ //Knoten mit C aus text existiert
				currentTrieNode_ = nodes_[currentTrieNode_].children[haystack_[currentHaystackPos_]];
				break;
			}else if (currentTrieNode_ == 0){ 	//An wurzel ohne match
				break;
			}else{ //nicht wurzel ohne match -> folge Suffixlink
				currentTrieNode_ = nodes_[currentTrieNode_].suffixLink;
			}
		}
		if (!nodes_[currentTrieNode_].needleIndeces.empty()){ //Hits existieren
			for (size_t needleIndex : nodes_[currentTrieNode_].needleIndeces){
				Hit newHit(needleIndex, currentHaystackPos_+1-nodes_[currentTrieNode_].depth);
				hits.push_back(newHit);
			}
		}
		outputPos = currentTrieNode_;
		while (1){ //prüfe auf alle outputs der outputs
			outputPos = nodes_[outputPos].outputLink; //gehe zu outputLink
			if (outputPos == 0){break;} //break falls Wurzel
			for (size_t needleIndex : nodes_[outputPos].needleIndeces){ //füge Hit hinzu
				Hit newHit(needleIndex, currentHaystackPos_+1-nodes_[outputPos].depth);
				hits.push_back(newHit);
			}
		}
		currentHaystackPos_++;
		if (!hits.empty()){return true;}
	}
}


string ACTrie::getTree() const{
	string treeString{};
	this->getTreeHelp(treeString, 0, '0');
	return treeString;
}

void ACTrie::getTreeHelp(string& treeString, const size_t pos, const char character) const{ // pos = current Position in Trie, character =  Char des Nodes
	treeString += character; //++character
	vector<pair<size_t, char>> all_children{}; //suche nach allen Kindern
	for (const auto &[c, i] : nodes_[pos].children){
		all_children.push_back({i,c});
	}
	sort(all_children.begin(), all_children.end(), [](const pair<size_t, char>& p1, const pair<size_t, char>& p2){return (p1.first < p2.first);}); //sortiere nach position im vector = index
	for (const auto &[i, c] : all_children){ //für alle kinder das Selbe => Tiefensuche
		treeString += '(';
		this -> getTreeHelp(treeString, i, c);
		treeString += ')';
	}
}



























