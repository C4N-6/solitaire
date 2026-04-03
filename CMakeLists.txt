cmake_minimum_required(VERSION 4.1.2)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(solitaire LANGUAGES CXX VERSION 4.0.0)

find_package(PkgConfig REQUIRED)
pkg_check_modules(READLINE REQUIRED IMPORTED_TARGET readline)

find_package(nlohmann_json REQUIRED)
find_package(argparse REQUIRED)

set(CMAKE_CXX_STANDARD 17)

set(PROJECT_SOURCES
    src/card.cpp
    src/deck.cpp
    src/main.cpp
    src/SuitPile.cpp
    src/Game.cpp
    src/cardStack.cpp
    src/stats.cpp
    src/leaderBoard.cpp
)

add_definitions(-DPROJECT_VERSION="${PROJECT_VERSION}")

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE
    PkgConfig::READLINE
    nlohmann_json::nlohmann_json
    argparse::argparse
)

target_include_directories(${PROJECT_NAME} PRIVATE ${READLINE_INCLUDE_DIRS})

target_compile_definitions(${PROJECT_NAME} PRIVATE $<$<CONFIG:Debug>:DEBUG>)

install(TARGETS ${PROJECT_NAME} DESTINATION bin)

add_custom_target(
    run
    COMMAND ${PROJECT_NAME}
    DEPENDS ${PROJECT_NAME}
    COMMENT "Ruining ${PROJECT_NAME}"
)

add_custom_target(
    version
    COMMAND ${PROJECT_NAME} -v
    DEPENDS ${PROJECT_NAME}
    COMMENT "getting version"
)

add_custom_target(
    showHelp
    COMMAND ${PROJECT_NAME} -h
    DEPENDS ${PROJECT_NAME}
    COMMENT "getting version"
)
