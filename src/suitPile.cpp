#include "suitPile.h"
#include "card.h"

#include <algorithm>
#include <optional>

std::optional<Card> SuitPile::peek() const {
  if (empty()) {
    return {};
  }
  return Card{m_suit, Card::Face{m_currntCard}};
}

std::optional<Card> SuitPile::pop() {
  std::optional<Card> card{peek()};
  m_currntCard = std::max(0, m_currntCard - 1);
  return card;
}

bool SuitPile::canAdd(const Card card) const {
  return m_currntCard < static_cast<int>(Card::Face::king) &&
         (m_suit == card.getSuit() || empty()) &&
         static_cast<int>(card.getFace()) == m_currntCard + 1;
}

bool SuitPile::add(Card card) {
  if (!canAdd(card)) {
    return false;
  }
  if (empty()) {
    m_suit = card.getSuit();
  }
  m_currntCard++;
  return true;
}

void from_json(const nlohmann::json &j, SuitPile &s) {
  j.at("suit").get_to(s.m_suit);
  j.at("currentCard").get_to(s.m_currntCard);
}
void to_json(nlohmann::json &json, const SuitPile &suitPile) {
  json = {{"suit", suitPile.m_suit}, {"currentCard", suitPile.m_currntCard}};
}
