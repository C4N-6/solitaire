#include "game.h"
#include "cardStack.h"

#include <cassert>
#include <iostream>
#include <iterator>

#define NUMBER_OF_LINES_TO_DISPLAY_GAME 10
Game::Game() {
  for (int i{0}; i < 7; i++) {
    for (int j{i}; j < 7; j++) {
      m_cardStack.at(j).addClosedCard(m_deck.drawAndTake());
    }
  }
  refresh();
}
// TODO
Game::userErrors Game::move(const std::string_view from,
                            const std::string_view to) {
  return {};
}
// TODO
Game::userErrors Game::command(const std::string_view command) {
  static int s_extraLines{0};
  if (command.size() == 0) {
    s_extraLines++;
    return Game::userErrors::blank_command;
  }
  switch (command.at(0)) {
  case 'd': // draw
    draw();
    clearPreviousLines(NUMBER_OF_LINES_TO_DISPLAY_GAME + 2 * s_extraLines);
    s_extraLines = 0;
    std::cout << *this << std::endl;
    return userErrors::no_error;
  case 'c': // clear
    clearPreviousLines(NUMBER_OF_LINES_TO_DISPLAY_GAME + 2 * s_extraLines);
    s_extraLines = 0;
    std::cout << *this << std::endl;
    return userErrors::no_error;
  default:
    s_extraLines++;
    return Game::userErrors::command_not_found;
  }
}
// TODO
bool Game::isGameOver() const { return false; }

// TODO
std::string_view errorToString(const Game::userErrors error) {
  switch (error) {
  case Game::userErrors::no_error:
    return "";
  case Game::userErrors::blank_command:
    return "no command entered";
  case Game::userErrors::command_not_found:
    return "the command you entered dose not exitst";
  }
}

constexpr size_t getBigistSize(const std::array<CardStack, 7> &cardStacks) {
  size_t maxSize{std::numeric_limits<size_t>::min()};
  for (const CardStack &c : cardStacks) {
    maxSize = std::max(maxSize, c.size());
  }
  return maxSize;
}

void Game::refresh() {
  for (CardStack &stack : m_cardStack) {
    stack.refresh();
  }
}

std::ostream &operator<<(std::ostream &cout, const Game &game) {
  cout << game.m_deck << "     ";
  for (int i{0}; i < std::size(game.m_suitPiles); i++) {
    cout << game.m_suitPiles[i] << ' ';
  }

  const size_t size{getBigistSize(game.m_cardStack)};
  cout << std::endl;
  for (size_t i{0}; i < size; i++) {
    for (const CardStack &c : game.m_cardStack) {
      cout << c.at(i) << ' ';
    }
    cout << std::endl;
  }
  return cout;
}
