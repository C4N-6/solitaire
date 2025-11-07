cmake_minimum_required(VERSION 4.1.2)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(solitaire LANGUAGES CXX VERSION 2.0.1)

set(CMAKE_CXX_STANDARD 17)

set(PROJECT_SOURCES
    src/card.cpp
    src/deck.cpp
    src/main.cpp
    src/SuitPile.cpp
    src/Game.cpp
    src/cardStack.cpp
)

add_definitions(-DPROJECT_VERSION="${PROJECT_VERSION}")

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

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
