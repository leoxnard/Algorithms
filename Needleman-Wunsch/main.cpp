#include <iostream>
#include <string>
#include "Alignment.hpp"

using namespace std;

int main(int argc, const char* argv[]){
	if (argc == 3){
		string a1{};
    	string a2{};
    	string gaps1{};
    	Alignment al1(argv[1],argv[2]);
   		al1.compute(3,-1,-2);
    	al1.getAlignment(a1, gaps1, a2);
        cout << a1 << endl << gaps1 << endl << a2 << endl;
    	cout << "   score:" << al1.getScore() << endl;
	}else if(argc == 4){
		string a1{};
    	string a2{};
    	string gaps1{};
    	Alignment al1(argv[1],argv[2]);
   		al1.compute(3,-1,-2, true);
    	al1.getAlignment(a1, gaps1, a2);
        cout << a1 << endl << gaps1 << endl << a2 << endl;
    	cout << "   score:" << al1.getScore() << endl;
    }else if(argc == 6){
		string a1{};
    	string a2{};
    	string gaps1{};
    	Alignment al1(argv[1],argv[2]);
   		al1.compute(stoi(argv[3]), stoi(argv[4]), stoi(argv[5]));
    	al1.getAlignment(a1, gaps1, a2);
        cout << a1 << endl << gaps1 << endl << a2 << endl;
    	cout << "   score:" << al1.getScore() << endl;
    }else if(argc == 7){
    	string a1{};
    	string a2{};
    	string gaps1{};
    	Alignment al1(argv[1],argv[2]);
   		al1.compute(stoi(argv[3]), stoi(argv[4]), stoi(argv[5]), true);
    	al1.getAlignment(a1, gaps1, a2);
        cout << a1 << endl << gaps1 << endl << a2 << endl;
    	cout << "   score:" << al1.getScore() << endl;

	}else{
		cout << "unexpexted input" << endl;
		return 1;
	}
}
