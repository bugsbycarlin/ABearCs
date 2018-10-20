/*
  BearsLoveHoney
  Matthew Carlin
  Copyright 2018
*/

#pragma once

#include <string>
#include <array>

#include "honey.h"

using namespace Honey;
using namespace std;

const int num_keyboard_drums = 19;

class Keyboard : public Screen {
 public:
  Keyboard();

  void loop();

  void initialize();

  void logic();
  void render();

  void setDrums();

  ~Keyboard();

  const array<string, 5> key_colors = {
    "red",
    "orange",
    "yellow",
    "green",
    "blue",
  };

  const array<string, 5> key_names = {
    "C",
    "D",
    "E",
    "F",
    "G"
  };

  const array<string, 5> reward_bears = {
    "magnet_bear",
    "grim_bear",
    "boss_bear",
    "lawn_dart_bear",
    "explorer_bear"
  };
  

  int screen_width;
  int screen_height;

  std::string screen_color;

  float last_drum;

  int drum_num;

  int key_num;
};