#ifndef CAN_CARD_H
#define CAN_CARD_H

#include <optional>
#include <ostream>
#include <string>

class Card {
public:
  enum class Suit {
    club,
    spade,
    heart,
    diamond,
  };
  enum class Color {
    red,
    black,
  };
  enum class Face {
    ace = 1,
    two,
    three,
    four,
    five,
    six,
    seven,
    eight,
    nine,
    ten,
    joker,
    quien,
    king
  };

private:
  Suit m_suit{};
  Face m_face{};

public:
  Card() {}
  Card(Suit suit, Face face) : m_suit{suit}, m_face{face} {}
  static Color getColor(Suit suit);
  Color getColor() const { return getColor(m_suit); }
  Suit getSuit() const { return m_suit; }
  Face getFace() const { return m_face; }
  std::string str() const;
};

inline std::ostream &operator<<(std::ostream &cout, const Card &card) {
  return cout << card.str();
}
inline std::ostream &operator<<(std::ostream &cout,
                                const std::optional<Card> &card) {
  if (card) {
    return cout << card->str();
  }
  return cout << "   ";
}
std::string operator+(const Card::Suit suit);
std::string operator+(const Card::Face face);
#endif // !CAN_CARD_H
