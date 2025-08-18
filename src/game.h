#ifndef CAN_GAME_H
#define CAN_GAME_H
#include "deck.h"
#include "suitPile.h"

#include <array>
#include <ostream>
class Game {
  Deck m_deck{};
  std::array<SuitPile, 4> m_suitPiles{};

public:
  friend int main(int argc, char *argv[]);
  friend std::ostream &operator<<(std::ostream &cout, const Game &game);
};

#endif // !CAN_GAME_H
