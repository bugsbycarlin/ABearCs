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

  void logic();
  void render();

  ~ABearCs();

  vector<string> letters;

  vector<string> pictures;

  Textbox* letter_box;
  Textbox* small_letter_box;

  vector<string> colors;

  string mode;

  int screen_width;
  int screen_height;

  string current_word;
  string current_picture;

  std::string screen_color;
  int picture_x;
  int picture_y;
  int letter_x;
  int letter_y;
  int word_x;
  int word_y;
  int tween_type;
};