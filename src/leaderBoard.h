#pragma once

#include "stats.h"
#include <cstddef>
#include <filesystem>
#include <vector>

class LeaderBoard {
  std::vector<Stats> m_stats{};
  std::filesystem::path m_path{};

public:
  LeaderBoard(std::filesystem::path leaderBoardFile);

  void add(Stats stat) { m_stats.push_back(stat); }
  void sort(bool (*compare)(Stats &, Stats &) = [](Stats &s1,
                                                   Stats &s2) -> bool {
    return s1.moveCount > s2.moveCount;
  });

  void save() const { save(m_path); };
  void save(std::filesystem::path path) const;

  std::string toString(size_t NUMBER_OF_GAMES_DISPLAYED = 10,
                       bool SHOW_AVERAGES = true, bool SHOW_DATE = true,
                       bool SHOW_INDEX = true, bool SHOW_TIME = true,
                       bool SHOW_MOVES = true, bool SHOW_SEED = true,
                       bool SHOW_USERNAME = true,
                       bool SHOW_VERSION = true) const;
};
