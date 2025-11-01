
#ifndef __MAIN_H_
#define __MAIN_H_
#include "game.h"

Game::userErrors game_command(Game &game, const std::string_view command);

void endingAnimation(Game &game);

inline void clearPreviousLines(int lines) {
  for (int i = 0; i < lines; ++i) {
    std::cout << "\033[F\033[2K"; // Move up and clear line
  }
}

#endif // !__MAIN_H_
