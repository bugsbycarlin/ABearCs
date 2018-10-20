/*
  BearsLoveHoney
  Matthew Carlin
  Copyright 2018
*/

#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>

#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>

#include "honey.h"

using namespace Honey;
using namespace std;

const int puzzle_width = 5;
const int puzzle_height = 4;

const int cut_size = 40;

class Puzzle : public Screen {
 public:
  Puzzle();

  void loop();

  void initialize();

  void logic();
  void render();

  void jumble();

  void generate(int number);

  ~Puzzle();

  int screen_width;
  int screen_height;

  int piece_width;
  int piece_height;

  //array<array<string, puzzle_width>, puzzle_height> puzzle_labels;

  unordered_map<string, position> original_positions;
  vector<Sprite*> puzzle_pieces;
  vector<Sprite*> shuffled_puzzle_pieces;

  std::string screen_color;
};