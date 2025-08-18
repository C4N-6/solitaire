cmake_minimum_required(VERSION 4.1.0)

project(Solitare CXX)

set(CMAKE_CXX_STANDARD 17)

set(PROJECT_SOURCES
  src/card.cpp
  src/deck.cpp
  src/main.cpp
  src/SuitPile.cpp
  src/Game.cpp
)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

add_custom_target(
  run
  COMMAND ${PROJECT_NAME}
  DEPENDS ${PROJECT_NAME}
  COMMENT "Runing ${PROJECT_NAME}"
)
