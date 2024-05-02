#include <fmt/ranges.h>

#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>

static double sign(double v) {
  if (v < -0.01) return -1;
  if (v > 0.01) return 1;

  return 0;
}
static double lerp(double from, double to, double t) {
  return from + ((to - from) * t);
}

class Mario {
 public:
  double x = 20;
  double y;

  double maxVelocity = 10.0;
  double acceleration = 0.1;
  double velocity = 0.0;

  void Move(int input) {
    if (input == 1)
      velocity += acceleration;
    else if (input == -1)
      velocity -= acceleration;

    velocity = sign(velocity) * std::min(std::abs(velocity), maxVelocity);
    if (input == 0) velocity = lerp(velocity, 0, 0.005);

    fmt::println("Velocity: {}", velocity);
    fmt::println("Input: {}", input);

    x += velocity * 0.005f;
  }
};