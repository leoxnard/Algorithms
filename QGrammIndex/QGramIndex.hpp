#pragma once

#include <vector>
#include <string>

class QGramIndex 
{
public:
    QGramIndex(const std::string& text, const uint8_t q);

    /**
      @brief Returns the text.
      
      @return the full text
    */
    const std::string& getText() const;
    
    /**
      @brief Returns a vector of indices into text, where the q-gram with hash @p h occurs.
      
      The vector might be empty, if the q-gram does not exist, i.e. empty range within the suffix array.
      The hash value must be within [0...|SIGMA|^q), otherwise an exception is thrown.
      
      @note The indices are in reverse order of occurrence in the text, or might be even "unordered"
      when OpenMP multithreading is used.    
      
      @param h Hash value of the q-gram
      @return Vector of hits (indices into text)
      @throws std::invalid_argument("Invalid hash!"); if h is outside of valid hash values
    */
    std::vector<uint32_t> getHits(const uint32_t h) const;
    
    /**
      @brief Get the length of q-grams, i.e. 'q'.
    */
    uint8_t getQ() const;
    
    /**
      @brief Compute a full hash for a given q-gram.
      @param qgram A q-gram; must have size 'q'
      @return hash value of @p qgram
      @throws std::invalid_argument("Invalid q-gram. Wrong length!"); if qgram.size() != q
      
    */
    uint32_t hash(const std::string& qgram) const;
    
    /**
       @brief Returns the next rolling hash for given new character and previous hash (of previous q-gram).       
       @param prev_hash Previous hash value
       @param new_pos new character (last position of current q-gram)
       @return The updated hash value
    */
    uint32_t hashNext(const uint32_t prev_hash, const char new_pos) const;
    
    std::vector<uint32_t> get_sa(){
    	return this->sa;
    }
    
    std::vector<uint32_t> get_dir(){
    	return this->dir;
    }

private:
    const std::string* text;
    uint q{};
	std::vector<uint32_t> sa{};
	std::vector<uint32_t> dir{};
};























