#ifndef CAN_DECK_H
#define CAN_DECK_H

#include "card.h"

#include <cassert>
#include <optional>
#include <ostream>
#include <random>
#include <vector>

using seed_t = unsigned long;

class Deck {
private:
  std::vector<Card> m_deck{};
  std::vector<Card> m_deckPile{};
  std::mt19937 m_generator{};

public:
  Deck() : m_generator{std::random_device{}()} {
    for (int i{static_cast<int>(Card::Suit::club)};
         i <= static_cast<int>(Card::Suit::diamond); i++) {
      Card::Suit suit{static_cast<Card::Suit>(i)};
      for (int j{static_cast<int>(Card::Face::ace)};
           j <= static_cast<int>(Card::Face::king); j++) {
        Card::Face face{static_cast<Card::Face>(j)};
        m_deck.push_back(Card{suit, face});
      }
    }

    shuffle();

    assert(m_deck.size() == 52 && "Is the deck a full deck");
  }
  Deck(seed_t seed) {
    m_generator.seed(seed);
    for (int i{static_cast<int>(Card::Suit::club)};
         i <= static_cast<int>(Card::Suit::diamond); i++) {
      Card::Suit suit{static_cast<Card::Suit>(i)};
      for (int j{static_cast<int>(Card::Face::ace)};
           j <= static_cast<int>(Card::Face::king); j++) {
        Card::Face face{static_cast<Card::Face>(j)};
        m_deck.push_back(Card{suit, face});
      }
    }

    shuffle();

    assert(m_deck.size() == 52 && "Is the deck a full deck");
  }

  void draw();
  std::optional<Card> peek() const;
  std::optional<Card> take();
  std::optional<Card> drawAndTake();

  void shuffle();

  friend std::ostream &operator<<(std::ostream &cout, const Deck &deck);
};

#endif // !CAN_DECK_H
