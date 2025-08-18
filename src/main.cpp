#include "card.h"
#include "game.h"
#include <iostream>

int main(int argc, char *argv[]) {
  Game game{};
  game.m_suitPiles[0].add(Card{Card::Suit::club, Card::Face::ace});
  game.m_suitPiles[1].add(Card{Card::Suit::heart, Card::Face::ace});
  game.m_suitPiles[2].add(Card{Card::Suit::diamond, Card::Face::ace});
  game.m_suitPiles[3].add(Card{Card::Suit::spade, Card::Face::ace});
  for (int i{0}; i < 54; i++) {
    std::cout << game << std::endl;
    game.m_deck.draw();
  }
  return 0;
}
