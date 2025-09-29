#include "game.h"
#include "card.h"
#include "cardStack.h"
#include "suitPile.h"

#include <algorithm>
#include <cassert>
#include <cctype>
#include <cstddef>
#include <iostream>
#include <iterator>
#include <limits>
#include <optional>
#include <string_view>
#include <vector>

#define NUMBER_OF_LINES_PRINTED_TO_DISPLAY_GAME 3 + longestCardStackLen()

Game::Game() {
  for (int i{0}; i < 7; i++) {
    for (int j{i}; j < 7; j++) {
      m_cardStack.at(j).addClosedCard(m_deck.drawAndTake().value());
    }
  }
  refresh();
}
size_t Game::longestCardStackLen() const {
  size_t max{std::numeric_limits<size_t>::min()};
  for (const CardStack &cardStack : m_cardStack) {
    max = std::max(max, cardStack.size());
  }
  return max;
}
bool charIsAReg(const char c) {
  switch (c) {
  case '1':
  case '2':
  case '3':
  case '4':
  case '5':
  case '6':
  case '7':
  case 'd':
  case 'w':
  case 'e':
  case 'r':
  case 't':
    return true;
  default:
    return false;
  }
}

Game::userErrors Game::move(const std::string_view from,
                            const std::string_view to) {
  if (!charIsAReg(from.at(0)) || !charIsAReg(to.at(0))) {
    return userErrors::register_not_found;
  }

  // find the cards that we can move
  std::vector<Card> movingCard{};
  if (from.at(0) == 'd') {
    std::optional<Card> tempCard{m_deck.peek()};
    if (tempCard.has_value()) {
      movingCard.push_back(tempCard.value());
    } else {
      return userErrors::invalid_move;
    }
  } else if (from.at(0) == 'w') {
    std::optional<Card> tempCard{m_suitPiles.at(0).peek()};
    if (tempCard.has_value()) {
      movingCard.push_back(tempCard.value());
    } else {
      return userErrors::invalid_move;
    }
  } else if (from.at(0) == 'e') {
    std::optional<Card> tempCard{m_suitPiles.at(1).peek()};
    if (tempCard.has_value()) {
      movingCard.push_back(tempCard.value());
    } else {
      return userErrors::invalid_move;
    }
  } else if (from.at(0) == 'r') {
    std::optional<Card> tempCard{m_suitPiles.at(2).peek()};
    if (tempCard.has_value()) {
      movingCard.push_back(tempCard.value());
    } else {
      return userErrors::invalid_move;
    }
  } else if (from.at(0) == 't') {
    std::optional<Card> tempCard{m_suitPiles.at(3).peek()};
    if (tempCard.has_value()) {
      movingCard.push_back(tempCard.value());
    } else {
      return userErrors::invalid_move;
    }
  } else if (from.at(0) >= '1' && from.at(0) <= '7') {
    CardStack &cardStack{m_cardStack.at(from.at(0) - '1')};
    movingCard = cardStack.peekOpenCards(cardStack.noOfOpenCards());
  }

  // find if we can move those cards
  if (movingCard.empty()) {
    return userErrors::invalid_move;
  } else if (to.at(0) >= '1' && to.at(0) <= '7') {
    CardStack &cardStack{m_cardStack.at(to.at(0) - '1')};
    if (!cardStack.canAddOpenCard(movingCard.at(0))) {
      return userErrors::invalid_move;
    }
    cardStack.addOpenCards(movingCard);
  } else if (movingCard.size() != 1) {
    return userErrors::invalid_move;
  } else if (to.at(0) == 'w') {
    if (!m_suitPiles.at(0).canAdd(movingCard.at(0))) {
      return userErrors::invalid_move;
    }
    m_suitPiles.at(0).add(movingCard.at(0));
  } else if (to.at(0) == 'e') {
    if (!m_suitPiles.at(1).canAdd(movingCard.at(0))) {
      return userErrors::invalid_move;
    }
    m_suitPiles.at(1).add(movingCard.at(0));
  } else if (to.at(0) == 'r') {
    if (!m_suitPiles.at(2).canAdd(movingCard.at(0))) {
      return userErrors::invalid_move;
    }
    m_suitPiles.at(2).add(movingCard.at(0));
  } else if (to.at(0) == 't') {
    if (!m_suitPiles.at(3).canAdd(movingCard.at(0))) {
      return userErrors::invalid_move;
    }
    m_suitPiles.at(3).add(movingCard.at(0));
  }

  // TODO: Game::move(const std::string_view, const std::string_view) remove the
  // moved cards
  if (from.at(0) == 'd') {
    m_deck.take();
  } else if (from.at(0) == 'w') {
    m_suitPiles.at(0).pop();
  } else if (from.at(0) == 'e') {
    m_suitPiles.at(1).pop();
  } else if (from.at(0) == 'r') {
    m_suitPiles.at(2).pop();
  } else if (from.at(0) == 't') {
    m_suitPiles.at(3).pop();
  } else if (from.at(0) >= '1' && from.at(0) <= '7') {
    CardStack &cardStack{m_cardStack.at(from.at(0) - '1')};
    cardStack.takeOpenCards(cardStack.noOfOpenCards());
  } else {
    return userErrors::register_not_found;
  }

  refresh();
  return userErrors::no_error;
}

