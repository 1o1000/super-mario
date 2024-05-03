#include <fmt/ranges.h>

#include <fstream>
#include <iostream>
#include <string>

#include "Ground.h"
using namespace std;

void addGround(vector<Ground> *grounds, int x, int y);

vector<Ground> loadMap() {
  string line;
  ifstream MyReadFile("maps/map.txt");
  vector<Ground> grounds = {};

  Ground ground;
  int i = 0;
  int l = 0;
  int h = 0;

  while (getline(MyReadFile, line)) {
    l++;
    while (i < line.length()) {
      char a = line[i];
      if (a == '#') addGround(&grounds, i, l - 1);
      i++;
    }
    i = 0;
    h = 0;
  }

  MyReadFile.close();
  return grounds;
}

void addGround(vector<Ground> *grounds, int x, int y) {
  Ground ground;
  ground.x = x;
  ground.y = y;
  grounds[0].push_back(ground);
}
