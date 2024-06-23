#ifndef ANIMATION_H
#define ANIMATION_H
#include <fmt/ranges.h>

#include <SFML/Graphics.hpp>
#include <vector>

class Animation {
 public:
  void animate(sf::Sprite& sprite, std::vector<sf::Texture>& textures,
               double duration, double dt) {
    time_elapsed += dt;
    if (time_elapsed >= duration) {
      if (index > textures.size() - 1) index = 0;
      sprite.setTexture(textures[index]);
      index++;
      time_elapsed = 0;
    }
  };

 private:
  int index = 0;
  double time_elapsed = 0;
};

#endif