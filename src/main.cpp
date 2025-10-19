#include "game.h"
#include <argparse/argparse.hpp>
#include <iostream>
#include <ostream>
#include <sstream>
#include <string>

int main(int argc, char *argv[]) {

  argparse::ArgumentParser arg{};

  arg.add_argument("-v").help("Shows version").flag();
  arg.add_argument("-s", "--seed").help("add a default seed").default_value("");

  try {
    arg.parse_args(argc, argv); // Example: ./main -abc 1.95 2.47
  } catch (const std::exception &err) {
    std::cerr << err.what() << std::endl;
    std::cerr << arg;
    return 1;
  }
  if (arg.get<bool>("-v")) {
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

    Game::userErrors commandError{game.command(command)};
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
