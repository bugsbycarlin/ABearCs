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

  pictures = {
    "apple", "airplane", "arrow",
    "bee", "bear", "ball", "bus", "bike",
    "carrot", "cloud", "crayons", "car", "cat", "cow", "circle",
    "drum", "dinosaur", "ducks", "diamond", "dog",
    "elephant", "eggs",
    "feather", "flowers",
    "goat", "grapes",
    "hamburger", "heart", "hexagon", "horse",
    "iguana", "icecream",
    "juice",
    "koala", "key",
    "lamp",
    "monkey", "moon",
    "noodles",
    "orange", "octopus", "octagon",
    "phone", "pentagon", "puppy", "pig",
    "quilt",
    "rhino", "rainbow",
    "sad", "square", "star", "sheep", "sun",
    "truck", "triangle",
    "umbrella",
    "vulture",
    "watermelon", "wolf",
    "xylophone",
    "yoga",
    "zebra"
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

  graphics.addImages("Art/", pictures);

  effects.makeOscillation("squish", hot_config.getFloat("animation", "squish_size"), hot_config.getFloat("animation", "squish_cycle"));
  effects.start("squish");

  letter_box = new Textbox(
    hot_config.getString("layout", "font_name"),
    hot_config.getInt("layout", "letter_size"),
    "X",
    (position) {letter_x, letter_y},
    "#000000"
  );

  small_letter_box = new Textbox(
    hot_config.getString("layout", "font_name"),
    hot_config.getInt("layout", "small_letter_size"),
    "X",
    (position) {letter_x, letter_y},
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

    if (input.keyDown("backspace") && current_word.length() > 0) {
      current_word = current_word.substr(0, current_word.length() - 1);
      input.lockInput(0.1);
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
      if (current_word.length() < 5) {
        letter_box->setText(output_word);
        letter_box->setPosition({letter_x, letter_y});
        letter_box->draw();
      } else {
        small_letter_box->setText(output_word);
        small_letter_box->setPosition({(int) (letter_x - current_word.length() * 10), letter_y});
        small_letter_box->draw();
      }
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