// Part 2 of the compression challenge at:
// http://www.reddit.com/r/dailyprogrammer/comments/25hlo9/5142014_challenge_162_intermediate_novel/

#include <boost/algorithm/string.hpp>
#include <fstream>
#include <iostream>
#include <map>
#include <stdlib.h>
#include <sstream>
#include <string>
#include <vector>

// # define TEXT "chal2_plaintext.txt"
// # define DICT "chal2_dict.txt"
// # define CHUNKS "chal2_chunks.txt"

typedef std::map<std::string, int> Dict;

// Patch the to_string method from std.
namespace patch {
  template <typename T> std::string to_string(const T& n) {
    std::ostringstream stm;
    stm << n;
    return stm.str();
  }
}

int main(int argc, const char *argv[]) {
  Dict wordlist;
  int wordcount = 0;
  std::string compressed_str;
  std::ifstream fi;
  std::ofstream fo;

  if(argc != 4) {
    std::cout << "Argument error. Should be:" << std::endl
              << "  compressor.cpp <inputfile> <output_dict> <output_chunks>" << std::endl;
    exit(1);
  }

  const char* TEXT = argv[1];
  const char* DICT = argv[2];
  const char* CHUNKS = argv[3];

  fi.open(TEXT);
  fo.open(DICT);
  if(!fi.good()) return 1;
  if(!fo.good()) return 1; // fum?

  std::cout << "Storing words in dictionary map without repetition." << std::endl;
  while(!fi.eof()) {
    std::string word, clean_word;
    bool capitalize = false;
    fi >> word;

    if(word == "\0") { compressed_str += "E"; break; }

    clean_word = boost::to_lower_copy(word);
    if(word[0] != clean_word[0]) { capitalize = true; }

    // Create key value pair and add to dictionary map.
    // Also Handle punctuation separately, and don't add to dict.
    if(word.find_first_of(".,?!;:") == -1) {
      if(!wordlist.count(clean_word)) {
        wordlist.insert(std::map<std::string, int>::value_type(clean_word, wordcount++));
        fo << clean_word << std::endl;
      }
      compressed_str += patch::to_string(wordlist.at(clean_word));
      if(capitalize) { compressed_str += "^"; }
    } else {
      std::string head = clean_word.substr(0, clean_word.length()-1);
      std::string tail = clean_word.substr(clean_word.length()-1, 1);
      if(!wordlist.count(head)) {
        wordlist.insert(std::map<std::string, int>::value_type(head, wordcount++));
        fo << head << std::endl;
      }
      compressed_str += patch::to_string(wordlist.at(head));
      if(capitalize) { compressed_str += "^"; }
      compressed_str += " ";
      compressed_str += tail;
    }
    if(fi.peek() == '\n') { compressed_str += " R"; }
    compressed_str += " ";
  }

  std::cout << "Writing Dictionary to " << DICT << std::endl;
  fo.close();

  std::cout << "Writing Compressed text of " << TEXT << " to " << CHUNKS <<  std::endl;
  fo.open(CHUNKS);
  if(!fo.good()) return 1; 
  fo << compressed_str;
  fo.close();

  std::cout << "Files written successfully. Probably." << std::endl;
  return 0;
}
