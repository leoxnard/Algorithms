//#pragma once

#include <string>
#include <vector>

enum class Trace{
    NONE,
    VERTICAL,
    HORIZONTAL,
    DIAGONAL_MATCH,
    DIAGONAL_MISMATCH
};


class Alignment
{
public:
  Alignment() = delete;
  
  Alignment(const std::string& seq_v, const std::string& seq_h);


  void compute(const int match, const int mismatch, const int gap, const bool local_align = false);
  
  int getScore() const;
  
  void getAlignment(std::string& a1, std::string& gaps, std::string& a2) const;
  /// Output alignment into three strings.
  /// Gaps are denoted as '-' in sequences.
  /// The gap-string uses '|' (match) and ' ' (mismatch/gap).
  /// Note: all strings are equal in size
  /// e.g.
  /// a1:   "IMISSMISSIS-SIPPI-"
  /// gaps: " |   ||||||  |||| "
  /// a2:   "-M--YMISSISAHIPPIE"
  /// , where a1 corresponds to seq1, etc.

  
  
private:
    std::string seqX{};
    std::string seqY{};
    size_t sizeX{};
    size_t sizeY{};
    std::vector<Trace> traceback{};
    uint32_t score{};
    size_t maxPos{};
};
