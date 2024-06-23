#include <fmt/ranges.h>
#include <time.h>

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>

#include "headers/Block.h"
#include "headers/Game.h"
#include "headers/Goomba.h"
#include "headers/Map.h"
#include "headers/Mario.h"

Game game;
Mario mario;

int main() {
  auto window =
      sf::RenderWindow(sf::VideoMode({game.currentLevel.tile_size * 22,
                                      game.currentLevel.tile_size * 15}),
                       "Super Mario");
  window.setPosition(sf::Vector2i(0, 0));

  window.setVerticalSyncEnabled(true);

  game.Load("maps/map.txt");

  game.currentLevel.SetTextures();

  sf::View camera;
  int leftPos = 0;
  camera.setCenter(window.getSize().x / 2, window.getSize().y / 2);

  sf::Clock deltaClock;
  sf::Time dt;

  sf::Font font, font2;
  font.loadFromFile("fonts/notosans.ttf");
  font2.loadFromFile("fonts/font.ttf");

  sf::Text timeText{"TIME", font, 24};
  sf::Text time{"00:00", font, 24};

  sf::Text livesText{"LIVES", font, 24};
  sf::Text lives{std::to_string(mario.lives), font, 24};

  sf::Text pointsText{"POINTS", font, 24};
  sf::Text points{std::to_string(game.points), font, 24};

  sf::Text coinsText{"COINS", font, 24};
  sf::Text coins{std::to_string(game.coins), font, 24};

  sf::Text gameOverText{"GAME OVER", font2, 120};
  sf::Text stagePassedText{"STAGE PASSED", font2, 120};

  time.setFillColor(sf::Color::Black);
  timeText.setFillColor(sf::Color::Black);
  lives.setFillColor(sf::Color::Black);
  livesText.setFillColor(sf::Color::Black);
  stagePassedText.setFillColor(sf::Color::Black);
  pointsText.setFillColor(sf::Color::Black);
  points.setFillColor(sf::Color::Black);
  coinsText.setFillColor(sf::Color::Black);
  coins.setFillColor(sf::Color::Black);

  mario.x = game.currentLevel.startX;
  mario.y = game.currentLevel.startY;
  while (window.isOpen()) {
    sf::Event event = sf::Event();
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) window.close();
      if (event.type == sf::Event::KeyPressed) game.Start();
    }

    int x_input = 0;
    int y_input = 0;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) y_input -= 1;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) x_input -= 1;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) x_input += 1;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) window.close();

    dt = deltaClock.getElapsedTime();
    deltaClock.restart();

    if (mario.lives == 0 && mario.y > 620) {
      game.End();
      window.clear(sf::Color::Black);
      gameOverText.setPosition(
          camera.getCenter().x - gameOverText.getGlobalBounds().getSize().x / 2,
          camera.getCenter().y -
              gameOverText.getGlobalBounds().getSize().y / 2);
      window.draw(gameOverText);

      window.display();
      continue;
    } else if (mario.isDead && mario.y > 1000 && mario.lives > 0 &&
               mario.die.getStatus() == mario.die.Stopped) {
      mario.x = game.currentLevel.startX;
      mario.y = game.currentLevel.startY;
      game.StartTheme();
      mario.isDead = false;

      camera.setCenter(window.getSize().x / 2, window.getSize().y / 2);
    }

    time.setString(std::to_string(game.time));
    points.setString(std::to_string(game.points));
    coins.setString(std::to_string(game.coins));

    if (mario.isDead || mario.stage_passed) game.StopTheme();
    if (game.started) game.HandleFrame();
    if (mario.stage_passed &&
        mario.distance_travelled >= game.currentLevel.tile_size * 5.5) {
      game.End();
      if (mario.stageClear.getStatus() == mario.stageClear.Stopped) {
        game.Load("maps/map2.txt");

        game.currentMusic = 1;
        game.StartTheme();

        game.currentLevel.SetTextures();

        camera.setCenter(window.getSize().x / 2, window.getSize().y / 2);

        mario.distance_travelled = 0;
        mario.lives = 3;
        mario.slide_finished = false;
        mario.stage_passed = false;
      }
      stagePassedText.setPosition(
          camera.getCenter().x -
              stagePassedText.getGlobalBounds().getSize().x / 2,
          camera.getCenter().y - stagePassedText.getGlobalBounds().getSize().y);
      mario.Move(x_input, y_input, dt.asSeconds(), leftPos);
      window.draw(mario.sprite);
      window.draw(stagePassedText);

      window.display();
      continue;
    }

    mario.Move(x_input, y_input, dt.asSeconds(), leftPos);

    camera.setSize(window.getSize().x, window.getSize().y);
    if (camera.getCenter().x < mario.x &&
        (camera.getCenter().x + window.getSize().x / 2) <=
            (game.currentLevel.tile_size * 157))
      camera.setCenter(mario.x, window.getSize().y / 2);

    leftPos = camera.getCenter().x - (window.getSize().x / 2);
    timeText.setPosition(camera.getCenter().x - (window.getSize().x / 2), 0);
    time.setPosition(((camera.getCenter().x - (window.getSize().x / 2)) +
                      timeText.getGlobalBounds().getSize().x / 2) -
                         time.getGlobalBounds().getSize().x / 2,
                     timeText.getGlobalBounds().getSize().y + 10);

    livesText.setPosition(camera.getCenter().x + 100 - (window.getSize().x / 2),
                          0);

    lives.setPosition((camera.getCenter().x + 100 - (window.getSize().x / 2)) +
                          livesText.getGlobalBounds().getSize().x / 2,
                      livesText.getGlobalBounds().getSize().y + 10);

    pointsText.setPosition(
        camera.getCenter().x + 200 - (window.getSize().x / 2), 0);
    points.setPosition(
        ((camera.getCenter().x + 200 - (window.getSize().x / 2)) +
         pointsText.getGlobalBounds().getSize().x / 2) -
            points.getGlobalBounds().getSize().x / 2,
        pointsText.getGlobalBounds().getSize().y + 10);

    coinsText.setPosition(camera.getCenter().x + 320 - (window.getSize().x / 2),
                          0);
    coins.setPosition(((camera.getCenter().x + 320 - (window.getSize().x / 2)) +
                       coinsText.getGlobalBounds().getSize().x / 2) -
                          coins.getGlobalBounds().getSize().x / 2,
                      coinsText.getGlobalBounds().getSize().y + 10);

    lives.setString(std::to_string(mario.lives));

    window.setView(camera);

    window.clear(sf::Color(135, 206, 235));

    if (mario.stage_passed) game.End();

    for (Block& block : game.currentLevel.blocks) {
      block.Interact(dt.asSeconds());
      window.draw(block.sprite);
    };
    for (int i = 0; i < game.currentLevel.goombas.size();) {
      if (game.currentLevel.goombas[i].will_delete)
        game.currentLevel.goombas.erase(game.currentLevel.goombas.begin() + i);
      else
        i++;
    }
    for (Goomba& goomba : game.currentLevel.goombas) {
      goomba.Move(dt.asSeconds());

      window.draw(goomba.sprite);
    };
    for (Mushroom& mushroom : game.currentLevel.mushrooms) {
      mushroom.Move(dt.asSeconds());
      window.draw(mushroom.sprite);
    };
    for (Coin& star : game.currentLevel.stars) {
      star.Move(dt.asSeconds());
      window.draw(star.sprite);
    };

    window.draw(timeText);
    window.draw(time);
    window.draw(livesText);
    window.draw(lives);
    window.draw(pointsText);
    window.draw(points);
    window.draw(coinsText);
    window.draw(coins);

    window.draw(mario.sprite);

    window.display();
  }
}