#include "main.h"
#include "game.h"
#include "leaderBoard.h"
#include "stats.h"

#include <argparse/argparse.hpp>
#include <cctype>
#include <chrono>
#include <cstddef>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <ostream>
#include <string>
#include <thread>

const std::string PROGRAM_DESCRIPTION{
    "A program to play solitaire\n\n"
    "Commands:\n"
    "  d: draws a card\n"
    "  m [FromRegister] [ToRegister]: move a "
    "card from FromRegister to ToRegister\n\n"
    "    Registers:\n"
    "      1-7: these are all of the Tableau in order\n"
    "      w: the first Foundation\n"
    "      e: the second Foundation\n"
    "      r: the third Foundation\n"
    "      t: the fourth Foundation\n"
    "      d: the Waste Pile\n\n"
    "  c: redraws the game\n"
    "  s [FileName]: saves the current game to the file FileName\n"
    "  l [FileName]: loads the current game from the file FileName\n"
    "  h: prints the list of commands\n"
    "  q: quit the game\n"};

argparse::ArgumentParser arg("solitaire", PROJECT_VERSION);

int main(int argc, char *argv[]) {

  // Argument parsing
  arg.add_description(PROGRAM_DESCRIPTION);
  arg.add_argument("--leader-board")
      .help("set where the leader board is stored")
      .metavar("FILE")
      .default_value(getXdgDataHome() + "/solitaire/solitaire.json");

  argparse::ArgumentParser leaderBoardParser{"leaderBoard"};
  leaderBoardParser.add_description("display the leader board of past games");

  leaderBoardParser.add_argument("-n")
      .help("maximum number of games to be displayed")
      .metavar("NUMBER")
      .default_value(10)
      .nargs(1)
      .scan<'d', int>();

  leaderBoardParser.add_argument("-a")
      .help("display the average moves and average time per game")
      .default_value(false)
      .implicit_value(true);

  leaderBoardParser.add_argument("-d")
      .help("display the day each game ended at")
      .default_value(false)
      .implicit_value(true);

  leaderBoardParser.add_argument("-i")
      .help("display the index of the games")
      .default_value(false)
      .implicit_value(true);

  leaderBoardParser.add_argument("-t")
      .help("display the time the games took to beat")
      .default_value(false)
      .implicit_value(true);

  leaderBoardParser.add_argument("-m")
      .help("display the number of moves the games took to beat")
      .default_value(false)
      .implicit_value(true);

  leaderBoardParser.add_argument("-s")
      .help("display the seed of the games")
      .default_value(false)
      .implicit_value(true);

  leaderBoardParser.add_argument("-u")
      .help("display the user name of the person who beat the games")
      .default_value(false)
      .implicit_value(true);

  leaderBoardParser.add_argument("-v")
      .help("display the version the games was beaten at")
      .default_value(false)
      .implicit_value(true);

  arg.add_subparser(leaderBoardParser);

  auto &gameStateGroup = arg.add_mutually_exclusive_group();
  gameStateGroup.add_argument("-s", "--seed")
      .help("runs program with a set seed")
      .metavar("SEED")
      .scan<'d', unsigned long>();
  gameStateGroup.add_argument("-f", "--file")
      .help("continue a previous game from save file")
      .metavar("FILE");

  try {
    arg.parse_args(argc, argv);

  } catch (const std::exception &err) {
    std::cerr << err.what() << std::endl;
    std::cerr << arg;
    return EXIT_FAILURE;
  }

  LeaderBoard leaderBoard{arg.get("--leader-board")};

  if (arg.is_subcommand_used(leaderBoardParser)) {
    std::cout << leaderBoard.toString(
        leaderBoardParser.get<int>("-n"), leaderBoardParser.get<bool>("-a"),
        leaderBoardParser.get<bool>("-d"), leaderBoardParser.get<bool>("-i"),
        leaderBoardParser.get<bool>("-t"), leaderBoardParser.get<bool>("-m"),
        leaderBoardParser.get<bool>("-s"), leaderBoardParser.get<bool>("-u"),
        leaderBoardParser.get<bool>("-v"));
    return EXIT_SUCCESS;
  }

  Game game;

  if (arg.is_used("--file")) {
    std::ifstream file(arg.get("--file"));
    if (!file.is_open()) {
      std::cerr << "Error: failed to open file" << std::endl;
      return EXIT_FAILURE;
    }

    nlohmann::json j;

    file >> j;

    game = j.get<Game>();
    file.close();
  } else if (arg.is_used("--seed")) {
    game = Game{arg.get<unsigned long>("--seed")};
  } else {
    game = Game{};
  }

  // main loop

  std::cout << game << std::endl;
  while (!game.isGameOver()) {
    std::string command{};
    std::cin.sync();
    std::cout << ':';
    std::getline(std::cin, command);

    Game::userErrors commandError{game_command(game, command)};
    if (commandError == Game::userErrors::command_not_found &&
        command.at(0) == 'q') {
      goto endGame;
    } else if (commandError != Game::userErrors::no_error) {
      std::cout << command << ": " << errorToString(commandError) << std::endl;
    }
  }
  endingAnimation(game);
  {
    Stats stats{game.getStats()};
    leaderBoard.add(stats);
    leaderBoard.save();
    std::cout << stats << std::endl;
  }
endGame:
  return EXIT_SUCCESS;
}

