#ifndef CAN_GAME_H
#define CAN_GAME_H
#include "cardStack.h"
#include "deck.h"
#include "suitPile.h"

#include <array>
#include <cstdio>
#include <ctime>
#include <iostream>
#include <nlohmann/json.hpp>
#include <ostream>
#include <random>
#include <string_view>

struct Time {
  std::time_t m_second{};
  std::time_t m_minute{};
  std::time_t m_hours{};

  Time(std::time_t seconds) {
    m_hours = seconds / (60 * 60);
    seconds %= (60 * 60);
    m_minute = seconds / 60;
    m_second = seconds % 60;
  }
};
std::ostream &operator<<(std::ostream &cout, const Time &time);

struct Stats {
  std::string version{PROJECT_VERSION};
  seed_t seed{};
  std::time_t startTime{};
  std::time_t endTime{};
  int moveCount{0};
};

void to_json(nlohmann::json &json, const Stats &stats);
void from_json(const nlohmann::json &j, Stats &stats);
std::ostream &operator<<(std::ostream &cout, const Stats &s);

class Game {
  Deck m_deck{std::random_device{}()};
  std::array<SuitPile, 4> m_suitPiles{};
  std::array<CardStack, 7> m_cardStack{};
  Stats m_stats{.startTime = std::time(nullptr)};

public:
  enum class userErrors {
    no_error,
    blank_command,
    command_not_found,
    command_formating,
    register_not_found,
    invalid_move,
    file_not_found,
  };
  Game() : Game(std::random_device{}()) {}
  explicit Game(seed_t seed) : m_deck{seed} {
    for (int i{0}; i < 7; i++) {
      for (int j{i}; j < 7; j++) {
        m_cardStack.at(j).addClosedCard(m_deck.drawAndTake().value());
      }
      refresh();
    }
    m_stats.seed = seed;
  }
  void refresh();
  size_t longestCardStackLen() const;
  userErrors move(const std::string_view from, const std::string_view to);
  void draw() { m_deck.draw(); }
  bool isGameOver();
  bool isGameOver() const;
  bool isCardStacksEmpty() const;
  Stats getStats() const { return m_stats; }
  friend std::ostream &operator<<(std::ostream &cout, const Game &game);
  friend void to_json(nlohmann::json &j, const Game &game);
  friend void from_json(const nlohmann::json &j, Game &game);

private:
  SuitPile &getSuitPileFromChar(const char c);
};

std::string_view errorToString(const Game::userErrors error);
#endif // !CAN_GAME_H
