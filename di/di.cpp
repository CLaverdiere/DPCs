#include <boost/format.hpp>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Dice are completely random, so we get a roughly
//   even distribution here.
// A more interesting distribution would be the sums
//   of the dice, which would come out to be normally
//   distributed.

int main(int argc, const char* argv[]) {
  int max_rolls = 1e6;
  int total_rolls;
  int mag = 10;
  float dist[max_rolls];

  srand(time(NULL));

  std::cout << boost::format("%-12s %-8s %-8s %-8s %-8s %-8s %-8s\n")
               % "# of Rolls" % "1s" % "2s" % "3s" % "4s" % "5s" % "6s";

  for(total_rolls=0; total_rolls<max_rolls; total_rolls++) {
    int roll = rand() % 6;
    dist[roll]++;

    if(total_rolls == mag) {
      std::cout << boost::format("%-12d %-8.2f %-8.2f %-8.2f %-8.2f %-8.2f %-8.2f\n")
                   % mag
                   % ((dist[0] / total_rolls) * 100)
                   % ((dist[1] / total_rolls) * 100)
                   % ((dist[2] / total_rolls) * 100)
                   % ((dist[3] / total_rolls) * 100)
                   % ((dist[4] / total_rolls) * 100)
                   % ((dist[5] / total_rolls) * 100);
      mag *= 10; 
    }
  }

  return 0;
}
