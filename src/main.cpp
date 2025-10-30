#include "game.h"
#include <argparse/argparse.hpp>
#include <iostream>
#include <ostream>
#include <sstream>
#include <string>

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
  default: {
    s_extraLines++;
    return Game::userErrors::command_not_found;
  }
  }
}

int main(int argc, char *argv[]) {

  argparse::ArgumentParser arg{};

  arg.add_argument("-v").help("Shows version").flag();
  arg.add_argument("-s", "--seed").help("add a default seed").default_value("");

  try {
    arg.parse_args(argc, argv);
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

  if (arg.get("-s").empty()) {
    game = Game{};
  } else {
    unsigned long seed{};
    std::stringstream{arg.get("-s")} >> seed;
    game = Game{seed};
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
  clearPreviousLines(2);
  std::cout << "You win!!" << std::endl;
  std::cout << game.getStats() << std::endl;
endGame:
  return 0;
}
