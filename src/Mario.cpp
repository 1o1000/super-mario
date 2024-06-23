#include "headers/Mario.h"

#include "SFML/Audio.hpp"
#include "headers/Game.h"
#include "headers/Goomba.h"
#include "headers/Map.h"
#include "headers/Point.h"

extern Game game;

static double sign(double v) {
  if (v < -0.01) return -1;
  if (v > 0.01) return 1;
  return 0;
}

Goomba& getGoombaById(int id) {
  for (auto& goomba : game.currentLevel.goombas)
    if (goomba.id == id) return goomba;
  static Goomba goomba{-1, -1, -1};
  return goomba;
}

Mario::Mario() {
  big_mario_idle.loadFromFile("textures/BigMarioIdle.png");
  mario_idle.loadFromFile("textures/MarioIdle.png");
  mario_jump.loadFromFile("textures/MarioJump.png");
  mario_back.loadFromFile("textures/MarioBack.png");
  mario_brake.loadFromFile("textures/MarioBrake.png");
  mario_death.loadFromFile("textures/MarioDeath.png");
  mario_slide.loadFromFile("textures/MarioSlide.png");

  marioWalk.emplace_back();
  marioWalk.emplace_back();
  marioWalk.emplace_back();
  marioWalk[0].loadFromFile("textures/MarioWalk1.png");
  marioWalk[1].loadFromFile("textures/MarioWalk2.png");
  marioWalk[2].loadFromFile("textures/MarioWalk3.png");

  sprite.setScale(game.currentLevel.scale, game.currentLevel.scale);

  sprite.setOrigin(mario_idle.getSize().x / 2, mario_idle.getSize().y);

  static sf::SoundBuffer jumpBuffer, deathBuffer, stageClearBuffer;
  jumpBuffer.loadFromFile("sounds/MarioJump.wav");
  deathBuffer.loadFromFile("sounds/MarioDeath.wav");
  stageClearBuffer.loadFromFile("sounds/StageClear.wav");
  jump.setBuffer(jumpBuffer);
  die.setBuffer(deathBuffer);
  stageClear.setBuffer(stageClearBuffer);
}

void Mario::KillGoomba(int goombaId) {
  if (goombaId != -1) {
    auto& goomba = getGoombaById(goombaId);
    if (!goomba.is_dead) {
      game.AddPoints(100);
      goomba.Die();
      goomba.goombaDie.play();
    };

    if (!is_immortal) velocity.y += 3000;
  }
}

