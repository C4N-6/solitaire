#ifndef CAN_GAME_H
#define CAN_GAME_H
#include "cardStack.h"
#include "deck.h"
#include "suitPile.h"

#include <array>
#include <iostream>
#include <ostream>
#include <string_view>

class Game {
  Deck m_deck{};
  std::array<SuitPile, 4> m_suitPiles{};
  std::array<CardStack, 7> m_cardStack{};

public:
  enum class userErrors { no_error, blank_command, command_not_found };
  Game();
  void refresh();
  userErrors move(const std::string_view from, const std::string_view to);
  void draw() { m_deck.draw(); }
  userErrors command(const std::string_view command);
  bool isGameOver() const;
  friend std::ostream &operator<<(std::ostream &cout, const Game &game);
};
inline void clearPreviousLines(int lines) {
  for (int i = 0; i < lines; ++i) {
    std::cout << "\033[F\033[2K"; // Move up and clear line
  }
}

std::string_view errorToString(const Game::userErrors error);
#endif // !CAN_GAME_H
