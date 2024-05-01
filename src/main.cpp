#include <fmt/ranges.h>

#include <SFML/Graphics.hpp>

#include "Mario.h"

int main() {
  auto window =
      sf::RenderWindow(sf::VideoMode({800, 600}), "Test", sf::Style::Default,
                       sf::ContextSettings(0, 0, 8));

  Mario mario;
  sf::Sprite sprite;
  sf::Sprite sprite2;

  // loading mario texture
  sf::Texture texture;
  sf::Texture bTexture;
  if (!texture.loadFromFile("textures/MarioIdle.png") ||
      !bTexture.loadFromFile("textures/MarioBrake.png"))
    fmt::println("error occured while loading image");

  sprite.setTexture(texture);
  sprite2.setTexture(bTexture);

  sprite.setScale(sf::Vector2f(3.f, 3.f));   // absolute scale factor
  sprite2.setScale(sf::Vector2f(3.f, 3.f));  // absolute scale factor

  while (window.isOpen()) {
    sf::Event event = sf::Event();
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) window.close();
      if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Q) window.close();
        if (event.key.code == sf::Keyboard::D)
          if (mario.direction != Direction::Right) mario.changeDirection();
        if (event.key.code == sf::Keyboard::A)
          if (mario.direction != Direction::Left) mario.changeDirection();
      }
      if (event.type == sf::Event::KeyReleased) mario.stop();
    }

    // move mario
    if (mario.changedDirection) mario.move();
    sprite.setPosition(mario.x, mario.y + 500);
    sprite2.setPosition(mario.x, mario.y + 500);

    // set sky color
    window.clear(sf::Color(135, 206, 235));

    // handle textures
    if (mario.changedDirection)
      window.draw(sprite2);
    else
      window.draw(sprite);

    window.display();
  }
}
