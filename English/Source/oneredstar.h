/*
  BearsLoveHoney
  Matthew Carlin
  Copyright 2018
*/

#pragma once

#include <string>
#include <functional>
#include <array>
#include <vector>
#include <math.h>

#include "honey.h"

using namespace Honey;
using namespace std;

const int max_number = 10;

class OneRedStar : public Screen {
 public:
  OneRedStar();

  void loop();

  void initialize();
  void initializeAssets();
  void initializeLogic();

  void logic();
  void inputLogic();
  void animationSequence(int sequence_counter, float duration);
  int randomOutside(int size);
  void shuffle(int new_number);
  void remake();

  void render();

  void cleanup();

  ~OneRedStar();

  int screen_width;
  int screen_height;

  std::string screen_color;

  float fade_in_duration;
  float word_duration;
  float fade_out_duration;
  float fade_out_stagger;
  float shake_width;

  const array<string, max_number> shapes = {
    "Triangle",
    "Square",
    "Pentagon",
    "Hexagon",
    "Octagon",
    "Circle",
    "Star",
    "Arrow",
    "Diamond",
    "Heart"
  };

  const array<string, max_number> color_names = {
    "Green",
    "Red",
    "Grey",
    "Blue",
    "Purple",
    "Orange",
    "Yellow",
    "Black",
    "Pink",
    "Brown"
  };

  array<string, max_number> color_values;

  array<Sprite*, max_number> shape_sprites;
  Textbox* number_text;
  Textbox* color_text;
  Textbox* shape_text;

  vector<string> sequence_action;
  vector<float> sequence_timing;

  int number;
  string shape;
  string color_name;
  string color_value;
  float size;

};