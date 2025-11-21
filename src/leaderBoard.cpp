#include "leaderBoard.h"
#include "stats.h"
#include <algorithm>
#include <cstddef>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
LeaderBoard::LeaderBoard(std::filesystem::path leaderBoardFilePath) {
  m_path = leaderBoardFilePath;
  std::ifstream leaderBoardFile{leaderBoardFilePath};
  if (leaderBoardFile.is_open()) {
    nlohmann::json json;

    leaderBoardFile >> json;

    m_stats = json.get<std::vector<Stats>>();
  }
  leaderBoardFile.close();
}
time_t LeaderBoard::getAvgTime() const {
  int count{0};
  for (const Stats &stat : m_stats) {
    count += stat.endTime - stat.startTime;
  }
  return count / m_stats.size();
}
float LeaderBoard::getAvgMove() const {
  int count{0};
  for (const Stats &stat : m_stats) {
    count += stat.moveCount;
  }
  return count / static_cast<float>(m_stats.size());
}

void LeaderBoard::sort(bool (*compare)(const Stats &, const Stats &)) {
  std::sort(m_stats.begin(), m_stats.end(), compare);
}

void LeaderBoard::save(std::filesystem::path path) const {
  std::filesystem::create_directories(path.parent_path());
  std::ofstream leaderBoardFile{path};
  if (leaderBoardFile.is_open()) {
    leaderBoardFile << nlohmann::json(m_stats).dump(4);
  }

  leaderBoardFile.close();
}

std::string LeaderBoard::toString(size_t NUMBER_OF_GAMES_DISPLAYED,
                                  bool SHOW_AVERAGES, bool SHOW_DATE,
                                  bool SHOW_INDEX, bool SHOW_TIME,
                                  bool SHOW_MOVES, bool SHOW_SEED,
                                  bool SHOW_USERNAME, bool SHOW_VERSION) const {
  std::stringstream str{};
  size_t lineSize{0};
  if (SHOW_INDEX) {
    str << std::setw(2) << " " << '|';
    lineSize += 3;
  }

  if (SHOW_USERNAME) {
    str << std::setw(12) << "user name" << '|';
    lineSize += 13;
  }

  if (SHOW_VERSION) {
    str << std::setw(7) << "version" << '|';
    lineSize += 8;
  }

  if (SHOW_DATE) {
    str << std::setw(11) << "date" << '|';
    lineSize += 12;
  }

  if (SHOW_MOVES) {
    str << std::setw(4) << "move" << '|';
    lineSize += 5;
  }

  if (SHOW_TIME) {
    str << std::setw(10) << "time" << '|';
    lineSize += 15;
  }

  if (SHOW_SEED) {
    str << std::setw(20) << "seed" << '|';
    lineSize += 21;
  }
  str << std::endl << std::string(lineSize, '-') << std::endl;
  NUMBER_OF_GAMES_DISPLAYED =
      std::min(NUMBER_OF_GAMES_DISPLAYED, m_stats.size());
  for (size_t i{0}; i < NUMBER_OF_GAMES_DISPLAYED; i++) {
    bool addNewLine{false};
    Stats stat = m_stats.at(i);
    if (SHOW_INDEX) {
      str << std::setw(2) << i + 1 << '|';
      addNewLine = true;
    }

    if (SHOW_USERNAME) {
      str << std::setw(12) << stat.user << '|';
      addNewLine = true;
    }

    if (SHOW_VERSION) {
      str << std::setw(7) << stat.version << '|';
      addNewLine = true;
    }

    if (SHOW_DATE) {
      std::tm *now = std::localtime(&stat.endTime);
      str << std::setw(5) << (now->tm_year + 1900) << ' ' << std::setw(2)
          << (now->tm_mon + 1) << ' ' << std::setw(2) << now->tm_mday << '|';
      addNewLine = true;
    }

    if (SHOW_MOVES) {
      str << std::setw(4) << stat.moveCount << '|';
      addNewLine = true;
    }

    if (SHOW_TIME) {
      str << std::setw(10) << Time{stat.endTime - stat.startTime} << '|';
      addNewLine = true;
    }

    if (SHOW_SEED) {
      str << std::setw(20) << stat.seed << '|';
      addNewLine = true;
    }
    if (addNewLine) {
      str << std::endl;
    }
  }
  if (SHOW_AVERAGES) {
    str << getAvgMove() << " average moves per game, " << Time{getAvgTime()}
        << " average game time." << std::endl;
  }
  return str.str();
}

template <bool (*Func)(const Stats &, const Stats &)>
bool reverseCompare(const Stats &s1, const Stats &s2) {
  return !Func(s1, s2);
}
bool moveCompare(const Stats &s1, const Stats &s2) {
  return s1.moveCount < s2.moveCount;
}
bool timeCompare(const Stats &s1, const Stats &s2) {
  return s1.endTime - s1.startTime < s2.endTime - s2.startTime;
}
bool dateCompare(const Stats &s1, const Stats &s2) {
  return s1.endTime < s2.endTime;
}
bool usernameCompare(const Stats &s1, const Stats &s2) {
  return s1.user.compare(s2.user) < 0;
}
bool seedCompare(const Stats &s1, const Stats &s2) { return s1.seed < s2.seed; }

bool (*getCompareFunction(std::string functionName,
                          bool reverse))(const Stats &, const Stats &) {
  if (reverse) {
    if (functionName == "move") {
      return &reverseCompare<moveCompare>;
    } else if (functionName == "time") {
      return &reverseCompare<timeCompare>;
    } else if (functionName == "date") {
      return &reverseCompare<dateCompare>;
    } else if (functionName == "username") {
      return &reverseCompare<usernameCompare>;
    } else if (functionName == "seed") {
      return &reverseCompare<seedCompare>;
    } else {
      return &reverseCompare<moveCompare>;
    }
  } else {
    if (functionName == "move") {
      return &moveCompare;
    } else if (functionName == "time") {
      return &timeCompare;
    } else if (functionName == "date") {
      return &dateCompare;
    } else if (functionName == "username") {
      return &usernameCompare;
    } else if (functionName == "seed") {
      return &seedCompare;
    } else {
      return &moveCompare;
    }
  }
}
