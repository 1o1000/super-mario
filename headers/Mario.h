#ifndef MARIO_H
#define MARIO_H

#include <fmt/ranges.h>

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>

#include "headers/Animation.h"
#include "headers/Block.h"
#include "headers/Game.h"
#include "headers/Goomba.h"
#include "headers/Map.h"

Point getTileLocation(double x, double y);
Mushroom& checkMushroom(double x, double y);

class Mario {
 public:
  sf::Sprite sprite;

  double x = 240;
  double y = 0;

  int lives = 3;
  double distance_travelled = 0;
  sf::Sound stageClear;

  bool isDead = false;
  bool stage_passed = false;
  bool is_big = false;
  bool is_immortal = false;

  Mario();
  void Move(int x_input, int y_input, float dt, int leftPos);
  void Die();
  void Slide(float dt);
  void KillGoomba(int goombaId);

  void SetTexture(float dt, int input);

  Animation animation;

  std::vector<sf::Texture> marioWalk;
  sf::Texture mario_idle;
  sf::Texture big_mario_idle;
  sf::Texture mario_back;
  sf::Texture mario_jump;
  sf::Texture mario_slide;
  sf::Texture mario_brake;
  sf::Texture mario_death;

  sf::Sound jump;
  sf::Sound die;

  sf::Vector2f velocity;
  const double acceleration = 200;
  const double jump_strength = 500.0;
  const double max_x_velocity = 250.0;
  const double max_y_velocity = 800.0;

  double old_y;
  double jump_duration = 0;
  bool slide_finished = false;
  bool is_jumping = false;
  bool isSliding = false;
  bool isOnGround = true;
};

#endif