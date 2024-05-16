#include <fmt/ranges.h>

#include "Mario.h"

class Goomba {
 public:
  double x = 0;
  double y = 400;
  int direction = 1;
  int fall = 0;

  void Move(float delta, std::vector<Block> blocks) {
    x += 32 * delta * direction;
    y += 32 * delta * fall;

    if (hasBlock(getTileLocation(x + 32, y - 16), blocks)) direction = -1;
    if (hasBlock(getTileLocation(x - 8, y - 16), blocks)) direction = 1;
    if (!hasBlock(getTileLocation(x, y + 32), blocks)) fall += 1;
  };
};