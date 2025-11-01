#include "main.h"
#include "game.h"

#include <argparse/argparse.hpp>
#include <chrono>
#include <fstream>
#include <iostream>
#include <ostream>
#include <sstream>
#include <string>
#include <thread>

int main(int argc, char *argv[]) {

  argparse::ArgumentParser arg{};

  arg.add_argument("-v").help("Shows version").flag();
  arg.add_argument("-s", "--seed").help("add a default seed").default_value("");
  arg.add_argument("-f", "--file")
      .help("continue from save file")
      .default_value("");

  try {
    arg.parse_args(argc, argv);

    if (!arg.get<std::string>("--seed").empty() &&
        !arg.get<std::string>("--file").empty()) {
      throw std::runtime_error(
          "Error: --seed and --file cannot be used together.");
    }
  } catch (const std::exception &err) {
    std::cerr << err.what() << std::endl;
    std::cerr << arg;
    return 1;
  }
  if (arg.get<bool>("-v")) {
#ifdef DEBUG
    std::cout << "DEBUG: ";
#endif
    std::cout << "Version: " << PROJECT_VERSION << std::endl;
    return 0;
  }

  Game game;

  if (!arg.get("--file").empty()) {
    std::ifstream file(arg.get("--file"));
    if (!file.is_open()) {
      std::cerr << "Error: failed to open file" << std::endl;
      return 1;
    }

    nlohmann::json j;

    file >> j;

    game = j.get<Game>();
  } else if (!arg.get("--seed").empty()) {
    unsigned long seed{};
    std::stringstream{arg.get("-s")} >> seed;
    game = Game{seed};
  } else {
    game = Game{};
  }

  std::cout << game << std::endl;
  while (!game.isGameOver()) {
    std::string command{};
    std::cin.sync();
    std::cout << ':';
    std::getline(std::cin, command);

    Game::userErrors commandError{game_command(game, command)};
    if (commandError == Game::userErrors::command_not_found &&
        command.at(0) == 'q') {
      std::cout << "quitting game" << std::endl;
      goto endGame;
    } else if (commandError != Game::userErrors::no_error) {
      std::cout << command << ": " << errorToString(commandError) << std::endl;
    }
  }
  clearPreviousLines(3);
  endingAnimation(game);
  std::cout << "You win!!" << std::endl;
  std::cout << game.getStats() << std::endl;
endGame:
  return 0;
}

Game::userErrors game_command(Game &game, const std::string_view command) {
  static int s_extraLines{0};
  if (command.size() == 0) {
    s_extraLines++;
    return Game::userErrors::blank_command;
  }
  switch (command.at(0)) {
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
      s_extraLines++;
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
      s_extraLines++;
      return Game::userErrors::command_formating;
    }
    std::string_view to = command.substr(to_start, start - to_start);
    Game::userErrors error{game.move(from, to)};
    if (error == Game::userErrors::no_error) {
      clearPreviousLines(gameLineSize + 2 * s_extraLines);
      s_extraLines = 0;
      std::cout << game << std::endl;
    } else {
      s_extraLines++;
    }
    return error;
  }
  case 'd': // draw
  {
    game.draw();
    clearPreviousLines(3 + game.longestCardStackLen() + 2 * s_extraLines);
    s_extraLines = 0;
    std::cout << game << std::endl;
    return {};
  }
  case 'c': // clear
  {
    clearPreviousLines(3 + game.longestCardStackLen() + 2 * s_extraLines);
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
      s_extraLines++;
      return Game::userErrors::command_formating;
    }
    std::string fileName{command.substr(from_start, start - from_start)};

    std::ofstream file(fileName);
    if (!file.is_open()) {
      s_extraLines++;
      return Game::userErrors::file_not_found;
    }

    file << j.dump(2);

    clearPreviousLines(3 + game.longestCardStackLen() + 2 * s_extraLines);
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
      s_extraLines++;
      return Game::userErrors::command_formating;
    }
    std::string fileName{command.substr(from_start, start - from_start)};

    std::ifstream file(fileName);
    if (!file.is_open()) {
      s_extraLines++;
      return Game::userErrors::file_not_found;
    }

    nlohmann::json j;
    file >> j;
    game = j.get<Game>();

    clearPreviousLines(3 + game.longestCardStackLen() + 2 * s_extraLines);
    s_extraLines = 0;
    std::cout << game << std::endl;
    return {};
  }
  default: {
    s_extraLines++;
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
