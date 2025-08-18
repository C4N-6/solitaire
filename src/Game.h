#ifndef CAN_GAME_H
#define CAN_GAME_H
#include "SuitPile.h"
#include "deck.h"

#include <ostream>
class Game {
  Deck m_deck{};
  SuitPile m_suitPiles[4]{};

public:
  friend int main(int argc, char *argv[]);
  friend std::ostream &operator<<(std::ostream &cout, const Game &game);
};

#endif // !CAN_GAME_H
