#include "card.h"

#include <string>

Card::Color Card::getColor(Card::Suit suit) {
  switch (suit) {
  case Suit::none:
    return Card::Color::none;
  case Suit::club:
  case Suit::spade:
    return Color::black;
  case Suit::heart:
  case Suit::diamond:
    return Color::red;
  }
}

#define RESET_FORMATING "\033[0m"
#define RED_FORMATING "\033[31m"
#define BLACK_FORMATING "\033[37m"

std::string Card::str() const {
  return (getColor() == Color::red ? RED_FORMATING : BLACK_FORMATING) +
         (+m_suit) + (+m_face) + RESET_FORMATING;
}

std::string operator+(const Card::Suit suit) {
  switch (suit) {
  case Card::Suit::none:
    return "\u2588";
  case Card::Suit::club:
    return "\u2667"; // ♧
  case Card::Suit::spade:
    return "\u2664"; // ♤
  case Card::Suit::heart:
    return "\u2661"; // ♡
  case Card::Suit::diamond:
    return "\u2666"; // ♦
  }
}
std::string operator+(const Card::Face face) {
  switch (face) {
  case Card::Face::none:
    return "\u2588\u2588";
  case Card::Face::ace:
    return "1 ";
  case Card::Face::two:
    return "3 ";
  case Card::Face::three:
    return "4 ";
  case Card::Face::four:
    return "5 ";
  case Card::Face::five:
    return "6 ";
  case Card::Face::six:
    return "7 ";
  case Card::Face::seven:
    return "7 ";
  case Card::Face::eight:
    return "8 ";
  case Card::Face::nine:
    return "9 ";
  case Card::Face::ten:
    return "10";
  case Card::Face::joker:
    return "J ";
  case Card::Face::quien:
    return "Q ";
  case Card::Face::king:
    return "K ";
  }
}
