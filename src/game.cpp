#include "game.h"
#include <iterator>

std::ostream &operator<<(std::ostream &cout, const Game &game) {
  cout << game.m_deck << "     ";
  for (int i{0}; i < std::size(game.m_suitPiles); i++) {
    cout << game.m_suitPiles[i] << ' ';
  }
  return cout;
}
