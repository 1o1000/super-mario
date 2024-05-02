#include <fmt/ranges.h>
#include <time.h>

#include <SFML/Graphics.hpp>
#include <iostream>

#include "Mario.h"

int main() {
  auto window = sf::RenderWindow(sf::VideoMode({800, 600}), "Super Mario");

  Mario mario;
  sf::Sprite sprite, brakedSprite;

  // loading mario texture
  sf::Texture texture, bTexture;
  if (!texture.loadFromFile("textures/MarioIdle.png") ||
      !bTexture.loadFromFile("textures/MarioBrake.png"))
    fmt::println("error occured while loading textures");

  sprite.setTexture(texture);
  sprite.setScale(3.f, 3.f);

  brakedSprite.setTexture(bTexture);
  brakedSprite.setScale(3.f, 3.f);

  // absolute scale factor

  bool d_down = false;
  bool a_down = false;

  while (window.isOpen()) {
    sf::Event event = sf::Event();

    while (window.pollEvent(event))
      if (event.type == sf::Event::Closed) window.close();

    int mario_input = 0;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) mario_input -= 1;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) mario_input += 1;

    mario.Move(mario_input);
    if (mario_input == 1)
      sprite.setScale(3.f, 3.f);
    else if (mario_input == -1)
      sprite.setScale(-3.f, 3.f);

    sprite.setOrigin(texture.getSize().x / 2, 0);
    sprite.setPosition(mario.x, mario.y + 500);

    if (mario_input == 1 && mario.acceleration != 0)
      brakedSprite.setScale(3.f, 3.f);
    else if (mario_input == -1 && mario.acceleration != 0)
      brakedSprite.setScale(-3.f, 3.f);

    brakedSprite.setOrigin(bTexture.getSize().x / 2, 0);
    brakedSprite.setPosition(mario.x, mario.y + 500);

    window.clear(sf::Color(135, 206, 235));

    if (mario_input == 0 && mario.velocity != 0)
      window.draw(brakedSprite);
    else
      window.draw(sprite);

    window.display();
  }
}
