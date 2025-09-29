#include "deck.h"
#include "card.h"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <optional>
#include <random>

Deck::Deck() {
  for (int i{static_cast<int>(Card::Suit::club)};
       i <= static_cast<int>(Card::Suit::diamond); i++) {
    Card::Suit suit{static_cast<Card::Suit>(i)};
    for (int j{static_cast<int>(Card::Face::ace)};
         j <= static_cast<int>(Card::Face::king); j++) {
      Card::Face face{static_cast<Card::Face>(j)};
      m_deck.push_back(Card{suit, face});
    }
  }

  shufle();

  assert(m_deck.size() == 52 && "Is the deck a full deck");
}

void Deck::shufle() {
  std::mt19937 s_generator(std::random_device{}());

  std::shuffle(m_deck.begin(), m_deck.end(), s_generator);
}

std::optional<Card> Deck::peek() const {
  return m_deckPile.empty() ? std::optional<Card>{} : m_deckPile.back();
}

void Deck::draw() {
  if (m_deck.empty()) {
    while (!m_deckPile.empty()) {
      m_deck.push_back(m_deckPile.back());
      m_deckPile.pop_back();
    }
  } else {
    m_deckPile.push_back(m_deck.back());
    m_deck.pop_back();
  }
}
std::optional<Card> Deck::take() {
  std::optional<Card> returnVar{peek()};
  if (returnVar.has_value()) {
    m_deckPile.pop_back();
  }
  return returnVar;
}
std::optional<Card> Deck::drawAndTake() {
  draw();
  return take();
}

std::ostream &operator<<(std::ostream &cout, const Deck &deck) {
  if (deck.m_deck.empty()) {
    cout << "   ";
  } else {
    cout << "\u2588\u2588 "; // █ full block
  }
  if (deck.m_deckPile.empty()) {
    cout << "   ";
  } else {
    cout << deck.m_deckPile.back();
  }
  return cout;
}
