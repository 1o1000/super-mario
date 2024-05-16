#include <fmt/ranges.h>
#include <time.h>

#include <SFML/Graphics.hpp>
#include <iostream>

#include "Block.h"
#include "Goomba.h"

std::vector<Block> loadMap();

// TODO: Get Animated Texture From Single Image
int main() {
  auto window = sf::RenderWindow(sf::VideoMode({780, 480}), "Super Mario");

  window.setVerticalSyncEnabled(true);

  sf::Clock deltaClock;
  sf::Time dt;
  sf::View camera;
  camera.setCenter(window.getSize().x / 2, window.getSize().y / 2);

  int leftPos = 0;
  const int tile_size = 48;

  std::vector<Block> grounds = loadMap();

  Mario mario;
  Goomba goomba;

  sf::Sprite marioSprite;
  sf::Sprite goombaSprite;
  sf::Sprite brick, block, question;
  sf::Sprite pipe, pipeHead;

  std::vector<sf::Texture> goombaTex = {};
  sf::Texture goombaWalk1, goombaWalk2;
  if (!goombaWalk1.loadFromFile("textures/GoombaWalk1.png") ||
      !goombaWalk2.loadFromFile("textures/GoombaWalk2.png"))
    fmt::println("error occured while loading textures");

  goombaTex.push_back(goombaWalk1);
  goombaTex.push_back(goombaWalk2);

  std::vector<sf::Texture> marioWalk = {};
  sf::Texture walk1, walk2, walk3;
  if (!walk1.loadFromFile("textures/MarioWalk1.png") ||
      !walk2.loadFromFile("textures/MarioWalk2.png") ||
      !walk3.loadFromFile("textures/MarioWalk3.png"))
    fmt::println("error occured while loading textures");
  marioWalk.push_back(walk1);
  marioWalk.push_back(walk2);
  marioWalk.push_back(walk3);

  std::vector<sf::Texture> questionTex = {};
  sf::Texture question1, question2, question3;
  if (!question1.loadFromFile("textures/Question1.png") ||
      !question2.loadFromFile("textures/Question2.png") ||
      !question3.loadFromFile("textures/Question3.png"))
    fmt::println("error occured while loading textures");

  question.setTexture(question1);
  questionTex.push_back(question1);
  questionTex.push_back(question2);
  questionTex.push_back(question3);

  sf::Texture pipeHeadTexture, pipeTexture;
  if (!pipeHeadTexture.loadFromFile("textures/PipeHead.png") ||
      !pipeTexture.loadFromFile("textures/Pipe.png"))
    fmt::println("error occured while loading textures");
  pipe.setTexture(pipeTexture);
  pipeHead.setTexture(pipeHeadTexture);

  sf::Texture texture, brakeTexture, jumpTexture;
  sf::Texture brickTexture, blockTexture;
  if (!texture.loadFromFile("textures/MarioIdle.png") ||
      !brakeTexture.loadFromFile("textures/MarioBrake.png") ||
      !jumpTexture.loadFromFile("textures/MarioJump.png") ||
      !blockTexture.loadFromFile("textures/Block.png") ||
      !brickTexture.loadFromFile("textures/Brick.png"))
    fmt::println("error occured while loading textures");

  marioSprite.setTexture(texture);
  marioSprite.setScale(3.f, 3.f);
  marioSprite.setOrigin(texture.getSize().x / 2, texture.getSize().x);

  goombaSprite.setTexture(goombaWalk1);

  brick.setTexture(brickTexture);
  block.setTexture(blockTexture);

  brick.setScale(3.f, 3.f);
  block.setScale(3.f, 3.f);
  pipe.setScale(3.f, 3.f);
  pipeHead.setScale(3.f, 3.f);
  question.setScale(3.f, 3.f);
  for (Block &ground : grounds) {
    ground.x = ground.x * tile_size;
    ground.y = ground.y * tile_size;
  }

  double duration = 0;
  double qDuration = 0;
  double gDuration = 0;

  int mIndex = 0;
  int qIndex = 0;
  int gIndex = 0;

  goombaSprite.setScale(2.f, 2.f);

  while (window.isOpen()) {
    sf::Event event = sf::Event();

    while (window.pollEvent(event))
      if (event.type == sf::Event::Closed) window.close();

    int mario_x_input = 0;
    int mario_y_input = 0;

    dt = deltaClock.getElapsedTime();
    deltaClock.restart();
    duration += dt.asSeconds();

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) mario_x_input -= 1;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) mario_x_input += 1;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) mario_y_input -= 1;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) window.close();
    // if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) window.close();

    mario.HorizontalMove(mario_x_input, dt.asSeconds(), grounds, leftPos);
    mario.VerticalMove(mario_y_input, dt.asSeconds(), grounds);

    goomba.Move(dt.asSeconds(), grounds);

    camera.setSize(window.getSize().x, window.getSize().y);
    if (camera.getCenter().x < mario.x)
      camera.setCenter(mario.x, window.getSize().y / 2);
    leftPos = camera.getCenter().x - (window.getSize().x / 2);
    window.setView(camera);

    if (mario.velocity.x > 0)
      marioSprite.setScale(3.f, 3.f);
    else if (mario.velocity.x < 0)
      marioSprite.setScale(-3.f, 3.f);

    marioSprite.setPosition(mario.x, mario.y);
    goombaSprite.setPosition(goomba.x, goomba.y);

    if (!mario.isOnGround)
      marioSprite.setTexture(jumpTexture);
    else if (mario_x_input == 1 && mario.velocity.x < 0)
      marioSprite.setTexture(brakeTexture);
    else if (mario_x_input == -1 && mario.velocity.x > 0)
      marioSprite.setTexture(brakeTexture);
    else if ((mario_x_input == 1 && mario.velocity.x > 0) ||
             mario_x_input == -1 && mario.velocity.x < 0) {
      duration += dt.asSeconds();
      if (duration >= 0.4) {
        if (mIndex < 3) {
          marioSprite.setTexture(marioWalk[mIndex]);
          mIndex++;
          duration = 0;
        } else
          mIndex = 0;
      }
    } else
      marioSprite.setTexture(texture);

    window.clear(sf::Color(135, 206, 235));

    qDuration += dt.asSeconds();
    if (qDuration >= 0.4) {
      if (qIndex < 3) {
        question.setTexture(questionTex[qIndex]);
        qIndex++;
        qDuration = 0;
      } else
        qIndex = 0;
    }

    gDuration += dt.asSeconds();
    if (gDuration >= 0.4) {
      if (gIndex <= 1) {
        goombaSprite.setTexture(goombaTex[gIndex]);
        gIndex++;
        gDuration = 0;
      } else
        gIndex = 0;
    }

    for (Block ground : grounds) {
      if (ground.id == "ground") brick.setPosition(ground.x, ground.y);
      if (ground.id == "block") block.setPosition(ground.x, ground.y);
      if (ground.id == "question") question.setPosition(ground.x, ground.y);
      window.draw(brick);
      window.draw(block);
      window.draw(question);
    }

    window.draw(marioSprite);
    window.draw(goombaSprite);

    window.display();
  }
}