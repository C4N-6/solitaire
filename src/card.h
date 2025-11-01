#ifndef CAN_CARD_H
#define CAN_CARD_H

#include <nlohmann/json.hpp>
#include <optional>
#include <ostream>
#include <string>

class Card {
public:
  enum class Suit {
    none,
    club,
    spade,
    heart,
    diamond,
  };
  enum class Color {
    none,
    red,
    black,
  };
  enum class Face {
    none,
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
  Card(const Card &card) : m_suit{card.m_suit}, m_face{card.m_face} {}
  static Color getColor(Suit suit);
  Color getColor() const { return getColor(m_suit); }
  Suit getSuit() const { return m_suit; }
  Face getFace() const { return m_face; }
  std::string str() const;

  friend void to_json(nlohmann::json &j, const Card &c);
  friend void from_json(const nlohmann::json &j, Card &c);
};

std::string getColorEscSeq(const Card::Color &color);

inline std::ostream &operator<<(std::ostream &cout, const Card &card) {
  return cout << card.str();
}
inline std::ostream &operator<<(std::ostream &cout,
                                const std::optional<Card> &card) {
  if (card) {
    return cout << card.value();
  }
  return cout << "   ";
}
std::string operator+(const Card::Suit suit);
std::string operator+(const Card::Face face);
#endif // !CAN_CARD_H
