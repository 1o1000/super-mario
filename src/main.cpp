#include <fmt/ranges.h>
#include <time.h>

#include <SFML/Graphics.hpp>
#include <iostream>

#include "Mario.h"

int main() {
  auto window = sf::RenderWindow(sf::VideoMode({800, 600}), "Super Mario");

  window.setVerticalSyncEnabled(true);

  sf::Clock deltaClock;
  sf::Time dt;

  sf::View camera;
  camera.setCenter(400, 300);

  Mario mario;
  sf::Sprite sprite;

  sf::Texture texture, brakeTexture, jumpTexture;
  if (!texture.loadFromFile("textures/MarioIdle.png") ||
      !brakeTexture.loadFromFile("textures/MarioBrake.png") ||
      !jumpTexture.loadFromFile("textures/MarioJump.png"))
    fmt::println("error occured while loading textures");

  sprite.setTexture(texture);
  sprite.setScale(3.f, 3.f);
  sprite.setOrigin(texture.getSize().x / 2, 0);

  bool d_down = false;
  bool a_down = false;

  while (window.isOpen()) {
    sf::Event event = sf::Event();

    while (window.pollEvent(event))
      if (event.type == sf::Event::Closed) window.close();

    int mario_x_input = 0;
    int mario_y_input = 0;

    dt = deltaClock.getElapsedTime();
    deltaClock.restart();

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
      mario_x_input -= 1;
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
      mario_x_input += 1;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) mario_y_input -= 1;

    mario.Move(mario_x_input, dt.asSeconds());
    mario.Jump(mario_y_input, dt.asSeconds());

    if (mario_x_input == 1)
      sprite.setScale(3.f, 3.f);
    else if (mario_x_input == -1)
      sprite.setScale(-3.f, 3.f);

    camera.setSize(800, 600);
    if (camera.getCenter().x < mario.x) camera.setCenter(mario.x, 300);

    window.setView(camera);

    sprite.setPosition(mario.x, mario.y);

    if (mario.y < 500)
      sprite.setTexture(jumpTexture);
    else if (mario_x_input == 0 && mario.velocity.x != 0)
      sprite.setTexture(brakeTexture);
    else
      sprite.setTexture(texture);

    window.clear(sf::Color(135, 206, 235));

    window.draw(sprite);

    window.display();
  }
}