Game::userErrors Game::command(const std::string_view command) {
  static int s_extraLines{0};
  if (command.size() == 0) {
    s_extraLines++;
    return Game::userErrors::blank_command;
  }
  switch (command.at(0)) {
  case 'm': {
    const int gameLineSize{
        static_cast<int>(NUMBER_OF_LINES_PRINTED_TO_DISPLAY_GAME)};
    size_t start{1};

    while (start < command.size() && std::isspace(command[start])) {
      ++start;
    }

    size_t from_start{start};
    while (start < command.size() && !std::isspace(command[start])) {
      ++start;
    }
    if (from_start == start) {
      s_extraLines++;
      return Game::userErrors::command_formating;
    }
    std::string_view from = command.substr(from_start, start - from_start);

    while (start < command.size() && std::isspace(command[start])) {
      ++start;
    }

    size_t to_start{start};
    while (start < command.size() && !std::isspace(command[start])) {
      ++start;
    }
    if (to_start == start) {
      s_extraLines++;
      return Game::userErrors::command_formating;
    }
    std::string_view to = command.substr(to_start, start - to_start);
    Game::userErrors error{move(from, to)};
    if (error == userErrors::no_error) {
      clearPreviousLines(gameLineSize + 2 * s_extraLines);
      s_extraLines = 0;
      std::cout << *this << std::endl;
    } else {
      s_extraLines++;
    }
    return error;
  }
  case 'd': // draw
  {
    draw();
    clearPreviousLines(NUMBER_OF_LINES_PRINTED_TO_DISPLAY_GAME +
                       2 * s_extraLines);
    s_extraLines = 0;
    std::cout << *this << std::endl;
    return {};
  }
  case 'c': // clear
  {
    clearPreviousLines(NUMBER_OF_LINES_PRINTED_TO_DISPLAY_GAME +
                       2 * s_extraLines);
    s_extraLines = 0;
    std::cout << *this << std::endl;
    return {};
  }
  default: {
    s_extraLines++;
    return Game::userErrors::command_not_found;
  }
  }
}

bool Game::isGameOver() const {
  for (const SuitPile &suitPile : m_suitPiles) {
    if (!suitPile.filedUp()) {
      return false;
    }
  }
  return true;
}

std::string_view errorToString(const Game::userErrors error) {
  switch (error) {
  case Game::userErrors::no_error:
    return "";
  case Game::userErrors::blank_command:
    return "no command entered";
  case Game::userErrors::command_not_found:
    return "the command you entered dose not exits";
  case Game::userErrors::command_formating:
    return "the command was formatted incorrectly";
  case Game::userErrors::register_not_found:
    return "the registers you entered dose not exist";
  case Game::userErrors::invalid_move:
    return "you can not make that move";
  }
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

  const size_t size{game.longestCardStackLen()};
  cout << std::endl;
  for (size_t i{0}; i < size; i++) {
    for (const CardStack &c : game.m_cardStack) {
      cout << c.at(i) << ' ';
    }
    cout << std::endl;
  }
  return cout;
}
