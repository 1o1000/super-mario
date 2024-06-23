#ifndef MAP_H
#define MAP_H

#include <SFML/Graphics.hpp>
#include <vector>

#include "headers/Block.h"
#include "headers/Coin.h"
#include "headers/Goomba.h"
#include "headers/Mushroom.h"

enum MapStyle { BLUE, REGULAR };

class Map {
 public:
  std::vector<Block> blocks;
  std::vector<Goomba> goombas;
  std::vector<Mushroom> mushrooms;
  std::vector<Coin> stars;
  unsigned int tile_size = 32;
  float scale = 2.f;

  double startX = 0;
  double startY = 0;

  MapStyle mapStyle;

  Map();
  Map(std::string path);
  void LoadMap(std::string path);
  void LoadTextures();
  void SetTextures();
  Block& HasBlock(Point point);
  void SpawnMushroom(int x, int y);
  void SpawnCoin(int x, int y);

 private:
  sf::Texture brickTexture;
  sf::Texture blockTexture;
  sf::Texture block2Texture;
  sf::Texture questionTexture;
  sf::Texture castleTexture;
  sf::Texture flagTexture;
  sf::Texture poleTexture;
  sf::Texture poleHeadTexture;
  sf::Texture pipeHead1;
  sf::Texture pipeHead2;
  sf::Texture pipe1;
  sf::Texture pipe2;

  sf::Texture bBlockTexture;
  sf::Texture bBrickTexture;
  sf::Texture bBlock2Texture;
  // sf::Texture questionTexture;
  // sf::Texture castleTexture;
  // sf::Texture flagTexture;
  // sf::Texture poleTexture;
  // sf::Texture poleHeadTexture;
  // sf::Texture pipeHead1;
  // sf::Texture pipeHead2;
  // sf::Texture pipe1;
  // sf::Texture pipe2;
};

#endif