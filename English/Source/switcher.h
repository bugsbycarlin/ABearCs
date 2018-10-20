/*
  BearsLoveHoney
  Matthew Carlin
  Copyright 2018
*/

#pragma once

#include <string>
#include <array>

#include "honey.h"
#include "abearcs.h"
#include "oneredstar.h"
#include "puzzle.h"
#include "spelling.h"
#include "keyboard.h"
#include "countup.h"

using namespace Honey;
using namespace std;

class Switcher {
 public:
  Switcher();

  void render();
  void logic();

  void makeOrSet(string screen_name);

  int switch_counter;
  int max_screen;

  ~Switcher();

  Textbox* nav_text;
};