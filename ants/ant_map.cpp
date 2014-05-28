// Given a map, we want to find the minimum collection of points that will
// close the path between the ants and the bunker.
//
// To do this, we first find any available path between the ants (*) and the
// bunker (o). Once we have our path, we want to find the point with the
// narrowest distance to adjacent walls. Once we find this point, block it off.
// This will minimize the amount of space that ants can get through.
//
// Repeat this procedure until no more paths exist, and thus the ants are completley
// blockaded from the bunkers.
//
// Problem description with map legend: 
// http://www.reddit.com/r/dailyprogrammer/comments/26oop1/5282014_challenge_164_intermediate_part_3_protect/

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <utility>
#include <vector>

// Display rectangular representation of the map.
void disp_map(char **map, int dimx, int dimy) {
  for(int j=0; j<dimy; j++) {
    for(int i=0; i<dimx; i++) {
      std::cout << map[j][i];
    }
    std::cout << std::endl;
  }
}

// Return a vector of point pairs that mark available points to move.
// Could be simplified with direction map, but four directions aren't worth it.
std::vector< std::pair<int, int> > get_open_points(char **map, 
                                   std::pair<int, int> pos, int dimx, int dimy) {
  std::vector< std::pair<int, int> > open_moves;

  if(pos.first > 0 && map[pos.first-1][pos.second] != '#') 
    open_moves.push_back(std::pair<int, int>(pos.first-1,pos.second));
  if(pos.first < dimy-1 && map[pos.first+1][pos.second] != '#')
    open_moves.push_back(std::pair<int, int>(pos.first+1,pos.second));
  if(pos.second > 0 && map[pos.first][pos.second-1] != '#') 
    open_moves.push_back(std::pair<int, int>(pos.first,pos.second-1));
  if(pos.second < dimx-1 && map[pos.first][pos.second+1] != '#') 
    open_moves.push_back(std::pair<int, int>(pos.first,pos.second+1));

  return open_moves;
}

// Check four cardinal directions to see if we're next to a bunker (o).
//   or, on top of a bunker, but that should never happen.
bool at_bunker(char **map, std::pair<int, int> pos, int dimx, int dimy) {
  if(map[pos.first][pos.second] == 'o') return true;
  std::vector< std::pair<int, int> > open_points = get_open_points(map, pos, 
                                                                   dimx, dimy);
  for(int i=0; i<open_points.size(); i++) {
    std::pair<int, int> point = open_points.at(i);
    if(map[point.first][point.second] == 'o') {
      return true;
    }
  }

  return false;
}

// Find a path to the bunker. Not necessarily the shortest.
std::vector< std::pair<int, int> > find_path(char **map, std::pair<int, int> start, 
                                             int dimx, int dimy) {
  std::vector< std::pair<int, int> > path_points, visited_points, open_points;
  std::pair<int, int> pos = start;

  path_points.push_back(start);
  while(!at_bunker(map, pos, dimx, dimy)) {
      
  }

  return path_points;
}

int main(int argc, const char *argv[])
{
  // Arg parsing.
  bool input_file = false;
  if(argc < 3 || argc > 4) { std::cout << "Wrong args.\n  ./ant_map <dimx> <dimy> [input_file]" 
                           << std::endl;
    exit(1);
  } else if (argc == 4) {
    input_file = true; 
  }
  
  // We store our map in an n by m array of chars.
  std::ifstream fi;
  int dimx = atoi(argv[1]), dimy = atoi(argv[2]);
  char **map = new char *[dimy];
  for(int i=0; i<dimy; i++) {
    map[i] = new char[dimx];
  }

  // Read file if filename arg passed in. Otherwise default to stdin later.
  if(input_file) {
    fi.open(argv[3]);
    if(!fi.good()) exit(1);
  }

  // Fill our map with symbols from stdin, or read in an input file.
  // Assuming rectangular map.
  for(int j=0; j<dimy; j++) {
    for(int i=0; i<dimx; i++) {
      if(input_file) fi >> map[j][i];
      else std::cin >> map[j][i];
    }
  }

  // Print starting map.
  std::cout << "Map without barricades:" << std::endl;
  disp_map(map, dimx, dimy);

  // Print map with all final path points shown as 1s.
  std::cout << "\nMap showing final path points:" << std::endl;
  for(int j=0; j<dimy; j++) {
    for(int i=0; i<dimx; i++) {
      std::pair<int, int> p(j, i);
      std::cout << at_bunker(map, p, dimx, dimy);
    }
    std::cout << std::endl;
  }

  return 0;
}
