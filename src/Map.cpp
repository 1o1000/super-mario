#include "headers/Map.h"

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "headers/Game.h"
#include "headers/Mario.h"

extern Game game;
extern Mario mario;

Map::Map() {}

Map::Map(std::string path) {
  LoadMap(path);
  LoadTextures();
}

void Map::SetTextures() {
  for (auto& block : blocks) {
    if (mapStyle == MapStyle::REGULAR) {
      if (block.id == "block") block.sprite.setTexture(blockTexture);
      if (block.id == "brick") block.sprite.setTexture(brickTexture);
      if (block.id == "block2") block.sprite.setTexture(block2Texture);
    }
    if (mapStyle == MapStyle::BLUE) {
      if (block.id == "block") block.sprite.setTexture(bBlockTexture);
      if (block.id == "brick") block.sprite.setTexture(bBrickTexture);
      if (block.id == "block2") block.sprite.setTexture(bBlock2Texture);
    }
    if (block.id == "castle") block.sprite.setTexture(castleTexture);
    if (block.id == "pole") block.sprite.setTexture(poleTexture);
    if (block.id == "flag") block.sprite.setTexture(flagTexture);
    if (block.id == "poleHead") block.sprite.setTexture(poleHeadTexture);
    if (block.id == "questionS") block.sprite.setTexture(questionTexture);
    if (block.id == "questionM") block.sprite.setTexture(questionTexture);
    if (block.id == "pipeHead1") block.sprite.setTexture(pipeHead1);
    if (block.id == "pipeHead2") block.sprite.setTexture(pipeHead2);
    if (block.id == "pipe1") block.sprite.setTexture(pipe1);
    if (block.id == "pipe2") block.sprite.setTexture(pipe2);

    block.sprite.setScale(game.currentLevel.scale, game.currentLevel.scale);
    block.sprite.setPosition(block.x, block.y);

    if (block.id == "flag")
      block.sprite.setPosition(block.x + 12, block.y - tile_size / 2);
    if (block.id == "castle")
      block.sprite.setPosition(block.x, block.y - tile_size);

    if (block.id == "castle") {
      float ratio = (tile_size * 4) / 1200;
      block.sprite.setScale(0.12f, 0.12f);
    } else
      block.sprite.setScale(game.currentLevel.scale, game.currentLevel.scale);
  }
}

void Map::LoadTextures() {
  blockTexture.loadFromFile("textures/Block.png");
  block2Texture.loadFromFile("textures/Block2.png");
  brickTexture.loadFromFile("textures/Brick.png");
  castleTexture.loadFromFile("textures/Castle.png");
  flagTexture.loadFromFile("textures/Flag.png");
  poleTexture.loadFromFile("textures/FlagPole.png");
  poleHeadTexture.loadFromFile("textures/FlagPoleHead.png");
  questionTexture.loadFromFile("textures/Question1.png");
  pipeHead1.loadFromFile("textures/PipeHead1.png");
  pipeHead2.loadFromFile("textures/PipeHead2.png");
  pipe1.loadFromFile("textures/Pipe1.png");
  pipe2.loadFromFile("textures/Pipe2.png");

  bBlockTexture.loadFromFile("textures/bBlock.png");
  bBlock2Texture.loadFromFile("textures/bBlock2.png");
  bBrickTexture.loadFromFile("textures/bBrick.png");
}

void Map::LoadMap(std::string path) {
  std::string line;
  std::ifstream fs(path);

  int i = 0;
  int l = 0;
  int id = 0;
  int flagX, flagY;

  while (getline(fs, line)) {
    if (line[0] == 'R') mapStyle = MapStyle::REGULAR;
    if (line[0] == 'B') mapStyle = MapStyle::BLUE;
    l++;
    while (i < line.length()) {
      char a = line[i];
      if (a == '#') blocks.push_back(Block{"brick", i, l - 1});
      if (a == '%') blocks.push_back(Block{"block", i, l - 1});
      if (a == '.') blocks.push_back(Block{"block2", i, l - 1});
      if (a == 'c') blocks.push_back(Block{"questionS", i, l - 1});
      if (a == 'm') blocks.push_back(Block{"questionM", i, l - 1});
      if (a == '=') blocks.push_back(Block{"poleHead", i, l - 1});
      if (a == '/') blocks.push_back(Block{"pole", i, l - 1});
      if (a == '&') blocks.push_back(Block{"castle", i, l - 1});
      if (a == '(') blocks.push_back(Block{"pipeHead1", i, l - 1});
      if (a == ')') blocks.push_back(Block{"pipeHead2", i, l - 1});
      if (a == '{') blocks.push_back(Block{"pipe1", i, l - 1});
      if (a == '}') blocks.push_back(Block{"pipe2", i, l - 1});
      if (a == '&') blocks.push_back(Block{"castle", i, l - 1});
      if (a == 'M') {
        startX = i * tile_size;
        startY = (l - 1) * tile_size;
        mario.x = game.currentLevel.startX;
        mario.y = game.currentLevel.startY;
      }
      if (a == '~') goombas.push_back(Goomba{id++, i, l - 1});
      if (a == '<') {
        flagX = i;
        flagY = l - 1;
      };
      i++;
    }
    i = 0;
  }
  blocks.push_back(Block{"flag", flagX, flagY});

  fs.close();

  for (Block& block : blocks) {
    block.x = block.x * tile_size;
    block.y = block.y * tile_size;
  }
  for (Goomba& goomba : goombas) {
    goomba.x = goomba.x * tile_size;
    goomba.y = goomba.y * tile_size;
  }
}

Point getTileLocation(double x, double y) {
  Point point;
  point.x = (int)(x / game.currentLevel.tile_size);
  point.y = (int)(y / game.currentLevel.tile_size);
  return point;
}

Block& Map::HasBlock(Point point) {
  for (Block& block : game.currentLevel.blocks) {
    Point tile = getTileLocation(block.x, block.y);
    if (block.id != "flag" && block.id != "castle")
      if (tile.x == point.x && tile.y == point.y) return block;
  }
  static auto block = Block{"", -1, -1};
  return block;
}

void Map::SpawnMushroom(int x, int y) { mushrooms.emplace_back(x, y); }
void Map::SpawnCoin(int x, int y) {
  stars.emplace_back(x, y);
  game.AddCoin();
}