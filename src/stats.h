#pragma once

#include "UserName.h"
#include "deck.h"
#include <string>

struct Time {
  std::time_t m_second{};
  std::time_t m_minute{};
  std::time_t m_hours{};

  Time(std::time_t seconds) {
    m_hours = seconds / (60 * 60);
    seconds %= (60 * 60);
    m_minute = seconds / 60;
    m_second = seconds % 60;
  }
};
std::ostream &operator<<(std::ostream &cout, const Time &time);

struct Stats {
  std::string version{PROJECT_VERSION};
  seed_t seed{};
  std::time_t startTime{};
  std::time_t endTime{};
  int moveCount{0};
  /**
   * the user who started the game
   */
  std::string user{getCurrentUserName()};
};

void to_json(nlohmann::json &json, const Stats &stats);
void from_json(const nlohmann::json &j, Stats &stats);
std::ostream &operator<<(std::ostream &cout, const Stats &s);
