#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

typedef struct point {
  float x, y;
} Point;

typedef struct line {
  string label;
  Point p1, p2;
  bool marked;
} Line;

// Determinant: ad - bc
float det(float p1[], float p2[]) {
  return p1[0]*p2[1] - p1[1]*p2[0];
}

// Our get_intersect only checks intersections at infinite bounds.
// We must check that our intersection is within the bounds of our line segment.
bool check_intersect(Line l1, Line l2, Point i){
  if(i.x <= max(l1.p1.x, l1.p2.x) && i.x >= min(l1.p1.x, l1.p2.x) && i.y <= max(l1.p1.y, l1.p2.y) && i.y >= min(l1.p1.y, l1.p2.y)) {
    if(i.x <= max(l2.p1.x, l2.p2.x) && i.x >= min(l2.p1.x, l2.p2.x) && i.y <= max(l2.p1.y, l2.p2.y) && i.y >= min(l2.p1.y, l2.p2.y)) {
      return true;
    }
  }
  return false;
}

// Adapting method from: 
// http://en.wikipedia.org/wiki/Line%E2%80%93line_intersection
Point get_intersect(Line l1, Line l2){
  float ix = ((l1.p1.x*l1.p2.y - l1.p1.y*l1.p2.x) * (l2.p1.x-l2.p2.x) - (l1.p1.x-l1.p2.x) * (l2.p1.x*l2.p2.y - l2.p1.y*l2.p2.x))
             / ((l1.p1.x-l1.p2.x) * (l2.p1.y-l2.p2.y) - (l1.p1.y-l1.p2.y) * (l2.p1.x-l2.p2.x));
  float iy = ((l1.p1.x*l1.p2.y - l1.p1.y*l1.p2.x) * (l2.p1.y-l2.p2.y) - (l1.p1.y-l1.p2.y) * (l2.p1.x*l2.p2.y - l2.p1.y*l2.p2.x))
             / ((l1.p1.x-l1.p2.x) * (l2.p1.y-l2.p2.y) - (l1.p1.y-l1.p2.y) * (l2.p1.x-l2.p2.x));
  Point i = {ix, iy};
  return i;
}

// Test Hardcoded points intersection.
void test_intersect() {
  Point p1 = {-1, -1};
  Point p2 = {1, 1};
  Point p3 = {1, -1};
  Point p4 = {-1, 1};

  Line l1 = {"A", p1, p2};
  Line l2 = {"B", p3, p4};

  // Calculate Intersection (From Wikipedia)
  Point i = get_intersect(l1, l2);

  if(check_intersect(l1, l2, i)) {
    cout << "Found intersection at: " << i.x << ", " << i.y << endl;
  } else {
    cout << "No intersection found." << endl;
  }
}

int main(int argc, char* argv[]) {
  // Read in line file, store in line vector.
  vector<Line> lines;
  ifstream fi;

  fi.open("points.txt");
  while(!fi.eof()) {
    Line l;
    fi >> l.label;
    fi >> l.p1.x;
    fi >> l.p1.y;
    fi >> l.p2.x;
    fi >> l.p2.y;
    l.marked = false;
    lines.push_back(l);
  }

  // Check each possible line intersection.
  cout << "Intersecting lines:" << endl;
  for(int i=0; i<lines.size()-2; i++) {
    for(int j=i+1; j<lines.size()-1; j++) {
      Line l1 = lines.at(i), l2 = lines.at(j);
      Point in = get_intersect(l1, l2);
      if(check_intersect(l1, l2, in)) {
        cout << l1.label << ", " << l2.label << " at point (" << in.x << ", " << in.y << ")." << endl;
        lines.at(i).marked = true;
        lines.at(j).marked = true;
      }
    }
  }

  cout << "No Intersections:" << endl;
  for(int i=0; i<lines.size()-1; i++) {
    Line l = lines.at(i);
    if (!l.marked) {
      cout << l.label << endl;
    }
  }
  
  return 0;
}
