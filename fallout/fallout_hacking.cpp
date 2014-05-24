#include <algorithm>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <time.h>
#include <vector>

using namespace std;

vector<string> wordgen(int diff, int len) {
  int num_words = 4 * diff;
  int rand_base = 100;
  vector<string> wordlist;

  ifstream fi ("/usr/share/dict/words");

  srand(time(NULL));

  for(int i=0; i<num_words; i++) {
    int r = rand();
    string line;

    while(r % rand_base != 0 || line.find("'") != -1 || line.size() != len){
      r = rand();
      getline(fi, line);
    }

    wordlist.push_back(line);
  }

  random_shuffle(wordlist.begin(), wordlist.end());
  return wordlist;
}

int matching_chars(string s1, string s2) {
  int matching = 0;
  int len = min(s1.size(), s2.size());
  for(int i=0; i<len; i++){
    if(s1.at(i) == s2.at(i)) matching++;
  }
  return matching;
}

int main(int argc, char *argv[])
{
  int diff;
  int total_guesses = 4;
  string key;

  cout << "Difficulty? (1-5): ";
  cin >> diff;

  vector<string> words = wordgen(diff, diff*2 + 2);
  key = words.at(rand() % words.size());

  for(vector<string>::const_iterator i = words.begin(); i != words.end(); ++i) {
    cout << *i << endl;
  }

  for(int i=total_guesses; i>0; i--) {
    string guess;
    cout << "Guess (" << i << " left)? ";
    cin >> guess;

    if(guess == key) {
      cout << "ACCESS GRANTED! HACK THE PLANET!";
      exit(0);
    } else {
      cout << "WRONGO. " << matching_chars(key, guess) << "/" << key.size() << " correct." << endl;
    }
  }

  cout << "ACCESS DENIED. TERMINAL LOCKED." << endl;

  return 0;
}
