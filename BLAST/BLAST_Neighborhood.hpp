#pragma once 

#include <string>
#include <vector>

#include "util.h"

struct NHResult
{
  std::string infix;
  std::vector< std::pair <std::string, int> > neighbors;
};


class BLAST_Neighborhood
{
  public:
    std::vector<NHResult> generateNeighborhood(const std::string& query,
                                               const ScoreMatrix& matrix,
                                               const int word_size,
                                               const int score_threshold,
                                               const int threads);

  
  private:
  std::vector<std::string> infixe(std::string const & query, int word_size);
  std::vector<std::string> all_variations_help(uint n, uint k);
  void all_variations(std::vector<std::string> & stVec, std::string & st, uint n, uint k);
  char toAA(unsigned x);
  
};
