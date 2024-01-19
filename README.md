# Algorithms  
Some algorithms, programmed in university.  
  
## Aho Corasick Trie  
build with `make`    
run with `./main <sequence> <query1> <...> <queryn>`  
input example: `./main GATTACA TT ATT ATT A`  
output: all hits and constructed trie `with brackets` Root(Leaf_1)(Leaf_2)`  
output example:  
```
pos 1, A  
pos 1, ATT  
pos 1, ATT  
pos 2, TT  
pos 4, A  
pos 6, A  
0(T(T))(A(T(T)))  
```
  
## BLAST  
constructs the neighborhood for the BLAST algorithm  
  
  
build with `make`  
run with `./main <SEQ> <SCORE_MATRIX> <WORD_SIZE> <THRESHOLD> <THREADS>`  
`<SEQ>` - Eine Aminosäurensequenz.  
`<SCORE_MATRIX>` - Eine Datei mit der Scoring-Matrix, z.B. Blosum62.  
`<WORD_SIZE>` - Die Länge der zu generierenden Wörter.  
`<THRESHOLD>` - Der Grenzwert für welchen ein Nachbarwort mit in die Liste über- nommen wird.  
`<THREADS>` - Die Anzahl der Threads für die parallele Verarbeitung.  
input example: `./main AAHILNMY blosum62 3 14 1`  
output: all k-mers with similar hits and position  
output example:   
```
AAH: (AAH, 16)  
AHI: (AHI, 16) (AHL, 14) (AHV, 15)  
HIL: (HII, 14) (HIL, 16) (HIM, 14) (HLL, 14) (HVL, 15)  
ILN: (ILN, 14)  
LNM: (LNM, 15)  
NMY: (NIY, 14) (NLY, 15) (NMF, 14) (NMY, 18) (NVY, 14)  
time: 0.13s
```
  
## Needleman-Wunsch  
find optimal allignment of two sequences, optional find the best local allignment  
build with `make`  
run with `./main <sequence1> <sequence2> <match=3> <mismatch=-1> <gap=-2> <optional>`  
	`<optional>`: if there is a third or sixth input, it chnages to Smith-Waterman algorithm  
input example: `./main IMISSMISSISSIPPI MYMISSISAHIPPIE 3 -4 -6`  
output: display allignment and score  
output example:   
```
IMISSMISSIS-SIPPI-  
 |   ||||||  ||||    
-M--YMISSISAHIPPIE  
score:21
```  
  
## QGrammIndex  
finds all hits in genom_file using QGrammIndex  
build with `make`  
run with `./main <genom_file> <query>`  
input example: `./main mini.text GATTACA`  
output: display query and hit-positions  
output example:   
```
GATTACA: 7 0
```
