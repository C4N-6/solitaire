cmake_minimum_required(VERSION 4.1.0)

project(solitaire LANGUAGES CXX VERSION 1.0.1)

set(CMAKE_CXX_STANDARD 17)

set(PROJECT_SOURCES
  src/card.cpp
  src/deck.cpp
  src/main.cpp
  src/SuitPile.cpp
  src/Game.cpp
  src/cardStack.cpp
)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

install(
  TARGETS ${PROJECT_NAME}
  DESTINATION bin
)
add_custom_target(
  run
  COMMAND ${PROJECT_NAME}
  DEPENDS ${PROJECT_NAME}
  COMMENT "Runing ${PROJECT_NAME}"
)
