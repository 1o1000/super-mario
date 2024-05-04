#include <fmt/ranges.h>

#include <fstream>
#include <iostream>
#include <string>

#include "Block.h"
using namespace std;

void addBlock(vector<Block> *grounds, string id, int x, int y);

vector<Block> loadMap() {
  string line;
  ifstream MyReadFile("maps/map.txt");
  vector<Block> grounds = {};

  Block ground;
  int i = 0;
  int l = 0;
  int h = 0;

  while (getline(MyReadFile, line)) {
    l++;
    while (i < line.length()) {
      char a = line[i];
      if (a == '#') addBlock(&grounds, "ground", i, l - 1);
      if (a == '%') addBlock(&grounds, "block", i, l - 1);
      if (a == '+') addBlock(&grounds, "question", i, l - 1);
      if (a == '^') addBlock(&grounds, "pipeHead", i, l - 1);
      if (a == '*') addBlock(&grounds, "pipe", i, l - 1);
      i++;
    }
    i = 0;
    h = 0;
  }

  MyReadFile.close();
  return grounds;
}

void addBlock(vector<Block> *grounds, string id, int x, int y) {
  Block ground;
  ground.id = id;
  ground.x = x;
  ground.y = y;
  grounds[0].push_back(ground);
}
