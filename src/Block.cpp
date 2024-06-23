#include "headers/Block.h"

#include <fmt/ranges.h>

#include "headers/Game.h"
#include "headers/Map.h"

extern Game game;

Block::Block(std::string id, int x, int y) {
  this->id = id;
  this->x = x;
  this->y = y;
  if (id == "questionS" || id == "questionM") {
    question1.loadFromFile("textures/Question1.png");
    question2.loadFromFile("textures/Question2.png");
    question3.loadFromFile("textures/Question3.png");
    question4.loadFromFile("textures/Question4.png");
    questions.emplace_back(question1);
    questions.emplace_back(question2);
    questions.emplace_back(question3);
    questions.emplace_back(question4);
  }
}

void Block::Interact(float dt) {
  if (id == "questionS") animation.animate(sprite, questions, 0.4, dt);
  if (id == "questionM") animation.animate(sprite, questions, 0.4, dt);
  if (!spawned) return;

  if (animation_y <= y - 10) reached = true;
  if (!reached) animation_y -= 1;
  if (reached) {
    if (animation_y > y) {
      animation_y = y;
      reached = false;
      spawned = false;
    } else {
      animation_y += 1;
    }
  }

  sprite.setPosition(x, animation_y);
}
void Block::Kick() {
  spawned = true;
  if (!kicked) {
    animation_y = y;
    kicked = true;
    if (id == "questionM")
      game.currentLevel.SpawnMushroom(x, y - game.currentLevel.tile_size * 2);
    if (id == "questionS")
      game.currentLevel.SpawnCoin(x, y - game.currentLevel.tile_size * 2);
  }
}