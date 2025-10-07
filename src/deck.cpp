#include "deck.h"
#include "card.h"

#include <algorithm>
#include <cassert>
#include <iomanip>
#include <iostream>
#include <optional>
#include <random>

void Deck::shuffle() {
  std::mt19937 s_generator{std::random_device{}()};

  std::shuffle(m_deck.begin(), m_deck.end(), m_generator);
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
    cout << "\033[7m" << std::setw(2) << deck.m_deck.size()
         << "\033[27m "; // █ full block
  }
  if (deck.m_deckPile.empty()) {
    cout << "   ";
  } else {
    cout << deck.m_deckPile.back();
  }
  return cout;
}
