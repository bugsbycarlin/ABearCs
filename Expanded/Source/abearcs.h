/*
  BearsLoveHoney
  Matthew Carlin
  Copyright 2018
*/

#pragma once

#include <array>
#include <map>
#include <string>

#include <boost/algorithm/string.hpp>

#include "honey.h"

using namespace Honey;
using namespace std;

struct wall_position {
  int x;
  int y;
  int last_x;
  int last_y;
};

class ABearCs : public Screen {
 public:
  ABearCs();

  void initialize();

  void initializeMaze();
  void renderMaze(int x, int y);

  void logic();
  void render();

  ~ABearCs();

  vector<string> letters;

  map<string, vector<string>> pictures;

  Textbox* letter_box;
  Textbox* word_box;

  vector<string> colors;

  // std::string pictures[26] = {
  //   "apple",
  //   "bear",
  //   "cat",
  //   "dog",
  //   "eggs",
  //   "flowers",
  //   "grapes",
  //   "hamburger",
  //   "icecream",
  //   "juicebox",
  //   "key",
  //   "lemon",
  //   "monkey",
  //   "nixon",
  //   "orange",
  //   "pizza",
  //   "queen",
  //   "rainbow",
  //   "strawberry",
  //   "trees",
  //   "umbrella",
  //   "viola",
  //   "watermelon",
  //   "xylophone",
  //   "yoga",
  //   "zebra"
  // };

  string mode;

  int screen_width;
  int screen_height;

  string last_letter;
  string current_letter;
  string last_word;
  string current_word;
  string last_color;
  string current_color;

  vector<vector<int>> maze;
  vector<vector<int>> runner_visited;
  vector<position> runner_positions;
  Sprite* runner_sprite;

  std::string screen_color;
  int picture_x;
  int picture_y;
  int letter_x;
  int letter_y;
  int word_x;
  int word_y;
  int tween_type;
};