#ifndef CAN_CARD_STACK_H
#define CAN_CARD_STACK_H

#include "card.h"
#include <cstddef>
#include <optional>
#include <vector>

class CardStack {
  std::vector<Card> m_closedCards{};
  std::vector<Card> m_openCards{};

public:
  void addClosedCard(Card closedCard);
  void refresh();
  static bool canAddOpenCard(std::optional<Card> oldOpenCard, Card openCard);
  bool canAddOpenCard(Card openCard) const;
  bool addOpenCards(std::vector<Card> &openCard);
  std::vector<Card> takeOpenCards(size_t noOfOpenCards);
  size_t size() const { return m_closedCards.size() + m_openCards.size(); }
  std::optional<Card> at(size_t index) const;
};

#endif // !CAN_CARD_STACK_H
