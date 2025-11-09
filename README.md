# Solitaire

## Description

A program to play solitaire

## Requirements

- cmake: Version >= 4.1.2

### Dependencies

- [argparse/argparse](https://github.com/p-ranav/argparse)
- [nlohmann/json](https://github.com/nlohmann/json)

## Build

```bash
cmake -B build
cmake --build build -- all
```

## Usage

### Run

```bash
# run trough cmake
cmake --build build -- run
# run normally
./build/solitaire
```
