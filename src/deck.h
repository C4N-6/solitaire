#ifndef CAN_DECK_H
#define CAN_DECK_H

#include "card.h"

#include <list>
#include <ostream>
#include <vector>

class Deck {
private:
  std::vector<Card> m_deck{};
  std::vector<Card> m_deckPile{};

public:
  Deck();

  void draw();
  Card take();
  Card drawAndTake();

  void shufle();

  friend std::ostream &operator<<(std::ostream &cout, const Deck &deck);
};

#endif // !CAN_DECK_H