void Mario::Move(int x_input, int y_input, float dt, int leftPos) {
  if (isDead) {
    velocity.y -= 3000 * dt;
    y -= velocity.y * dt;
    SetTexture(dt, x_input);
    return;
  }
  if (y > 620 && !isDead) {
    Die();
    die.play();
  };

  std::string checkFlag =
      game.currentLevel
          .HasBlock(getTileLocation(x + game.currentLevel.tile_size / 2,
                                    y - game.currentLevel.tile_size / 2))
          .id;

  if ((checkFlag != "" && (checkFlag == "pole" || checkFlag == "poleHead")) &&
      !isSliding && !slide_finished) {
    isSliding = true;
    velocity.y = 0;
    x += 10;
    SetTexture(dt, x_input);
    stage_passed = true;
    stageClear.play();
  }
  if (isSliding && !slide_finished) {
    Slide(dt);
    SetTexture(dt, x_input);
    return;
  };

  if (slide_finished &&
      distance_travelled < game.currentLevel.tile_size * 5.5) {
    velocity.x += (acceleration / 2) * dt;
    velocity.x =
        sign(velocity.x) *
        std::min(std::abs(static_cast<double>(velocity.x)), max_x_velocity);
    x += velocity.x * dt;
    distance_travelled += velocity.x * dt;
    is_big = false;
    SetTexture(dt, x_input);
    return;
  }
  if (distance_travelled >= game.currentLevel.tile_size * 5.5) {
    velocity.x = 0;
    SetTexture(dt, x_input);
    return;
  }

  if (x_input == 1 && velocity.x > -0.5) {
    velocity.x += acceleration * dt;
  } else if (x_input == -1 && velocity.x < 0.5) {
    velocity.x -= acceleration * dt;
  } else {
    velocity.x *= 0.95;
  }

  velocity.x =
      sign(velocity.x) *
      std::min(std::abs(static_cast<double>(velocity.x)), max_x_velocity);

  if (x - game.currentLevel.tile_size / 2 - 1 > leftPos) {
    x += velocity.x * dt;
  } else {
    x += 0.05;
    velocity.x = 0;
  }

  if (game.currentLevel
          .HasBlock(getTileLocation(x + game.currentLevel.tile_size / 2,
                                    y - game.currentLevel.tile_size / 2))
          .id != "") {
    velocity.x = 0;
    x -= fmod(x, game.currentLevel.tile_size) - game.currentLevel.tile_size / 2;
  }
  if (game.currentLevel
          .HasBlock(getTileLocation(x - game.currentLevel.tile_size / 2,
                                    y - game.currentLevel.tile_size / 2))
          .id != "") {
    velocity.x = 0;
    x += fmod(x, game.currentLevel.tile_size) -
         (game.currentLevel.tile_size / 2) - 1;
  }

  auto testa = checkGoomba(x - (game.currentLevel.tile_size / 2) + 1,
                           y + (game.currentLevel.tile_size / 2) + 1);
  auto testb = checkGoomba(x + (game.currentLevel.tile_size / 2) + 1,
                           y + (game.currentLevel.tile_size / 2) + 1);
  auto testc = checkGoomba(x, y + (game.currentLevel.tile_size / 2) + 1);

  if (testa != -1 && old_y < y) KillGoomba(testa);
  if (testb != -1 && old_y < y) KillGoomba(testb);
  if (testc != -1 && old_y < y) KillGoomba(testc);

  Mushroom& rightMushroom =
      checkMushroom(x + game.currentLevel.tile_size / 2, y - 1);
  Mushroom& leftMushroom =
      checkMushroom(x - game.currentLevel.tile_size / 2, y - 1);

  if (rightMushroom.x != -1) {
    rightMushroom.Die();
    is_big = true;
  }
  if (leftMushroom.x != -1) {
    leftMushroom.Die();
    is_big = true;
  }

  int goombaR = checkGoomba(x + game.currentLevel.tile_size / 2, y - 1);
  int goombaL = checkGoomba(x - game.currentLevel.tile_size / 2, y - 1);

  if (goombaR != -1 || goombaL != -1) {
    if (!is_big) {
      Die();
      die.play();
    } else {
      is_immortal = true;
      KillGoomba(goombaL);
      KillGoomba(goombaR);
      is_immortal = false;
      is_big = false;
    }
  }

  velocity.y -= 3000 * dt;

  if (jump_duration >= .4 || y_input != -1) {
    if (fmod(y, game.currentLevel.tile_size) == 0)
      jump_duration = 0;
    else {
      is_jumping = false;
      velocity.y -= jump_strength * dt;
    }
  } else if (y_input == -1 && isOnGround && !is_jumping) {
    jump.play();
    is_jumping = true;
    velocity.y = jump_strength;
    jump_duration = 0;
  } else if (y_input == -1 && is_jumping) {
    velocity.y = jump_strength;
    jump_duration += dt;
  }

  velocity.y =
      sign(velocity.y) *
      std::min(std::abs(static_cast<double>(velocity.y)), max_y_velocity);

  isOnGround = false;
  auto& block = game.currentLevel.HasBlock(
      getTileLocation(x, y - game.currentLevel.tile_size + 1));
  if (block.id != "") {
    block.Kick();
    is_jumping = false;
    velocity.y = 0;
    velocity.y -= 3000 * dt;
  }
  old_y = y;
  y -= velocity.y * dt;
  if (game.currentLevel.HasBlock(getTileLocation(x, y + 1)).id != "") {
    isOnGround = true;
    velocity.y = 0;
    y -= fmod(y, game.currentLevel.tile_size);
  }

  if (fmod(x, game.currentLevel.tile_size) > game.currentLevel.tile_size / 2) {
    if (game.currentLevel
            .HasBlock(getTileLocation(x + game.currentLevel.tile_size, y + 1))
            .id != "") {
      isOnGround = true;
      velocity.y = 0;
      y -= fmod(y, game.currentLevel.tile_size);
    }
  }
  if (fmod(x, game.currentLevel.tile_size) < game.currentLevel.tile_size / 2) {
    if (game.currentLevel
            .HasBlock(getTileLocation(x - game.currentLevel.tile_size, y + 1))
            .id != "") {
      isOnGround = true;
      velocity.y = 0;
      y -= fmod(y, game.currentLevel.tile_size);
    }
  }

  SetTexture(dt, x_input);
}

void Mario::SetTexture(float dt, int x_input) {
  if (!is_big) {
    if (velocity.x > 0)
      sprite.setScale(game.currentLevel.scale, game.currentLevel.scale);
    else if (velocity.x < 0)
      sprite.setScale(-game.currentLevel.scale, game.currentLevel.scale);
  } else {
    if (velocity.x > 0)
      sprite.setScale(game.currentLevel.scale * 2, game.currentLevel.scale * 2);
    else if (velocity.x < 0)
      sprite.setScale(-game.currentLevel.scale * 2,
                      game.currentLevel.scale * 2);
  }
  if (isDead)
    sprite.setTexture(mario_death);
  else if (distance_travelled >= game.currentLevel.tile_size * 5.5)
    sprite.setTexture(mario_back);
  else if (isSliding)
    sprite.setTexture(mario_slide);
  else if (velocity.x != 0 && slide_finished)
    animation.animate(sprite, marioWalk, 0.2, dt);
  else if (!isOnGround)
    sprite.setTexture(mario_jump);
  else if (x_input == 1 && velocity.x < -5)
    sprite.setTexture(mario_brake);
  else if (x_input == -1 && velocity.x > 5)
    sprite.setTexture(mario_brake);
  else if ((x_input == 1 && velocity.x > 0) || x_input == -1 && velocity.x < 0)
    animation.animate(sprite, marioWalk, 0.2, dt);
  else
    sprite.setTexture(mario_idle);

  sprite.setPosition(x, y);
}

void Mario::Slide(float dt) {
  if (game.currentLevel.HasBlock(getTileLocation(x, y + 1)).id != "") {
    isOnGround = true;
    velocity.y = 0;
    y -= 0.05;
    slide_finished = true;
    isSliding = false;
  } else {
    velocity.y += 150 * dt;
    y += velocity.y * dt;
  }
}

void Mario::Die() {
  velocity.y += 6000;
  isDead = true;
  lives -= 1;
  if (is_big) is_big = false;
}