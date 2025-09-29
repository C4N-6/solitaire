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
  std::vector<Card> peekOpenCards(size_t noOfOpenCards) const;
  std::vector<Card> takeOpenCards(size_t noOfOpenCards);
  size_t size() const { return m_closedCards.size() + m_openCards.size(); }
  size_t noOfOpenCards() const { return m_openCards.size(); }
  std::optional<Card> at(size_t index) const;
  std::optional<Card> top() const { return at(size() - 1).value(); }
};

#endif // !CAN_CARD_STACK_H
