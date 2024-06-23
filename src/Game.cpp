#include "headers/Game.h"

#include <fmt/ranges.h>

#include <iostream>

Game::Game(){};

void Game::Load(std::string path) {
  currentLevel = Map{path};
  currentMusic = 0;
  music.openFromFile("sounds/theme.mp3");
  music.setVolume(10);
};

void Game::AddPoints(int point) { points += point; }
void Game::AddCoin() { coins++; }
void Game::HandleFrame() {
  if (end) return;
  time = (int)clock.getElapsedTime().asSeconds();
}
void Game::Start() {
  if (!started) {
    clock.restart();
    started = true;
    StartTheme();
  }
}
void Game::End() {
  end = true;
  StopTheme();
}
void Game::StartTheme() { music.play(); }
void Game::StopTheme() { music.stop(); }