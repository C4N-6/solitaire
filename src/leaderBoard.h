#pragma once

#include "stats.h"
#include <cstddef>
#include <filesystem>
#include <string>
#include <vector>

template <bool (*Func)(const Stats &, const Stats &)>
bool reverseCompare(const Stats &s1, const Stats &s2);
bool moveCompare(const Stats &s1, const Stats &s2);
bool timeCompare(const Stats &s1, const Stats &s2);
bool dateCompare(const Stats &s1, const Stats &s2);
bool usernameCompare(const Stats &s1, const Stats &s2);
bool seedCompare(const Stats &s1, const Stats &s2);

bool (*getCompareFunction(std::string functionName,
                          bool reverse))(const Stats &, const Stats &);

class LeaderBoard {
  std::vector<Stats> m_stats{};
  std::filesystem::path m_path{};

public:
  LeaderBoard(std::filesystem::path leaderBoardFile);

  void add(Stats stat) { m_stats.push_back(stat); }
  void sort(bool (*compare)(const Stats &, const Stats &) = moveCompare);

  void save() const { save(m_path); };
  void save(std::filesystem::path path) const;

  std::string toString(size_t NUMBER_OF_GAMES_DISPLAYED = 10,
                       bool SHOW_AVERAGES = true, bool SHOW_DATE = true,
                       bool SHOW_INDEX = true, bool SHOW_TIME = true,
                       bool SHOW_MOVES = true, bool SHOW_SEED = true,
                       bool SHOW_USERNAME = true,
                       bool SHOW_VERSION = true) const;
};
