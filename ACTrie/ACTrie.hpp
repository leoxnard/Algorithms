#pragma once

#include <string>
#include <vector>
#include <limits>
#include <unordered_map>

struct Hit
{
  Hit() = default;
  Hit(size_t i, size_t p) : index(i), pos(p) {};
  size_t index;
  size_t pos;
  bool operator==(const Hit& rhs) const
  {
    return index==rhs.index && pos==rhs.pos;
  }
};

class Index
{
public:
  Index() = default;
  Index(size_t index, char c){
  	this->i_ = index;
  	this->c_ = c;
  };
  Index(size_t index) : i_(index) {};
  bool isNull() const { return i_ == std::numeric_limits<size_t>::max(); }
  size_t pos() const { return i_; }
  size_t& pos() { return i_; }
  
  char c() const{ return c_; }
private:
  size_t i_ = std::numeric_limits<size_t>::max();
  char c_{};
};

struct ACNode{
	size_t suffixLink{};
	size_t outputLink{};
	size_t parentLink{};
	std::unordered_map<char, size_t> children{};
	std::vector<size_t> needleIndeces{};
	size_t depth{};
};



class ACTrie
{
public:
  ACTrie(const std::vector<std::string>& needles);
  void setQuery(const std::string& haystack);
  bool next(std::vector<Hit>& hits);
  std::string getTree() const;
  /// E.g. given the AC tree of 'ga' and 'c'
  ///       0
  ///     /   \
  ///    g     c
  ///   /    
  ///  a     
  /// Output: "0(c)(g(a))"

private:
  void createLinks(const size_t nodePos, const char currentChar);
  std::string haystack_{};
  std::vector<std::string> needles_{};
  size_t currentTrieNode_{};
  size_t currentHaystackPos_{};
  std::vector<ACNode> nodes_{};
  void getTreeHelp(std::string& treeString, const size_t pos, const char character) const;
};



