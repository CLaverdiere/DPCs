// A program to decompress some arbitrary data from this challenge:
// http://www.reddit.com/r/dailyprogrammer/comments/25clki/5122014_challenge_162_easy_novel_compression_pt_1/
//
// In reality, a way for me to learn some c++ parsing for my Graphics class
//   next semester.

#include <boost/algorithm/string.hpp>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>

// #define WORDSIZE 50
// #define DICTSIZE 50 // lolz

// # define DICT "chal1_dict.txt"
// # define CHUNKS "chal1_chunks.txt"

using namespace std;

int main(int argc, const char *argv[])
{
  vector<string> wordlist;
  string output_text;
  int words_read = 0;

  if(argc != 4) {
    std::cout << "Argument error. Should be:" << std::endl
              << "  decompressor.cpp <input_dict> <input_chunks> <output_text>" << std::endl;
    exit(1);
  }

  const char* DICT = argv[1];
  const char* CHUNKS = argv[2];
  const char* TEXT = argv[3];

  cout << "Reading dictionary file into vector." << endl;
  ifstream fi;
  ofstream fo;

  fi.open(DICT);
  if(!fi.good()) return 1;
  while(!fi.eof())
  {
    string temp;
    fi >> temp;
    wordlist.push_back(temp);
  }
  fi.close();

  // cout << "Printing dictionary file." << endl;
  // for(vector<string>::const_iterator i = wordlist.begin(); i != wordlist.end(); ++i){
  //   cout << " > " << *i << endl;
  // }

  cout << "Parsing chunks." << endl;
  fi.open(CHUNKS);
  if(!fi.good()) return 1;

  int no_space = true;
  while(!fi.eof())
  {
    string s;
    fi >> s;

    if(s == "E") break;
    else if(s == "R") { output_text += "\n"; no_space = true; }
    else if(s.find_first_of(".,?!;:") == 0) { output_text += s; }
    else if(s == "-") { output_text += "-"; no_space = true; }
    else // Assume it's a number.
    {
      int i = atoi(s.c_str()); // convert from string to int for indexing.
      string word = wordlist.at(i);

      if(s.find("!") != -1) { word = boost::to_upper_copy(word); }
      else if(s.find("^") != -1) { word[0] = toupper(word[0]); }

      if(!no_space) output_text += " ";
      output_text += word;
      no_space = false;
    }
  }

  cout << "Writing decompressed text to " << TEXT << endl;
  fo.open(TEXT);
  if(!fo.good()) return 1;
  fo << output_text << endl;
  fo.close();

  return 0;
}
