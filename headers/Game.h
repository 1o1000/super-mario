#ifndef GAME_H
#define GAME_H

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include "headers/Map.h"

class Game {
 public:
  int points;
  int coins;

  sf::Clock clock;
  int time;

  Map currentLevel;

  std::vector<Map> maps;

  bool started;
  bool end;

  Game();
  void Load(std::string path);
  void Start();
  void End();
  void HandleFrame();
  void AddPoints(int point);
  void AddCoin();
  void StartTheme();
  void StopTheme();
  int currentMusic;

 private:
  sf::Music music;
};

#endif