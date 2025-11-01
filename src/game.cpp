#include "game.h"
#include "card.h"
#include "cardStack.h"
#include "suitPile.h"

#include <algorithm>
#include <cassert>
#include <cctype>
#include <cstddef>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <limits>
#include <nlohmann/json.hpp>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

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
bool charIsASuitPile(const char c) {
  switch (c) {
  case 'w':
  case 'e':
  case 'r':
  case 't':
    return true;
  default:
    return false;
  }
}
SuitPile &Game::getSuitPileFromChar(const char c) {
  switch (c) {
  case 'w':
    return m_suitPiles.at(0);
  case 'e':
    return m_suitPiles.at(1);
  case 'r':
    return m_suitPiles.at(2);
  case 't':
    return m_suitPiles.at(3);
  }
  assert(true && "impossible suitPiles to check for");
  return m_suitPiles.at(0);
}

Game::userErrors Game::move(const std::string_view from,
                            const std::string_view to) {
  if (!charIsAReg(from.at(0)) || !charIsAReg(to.at(0))) {
    return userErrors::register_not_found;
  }

  if (charIsASuitPile(from.at(0)) &&
      charIsASuitPile(to.at(0))) { // switch 2 suitPiles
    SuitPile &fromPile = getSuitPileFromChar(from.at(0));
    SuitPile &toPile = getSuitPileFromChar(to.at(0));

    SuitPile temp{fromPile};
    fromPile = toPile;
    toPile = temp;
  } else {
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
      unsigned long numberOfCardsToTake{cardStack.noOfOpenCards()};
      char toChar{to.at(0)};
      if (from.size() >= 3) {
        numberOfCardsToTake =
            std::stoi(static_cast<std::string>(from.substr(2)));
      } else if (toChar == 'w' || toChar == 'e' || toChar == 'r' ||
                 toChar == 't') {
        numberOfCardsToTake = 1;
      }
      movingCard = cardStack.peekOpenCards(numberOfCardsToTake);
    }

    // find if we can move those cards
    if (movingCard.empty() || to.at(0) == 'd') {
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

    // remove the cards
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
      unsigned long numberOfCardsToTake{cardStack.noOfOpenCards()};
      char toChar{to.at(0)};
      if (from.size() >= 3) {
        numberOfCardsToTake =
            std::stoi(static_cast<std::string>(from.substr(2)));
      } else if (toChar == 'w' || toChar == 'e' || toChar == 'r' ||
                 toChar == 't') {
        numberOfCardsToTake = 1;
      }
      movingCard = cardStack.takeOpenCards(numberOfCardsToTake);
    } else {
      return userErrors::register_not_found;
    }
  }

  refresh();
  m_stats.moveCount++;
  return userErrors::no_error;
}
bool Game::isGameOver() const {
  if (m_deck.count() != 0) {
    return false;
  }
  for (const CardStack &cardStack : m_cardStack) {
    if (cardStack.noOfClosedCards() != 0) {
      return false;
    }
  }
  return true;
}

bool Game::isCardStacksEmpty() const {
  if (m_deck.count() != 0) {
    return false;
  }
  for (const CardStack &cardStack : m_cardStack) {
    if (cardStack.size() != 0) {
      return false;
    }
  }
  return true;
}

bool Game::isGameOver() {
  if (static_cast<const Game *>(this)->isGameOver()) {
    std::time(&m_stats.endTime);
    return true;
  }
  return false;
}

std::string_view errorToString(const Game::userErrors error) {
  using errors = Game::userErrors;
  switch (error) {
  case errors::no_error:
    return "";
  case errors::blank_command:
    return "no command entered";
  case errors::command_not_found:
    return "the command you entered dose not exits";
  case errors::command_formating:
    return "the command was formatted incorrectly";
  case errors::register_not_found:
    return "the registers you entered dose not exist";
  case errors::invalid_move:
    return "you can not make that move";
  case errors::file_not_found:
    return "the file you entered was not found";
  }
}

void Game::refresh() {
  for (CardStack &stack : m_cardStack) {
    stack.refresh();
  }
}

std::ostream &operator<<(std::ostream &cout, const Time &time) {
  if (time.m_hours != 0) {
    cout << time.m_hours << "h ";
  }
  if (time.m_minute != 0) {
    cout << time.m_minute << "m ";
  }
  cout << time.m_second << "s";
  return cout;
}

std::ostream &operator<<(std::ostream &cout, const Stats &s) {
  cout << "The game(seed: " << s.seed << ") took "
       << Time(s.endTime - s.startTime) << ", and was completed in "
       << s.moveCount << " moves";
  return cout;
}

std::ostream &operator<<(std::ostream &cout, const Game &game) {
  cout << game.m_deck << std::string(4, ' ');
  for (int i{0}; i < std::size(game.m_suitPiles); i++) {
    cout << std::setw(3) << game.m_suitPiles[i] << ' ';
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

void from_json(const nlohmann::json &j, Stats &stats) {
  j.at("seed").get_to(stats.seed);
  j.at("startTime").get_to(stats.startTime);
  j.at("endTime").get_to(stats.endTime);
  j.at("version").get_to(stats.version);
  j.at("moveCount").get_to(stats.moveCount);
}
void to_json(nlohmann::json &json, const Stats &stats) {
  json = {{"seed", stats.seed},
          {"startTime", stats.startTime},
          {"endTime", stats.endTime},
          {"version", stats.version},
          {"moveCount", stats.moveCount}};
}
void from_json(const nlohmann::json &j, Game &game) {
  j.at("stats").get_to(game.m_stats);
  j.at("deck").get_to(game.m_deck);
  j.at("cardStack").get_to(game.m_cardStack);
  j.at("suitPiles").get_to(game.m_suitPiles);
}
void to_json(nlohmann::json &json, const Game &game) {
  json = {{"stats", game.m_stats},
          {"deck", game.m_deck},
          {"cardStack", game.m_cardStack},
          {"suitPiles", game.m_suitPiles}};
}
