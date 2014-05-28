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

#include <algorithm>
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

// Not sure how to deep copy our map here, so displaying the modified map
//   the slower way.
void disp_path(char **map, std::vector< std::pair<int, int> > path, 
               int dimx, int dimy) {
  for(int j=0; j<dimy; j++) {
    for(int i=0; i<dimx; i++) {
      std::pair<int, int> tpos (j, i);
      if(std::find(path.begin(), path.end(), tpos) != path.end()) {
        std::cout << '*'; 
      }
      else std::cout << map[j][i];
    }
    std::cout << std::endl;
  }
}  

// Return a vector of point pairs that mark available points to move.
// Could be simplified with direction map, but four directions aren't worth it.
std::vector< std::pair<int, int> > get_open_points(char **map, 
                                   std::pair<int, int> pos, int dimx, int dimy) {
  std::vector< std::pair<int, int> > open_moves;

  // Check down
  if(pos.first < dimy-1 && map[pos.first+1][pos.second] != '#')
    open_moves.push_back(std::pair<int, int>(pos.first+1,pos.second));

  // Check right
  if(pos.second < dimx-1 && map[pos.first][pos.second+1] != '#') 
    open_moves.push_back(std::pair<int, int>(pos.first,pos.second+1));

  // Check up
  if(pos.first > 0 && map[pos.first-1][pos.second] != '#') 
    open_moves.push_back(std::pair<int, int>(pos.first-1,pos.second));

  // Check left
  if(pos.second > 0 && map[pos.first][pos.second-1] != '#') 
    open_moves.push_back(std::pair<int, int>(pos.first,pos.second-1));

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
// Start at a point, and pick directions in any order, provided that
//   we've yet to visit the point in said direction.
// If we reach the end with no available directions, return to the previous
//   point and repeat. If we exhaust all possible moves, there is no path.
std::vector< std::pair<int, int> > find_path(
    char **map, 
    std::pair<int, int> pos, 
    std::vector< std::pair<int, int> > *visited_points,
    int dimx, int dimy) {
  std::vector< std::pair<int, int> > path_points, open_points, no_points;

  if(at_bunker(map, pos, dimx, dimy)) {
    path_points.push_back(pos);
    return path_points;
  }

  open_points = get_open_points(map, pos, dimx, dimy);
  for(int i=0; i<open_points.size(); i++) {
    if (std::find(visited_points->begin(), visited_points->end(), open_points.at(i)) 
        == visited_points->end()) {
      visited_points->push_back(open_points.at(i));
      path_points = find_path(map, open_points.at(i), visited_points, dimx, dimy);
      if(!path_points.empty()){
        path_points.push_back(pos);
        return path_points;
      }
    } 
  }

  return no_points;
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

  // Starting point for pathfinding.
  std::pair<int, int> anthill;

  // Pathfinding storage.
  std::vector< std::pair<int, int> > first_path, visited_points;

  // Read file if filename arg passed in. Otherwise default to stdin later.
  if(input_file) {
    fi.open(argv[3]);
    if(!fi.good()) exit(1);
  }

  // Fill our map with symbols from stdin, or read in an input file.
  // Assuming rectangular map.
  // Also, find our starting point (*).
  for(int j=0; j<dimy; j++) {
    for(int i=0; i<dimx; i++) {
      char c;
      if(input_file) fi >> c;
      else std::cin >> c;

      if(c == '*') {
        anthill.first = j; 
        anthill.second = i;
      }

      map[j][i] = c;
    }
  }

  // Print starting map.
  std::cout << "\nMap without barricades:" << std::endl;
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

  // Print map with a single valid path found.
  visited_points.push_back(anthill);
  first_path = find_path(map, anthill, &visited_points, dimx, dimy);

  std::cout << "\nFirst path from bunker to anthill:" << std::endl;
  for(int i=0; i<first_path.size(); i++) {
    std::cout << "(" << first_path.at(i).first << 
                 "," << first_path.at(i).second << 
                 ") -> ";
  }
  std::cout << "X" << std::endl;
  disp_path(map, first_path, dimx, dimy);

  std::cout << std::endl;
  return 0;
}
