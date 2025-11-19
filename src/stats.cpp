#include "stats.h"
#include <sstream>
std::ostream &operator<<(std::ostream &cout, const Time &time) {
  std::stringstream formattedTime{};
  if (time.m_hours != 0) {
    formattedTime << time.m_hours << "h ";
  }
  if (time.m_minute != 0) {
    formattedTime << time.m_minute << "m ";
  }
  formattedTime << time.m_second << "s";
  return cout << formattedTime.str();
}

std::ostream &operator<<(std::ostream &cout, const Stats &s) {
  cout << "The game(seed: " << s.seed << ") took "
       << Time(s.endTime - s.startTime) << ", and was completed in "
       << s.moveCount << " moves";
  return cout;
}

void from_json(const nlohmann::json &j, Stats &stats) {
  j.at("seed").get_to(stats.seed);
  j.at("startTime").get_to(stats.startTime);
  j.at("endTime").get_to(stats.endTime);
  j.at("version").get_to(stats.version);
  j.at("moveCount").get_to(stats.moveCount);
}

void to_json(nlohmann::json &json, const Stats &stats) {
  json = {{"seed", stats.seed},           {"startTime", stats.startTime},
          {"endTime", stats.endTime},     {"version", stats.version},
          {"moveCount", stats.moveCount}, {"user", stats.user}};
}
