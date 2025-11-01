#ifndef CAN_SUIT_PILE_H
#define CAN_SUIT_PILE_H

#include "card.h"

#include <nlohmann/json.hpp>
#include <ostream>

class SuitPile {
  Card::Suit m_suit{};
  int m_currntCard{0};

public:
  SuitPile() {}
  std::optional<Card> peek() const;
  std::optional<Card> pop();
  bool canAdd(const Card card) const;
  bool add(const Card card);
  bool empty() const { return !m_currntCard; }
  bool filedUp() const {
    return m_currntCard == static_cast<int>(Card::Face::king);
  }

  friend void to_json(nlohmann::json &json, const SuitPile &suitPile);
  friend void from_json(const nlohmann::json &j, SuitPile &s);
};

inline std::ostream &operator<<(std::ostream &cout, const SuitPile &suitPile) {
  return cout << suitPile.peek();
}

#endif // !CAN_SUIT_PILE_H
