#ifndef CAN_GAME_H
#define CAN_GAME_H
#include "cardStack.h"
#include "deck.h"
#include "suitPile.h"

#include <array>
#include <ostream>

class Game {
  Deck m_deck{};
  std::array<SuitPile, 4> m_suitPiles{};
  std::array<CardStack, 7> m_cardStack{};

public:
  Game();
  void refresh();
  friend int main(int argc, char *argv[]);
  friend std::ostream &operator<<(std::ostream &cout, const Game &game);
};

#endif // !CAN_GAME_H
