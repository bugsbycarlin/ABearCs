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

const int num_spelling_words = 42;
const int num_drums = 19;

class Spelling : public Screen {
 public:
  Spelling();

  void loop();

  void initialize();

  void logic();
  void render();

  void setDrumsAndRhymes();
  void rhyme(int i, float f);

  ~Spelling();

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

  std::string words[num_spelling_words] = {
    "rat",
    "hat",
    "dog",
    "log",
    "toy",
    "boy",
    "dot",
    "tot",
    "jar",
    "car",
    "bat",
    "cat",
    "bun",
    "nun",
    "pig",
    "jig",
    "cop",
    "mop",
    "job",
    "sob",
    "pet",
    "jet",
    "cog",
    "hog",
    "leg",
    "egg",
    "ham",
    "jam",
    "bug",
    "rug",
    "hen",
    "ten",
    "pug",
    "hug",
    "cap",
    "map",
    "man",
    "pan",
    "fox",
    "box",
    "yak",
    "mac"
  };

  const array<string, 10> color_names = {
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

  array<string, 10> color_values;

  Textbox* word_text;

  int screen_width;
  int screen_height;

  std::string screen_color;

  float last_drum;

  int word_num;
  int drum_num;
};