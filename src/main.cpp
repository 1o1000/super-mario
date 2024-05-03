#include <fmt/ranges.h>
#include <time.h>

#include <SFML/Graphics.hpp>
#include <iostream>

#include "Ground.h"
#include "Mario.h"

std::vector<Ground> loadMap();

int main() {
  auto window = sf::RenderWindow(sf::VideoMode({780, 480}), "Super Mario");

  window.setVerticalSyncEnabled(true);

  sf::Clock deltaClock;
  sf::Time dt;

  sf::View camera;
  camera.setCenter(window.getSize().x / 2, window.getSize().y / 2);

  const int tile_size = 48;

  std::vector<Ground> grounds = loadMap();

  Mario mario;
  sf::Sprite sprite;
  sf::Sprite brick;

  sf::Texture texture, brakeTexture, jumpTexture;
  sf::Texture brickTexture;
  if (!texture.loadFromFile("textures/MarioIdle.png") ||
      !brakeTexture.loadFromFile("textures/MarioBrake.png") ||
      !jumpTexture.loadFromFile("textures/MarioJump.png") ||
      !brickTexture.loadFromFile("textures/Brick.png"))
    fmt::println("error occured while loading textures");

  sprite.setTexture(texture);
  sprite.setScale(3.f, 3.f);
  sprite.setOrigin(texture.getSize().x / 2, texture.getSize().x);

  brick.setTexture(brickTexture);

  bool d_down = false;
  bool a_down = false;

  double ground_level = 0;

  brick.setScale(3.f, 3.f);
  for (Ground &ground : grounds) {
    ground.x = ground.x * tile_size;
    ground.y = ground.y * tile_size;
  }

  while (window.isOpen()) {
    sf::Event event = sf::Event();

    while (window.pollEvent(event))
      if (event.type == sf::Event::Closed) window.close();

    int mario_x_input = 0;
    int mario_y_input = 0;

    dt = deltaClock.getElapsedTime();
    deltaClock.restart();

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) mario_x_input -= 1;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) mario_x_input += 1;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) mario_y_input -= 1;

    mario.Move(mario_x_input, dt.asSeconds(), grounds);
    mario.Jump(mario_y_input, dt.asSeconds(), grounds);

    if (mario.velocity.x > 0)
      sprite.setScale(3.f, 3.f);
    else if (mario.velocity.x < 0)
      sprite.setScale(-3.f, 3.f);

    camera.setSize(window.getSize().x, window.getSize().y);
    if (camera.getCenter().x < mario.x)
      camera.setCenter(mario.x, window.getSize().y / 2);

    window.setView(camera);

    sprite.setPosition(mario.x, mario.y);

    if (!mario.isOnGround)
      sprite.setTexture(jumpTexture);
    else if (mario_x_input == 0 && mario.velocity.x != 0)
      sprite.setTexture(brakeTexture);
    else if (mario_x_input == 1 && mario.velocity.x < 0)
      sprite.setTexture(brakeTexture);
    else if (mario_x_input == -1 && mario.velocity.x > 0)
      sprite.setTexture(brakeTexture);
    else
      sprite.setTexture(texture);

    window.clear(sf::Color(135, 206, 235));

    for (Ground ground : grounds) {
      brick.setPosition(ground.x, ground.y);
      window.draw(brick);
    }

    window.draw(sprite);

    window.display();
  }
}