Game::userErrors game_command(Game &game, const std::string_view command) {
  static int s_extraLines{0};
  if (command.size() == 0) {
    s_extraLines += 2;
    return Game::userErrors::blank_command;
  }
  switch (command.at(0)) {
  case 'h': {
    s_extraLines += 20;
    std::cout << PROGRAM_DESCRIPTION;
    return {};
  }
  case 'm': {
    const int gameLineSize{static_cast<int>(3 + game.longestCardStackLen())};
    size_t start{1};

    while (start < command.size() && std::isspace(command[start])) {
      ++start;
    }

    size_t from_start{start};
    while (start < command.size() && !std::isspace(command[start])) {
      ++start;
    }
    if (from_start == start) {
      s_extraLines += 2;
      return Game::userErrors::command_formating;
    }
    std::string_view from = command.substr(from_start, start - from_start);

    while (start < command.size() && std::isspace(command[start])) {
      ++start;
    }

    size_t to_start{start};
    while (start < command.size() && !std::isspace(command[start])) {
      ++start;
    }
    if (to_start == start) {
      s_extraLines += 2;
      return Game::userErrors::command_formating;
    }
    std::string_view to = command.substr(to_start, start - to_start);
    Game::userErrors error{game.move(from, to)};
    if (error == Game::userErrors::no_error) {
      clearPreviousLines(gameLineSize + s_extraLines);
      s_extraLines = 0;
      std::cout << game << std::endl;
    } else {
      s_extraLines += 2;
    }
    return error;
  }
  case 'd': // draw
  {
    game.draw();
    clearPreviousLines(3 + game.longestCardStackLen() + s_extraLines);
    s_extraLines = 0;
    std::cout << game << std::endl;
    return {};
  }
  case 'c': // clear
  {
    clearPreviousLines(3 + game.longestCardStackLen() + s_extraLines);
    s_extraLines = 0;
    std::cout << game << std::endl;
    return {};
  }
  case 's': {
    nlohmann::json j = game;
    size_t start{1};

    while (start < command.size() && std::isspace(command[start])) {
      ++start;
    }

    size_t from_start{start};
    while (start < command.size() && !std::isspace(command[start])) {
      ++start;
    }
    if (from_start == start) {
      s_extraLines += 2;
      return Game::userErrors::command_formating;
    }
    std::string fileName{command.substr(from_start, start - from_start)};

    std::ofstream file(fileName);
    if (!file.is_open()) {
      s_extraLines += 2;
      return Game::userErrors::file_not_found;
    }

    file << j.dump();

    clearPreviousLines(3 + game.longestCardStackLen() + s_extraLines);
    s_extraLines = 0;
    std::cout << game << std::endl;
    return {};
  }
  case 'l': {
    size_t start{1};

    while (start < command.size() && std::isspace(command[start])) {
      ++start;
    }

    size_t from_start{start};
    while (start < command.size() && !std::isspace(command[start])) {
      ++start;
    }
    if (from_start == start) {
      s_extraLines += 2;
      return Game::userErrors::command_formating;
    }
    std::string fileName{command.substr(from_start, start - from_start)};

    std::ifstream file(fileName);
    if (!file.is_open()) {
      s_extraLines += 2;
      return Game::userErrors::file_not_found;
    }

    nlohmann::json j;
    file >> j;
    game = j.get<Game>();

    clearPreviousLines(3 + game.longestCardStackLen() + s_extraLines);
    s_extraLines = 0;
    std::cout << game << std::endl;
    return {};
  }
  default: {
    s_extraLines += 2;
    return Game::userErrors::command_not_found;
  }
  }
}

char intToSuitPileReg(int i) {
  switch (i) {
  case 0:
    return 'w';
  case 1:
    return 'e';
  case 2:
    return 'r';
  case 3:
    return 't';
  }
  return ' ';
}

#define animationSpeed 100
void endingAnimation(Game &game) {
  size_t cardStackLen{game.longestCardStackLen()};
  while (!game.isCardStacksEmpty()) {
    for (int i{1}; i <= 7; i++) {
      for (int j{0}; j < 4; j++) {
        if (game.move(std::to_string(i), std::string(1, intToSuitPileReg(j))) ==
            Game::userErrors::no_error) {
          clearPreviousLines(2 + cardStackLen);
          cardStackLen = game.longestCardStackLen();
          std::cout << game << std::endl;
          std::this_thread::sleep_for(
              std::chrono::milliseconds(animationSpeed));
        }
      }
    }
  }
}

std::string getXdgDataHome() {
  const char *xdgDataHome = std::getenv("XDG_DATA_HOME");
  if (xdgDataHome && *xdgDataHome) {
    return std::string(xdgDataHome);
  }

  const char *home = std::getenv("HOME");
  if (home && *home) {
    return std::string(home) + "/.local/share";
  }

  return "/tmp";
}
