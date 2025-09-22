#include "game.h"
#include "cardStack.h"

#include <cassert>
#include <iostream>
#include <iterator>

Game::Game() {
  for (int i{0}; i < 7; i++) {
    for (int j{i}; j < 7; j++) {
      m_cardStack.at(j).addClosedCard(m_deck.drawAndTake());
    }
  }
  refresh();
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
