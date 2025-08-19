#include "cardStack.h"
#include "card.h"

#include <algorithm>
#include <cstddef>
#include <optional>
#include <vector>

void CardStack::addClosedCard(Card closedCard) {
  m_closedCards.push_back(closedCard);
}
void CardStack::refresh() {
  if (m_openCards.size() == 0 && m_closedCards.size() != 0) {
    m_openCards.push_back(m_closedCards.back());
    m_closedCards.pop_back();
  }
}
bool CardStack::canAddOpenCard(std::optional<Card> oldOpenCard, Card openCard) {
  return (!oldOpenCard.has_value() && openCard.getFace() == Card::Face::king) ||
         (oldOpenCard.has_value() &&
          static_cast<int>(oldOpenCard.value().getFace()) ==
              static_cast<int>(openCard.getFace()) + 1 &&
          oldOpenCard.value().getColor() != openCard.getColor());
}
bool CardStack::canAddOpenCard(Card openCard) {
  if (m_openCards.empty()) {
    return canAddOpenCard(std::optional<Card>{}, openCard);
  }
  return canAddOpenCard(m_openCards.back(), openCard);
}
bool CardStack::addOpenCards(std::vector<Card> &openCard) {
  if (openCard.empty() || !canAddOpenCard(openCard.back())) {
    return false;
  }
  for (size_t i{openCard.size() - 2}; i < openCard.size(); i--) {
    if (!canAddOpenCard(openCard.at(i + 1), openCard.at(i))) {
      return false;
    }
  }

  while (!openCard.empty()) {
    m_openCards.push_back(openCard.back());
    openCard.pop_back();
  }

  return true;
}
std::vector<Card> CardStack::takeOpenCards(size_t noOfOpenCards) {
  noOfOpenCards = std::min(noOfOpenCards, m_openCards.size());
  std::vector<Card> removedCards{};
  for (size_t i{0}; i < noOfOpenCards; i++) {
    removedCards.push_back(m_openCards.back());
    m_openCards.pop_back();
  }
  return removedCards;
}
