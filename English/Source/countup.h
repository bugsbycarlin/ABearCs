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

class CountUp : public Screen {
 public:
  CountUp();

  void loop();

  void initialize();

  void logic();
  void render();

  ~CountUp();

  std::string letters[26] = {
    "a",
    "b",
    "c",
    "d",
    "e",
    "f",
    "g",
    "h",
    "i",
    "j",
    "k",
    "l",
    "m",
    "n",
    "o",
    "p",
    "q",
    "r",
    "s",
    "t",
    "u",
    "v",
    "w",
    "x",
    "y",
    "z"
  };

  std::string pictures[26] = {
    "apple",
    "bear",
    "cat",
    "dog",
    "eggs",
    "flowers",
    "grapes",
    "hamburger",
    "icecream",
    "juicebox",
    "key",
    "lemon",
    "monkey",
    "nixon",
    "orange",
    "pizza",
    "queen",
    "rainbow",
    "strawberry",
    "trees",
    "umbrella",
    "viola",
    "watermelon",
    "xylophone",
    "yoga",
    "zebra"
  };

  int screen_width;
  int screen_height;

  std::string screen_color;

  int current_number;
  int modulus;

  Sprite* takei;
  Sprite* bear[3];
  Textbox* number_text;
};