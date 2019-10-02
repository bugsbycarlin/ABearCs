/*
  BearsLoveHoney
  Matthew Carlin
  Copyright 2018
*/

#include "abearcs.h"

using namespace std;
using namespace Honey;

void debug(int x) {
  //if (false) {
  printf("Here %d\n", x);
  //}
}

ABearCs::ABearCs() {
}

void ABearCs::initialize() {
  screen_width = hot_config.getInt("layout", "screen_width");
  screen_height = hot_config.getInt("layout", "screen_height");
  screen_color = hot_config.getString("layout", "screen_color");
  picture_x = hot_config.getInt("layout", "abearcs_picture_x");
  picture_y = hot_config.getInt("layout", "abearcs_picture_y");
  letter_x = hot_config.getInt("layout", "abearcs_letter_x");
  letter_y = hot_config.getInt("layout", "abearcs_letter_y");
  word_x = hot_config.getInt("layout", "abearcs_word_x");
  word_y = hot_config.getInt("layout", "abearcs_word_y");
  tween_type = hot_config.getInt("animation", "tween_type");

  letters = {
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

  pictures = {};
  // "balloon"
  // "dog"
  // "flowers"
  // "grapes"
  pictures["a"] = {"apple", "airplane"};
  pictures["b"] = {"bee", "bear", "ball", "bus", "bike"};
  pictures["c"] = {"carrot", "cloud", "crayons", "car", "cat", "cow"};
  pictures["d"] = {"drum", "dinosaur", "ducks"};
  pictures["e"] = {"elephant", "eggs"};
  pictures["f"] = {"feather", "flowers"};
  pictures["g"] = {"goat", "grapes"};
  pictures["h"] = {"hamburger"};
  pictures["i"] = {"iguana", "icecream"};
  pictures["j"] = {"juice"};
  pictures["k"] = {"koala", "key"};
  pictures["l"] = {"lamp"};
  pictures["m"] = {"monkey", "moon"};
  pictures["n"] = {"noodles"};
  pictures["o"] = {"orange", "octopus"};
  pictures["p"] = {"phone"};
  pictures["q"] = {"quilt"};
  pictures["r"] = {"rhino", "rainbow"};
  pictures["s"] = {"sad"};
  pictures["t"] = {"truck"};
  pictures["u"] = {"umbrella"};
  pictures["v"] = {"vulture"};
  pictures["w"] = {"watermelon", "wolf"};
  pictures["x"] = {"xylophone"};
  pictures["y"] = {"yoga"};
  pictures["z"] = {"zebra"};

  pictures["shapes"] = {
    "arrow",
    "circle",
    "diamond",
    "heart",
    "hexagon",
    "octagon",
    "pentagon",
    "square",
    "star",
    "triangle"
  };

  colors = {
    "#77dd77",
    "#ff4941",
    "#c0c0c0",
    "#5284bd",
    "#917394",
    "#ffa82e",
    "#ffd700",
    "#444444",
    "#ffb6c1",
    "#835c3b"
  };

  for (int i = 0; i < 26; i++) {
    string letter = letters[i];
    sound.addSound(letter, "Sound/Letters/" + letter + ".wav");
    for (int j = 0; j < pictures[letter].size(); j++) {
      string picture = pictures[letter][j];
      graphics.addImage(picture, "Art/" + picture + ".png");
    }
  }
  graphics.addImages("Art/", pictures["shapes"]);

  effects.makeOscillation("squish", hot_config.getFloat("animation", "squish_size"), hot_config.getFloat("animation", "squish_cycle"));
  effects.start("squish");

  letter_box = new Textbox(
    hot_config.getString("layout", "font_name"),
    hot_config.getInt("layout", "letter_size"),
    "X",
    (position) {letter_x, letter_y},
    "#000000"
  );

  word_box = new Textbox(
    hot_config.getString("layout", "font_name"),
    hot_config.getInt("layout", "word_size"),
    "X",
    (position) {word_x, word_y},
    "#000000"
  );

  mode = "spelling";

  current_word = "";
  current_picture = "";
}

void ABearCs::logic() {
  float animation_duration = hot_config.getFloat("animation", "animation_duration");
  float shake_duration = hot_config.getFloat("animation", "shake_duration");
  int shake_width = hot_config.getInt("animation", "shake_width");
  float key_swap_duration = hot_config.getFloat("input", "key_swap_duration");
  float sound_volume = hot_config.getFloat("sound", "sound_volume");
  float music_volume = hot_config.getFloat("sound", "music_volume");

  sound.setSoundVolume(sound_volume);
  sound.setSoundVolume(music_volume);

  if (mode == "spelling" && !input.locked()) {
    for (int i = 0; i < 26; i++) {
      if (input.keyDown(letters[i])) {
        current_word += letters[i];
        input.lockInput(0.1);
      }
    }

    if (input.keyDown("space")) {
      current_word = "";
      current_picture = "";
      input.lockInput(0.1);
    }

    if (input.keyDown("return")) {
      printf("In here\n");
      if (graphics.checkImage(current_word)) {
        printf("In here too\n");
        current_picture = current_word;
      }
    }
  }

  if (input.threeQuickKey("escape")) {
    screenmanager.setQuit();
  }
}

void ABearCs::render() {
  graphics.clearScreen(screen_color);
  graphics.draw2D();

  if (mode == "spelling") {
    if (current_word.length() > 0) {
      //boost::to_upper(current_word);
      graphics.setColor("#000000", 1.0);
      string output_word = boost::to_upper_copy(current_word);
      letter_box->setText(output_word);
      letter_box->setPosition({letter_x, letter_y});
      letter_box->draw();
    }

    if (current_picture.length() > 0 && graphics.checkImage(current_picture)) {
      graphics.setColor("#FFFFFF", 1.0);
      graphics.pushModelMatrix();
      graphics.scale(1.0 + effects.oscillation("squish"), 1.0 - effects.oscillation("squish"), 1.0);
      graphics.translate(-400 * effects.oscillation("squish"), 600 * effects.oscillation("squish"), 0);
      graphics.drawImage(current_word, picture_x, picture_y);
      graphics.popModelMatrix();
    }
  }

}

ABearCs::~ABearCs() {
}