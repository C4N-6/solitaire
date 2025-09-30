#include "game.h"
#include <iostream>
#include <string>

int main(int argc, char *argv[]) {
  Game game{};
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
endGame:
  return 0;
}
