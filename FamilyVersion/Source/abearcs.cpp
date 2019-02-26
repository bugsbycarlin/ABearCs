/*
  BearsLoveHoney
  Matthew Carlin
  Copyright 2018
*/

#include "abearcs.h"

using namespace std;
using namespace Honey;

ABearCs::ABearCs() {
}

void ABearCs::loop() {
  logic();
  render();
}

void ABearCs::initialize() {
  screen_width = hot_config.getInt("layout", "screen_width");
  screen_height = hot_config.getInt("layout", "screen_height");

  current_letter = "";
  last_letter = "";

  for (int i = 0; i < 26; i++) {
    graphics.addImage(letters[i], "Art/" + letters[i] + ".png");
    graphics.addImage(pictures[i], "Art/" + pictures[i] + ".png");
    sound.addSound(letters[i] + "_" + pictures[i], "Sound/" + letters[i] + "_" + pictures[i] + ".wav");
    key_to_letter[letters[i]] = {letters[i]};
    letter_to_sound[letters[i]] = letters[i] + "_" + pictures[i];
    letter_to_picture[letters[i]] = pictures[i];
  }

  graphics.addImages("Art/", {
    "baobao",
    "baobao_picture",
    "mama",
    "mama_picture",
    "baba",
    "baba_picture",
    "jiajia",
    "jiajia_picture",
    "laolao",
    "laolao_picture",
    "laoye",
    "laoye_picture",
    "nainai",
    "nainai_picture",
    "yeye",
    "yeye_picture",
    "gougou",
    "gougou_picture"
  });

  sound.addSound("baba", "Sound/baba.wav");
  sound.addSound("baobao", "Sound/baobao.wav");
  sound.addSound("gougou", "Sound/gougou.wav");
  sound.addSound("jiajia", "Sound/jiajia.wav");
  sound.addSound("laolao", "Sound/laolao.wav");
  sound.addSound("laoye", "Sound/laoye.wav");
  sound.addSound("mama", "Sound/mama.wav");
  sound.addSound("nainai", "Sound/nainai.wav");
  sound.addSound("yeye", "Sound/yeye.wav");

  key_to_letter["b"].push_back("baba");
  key_to_letter["b"].push_back("baobao");
  key_to_letter["g"].push_back("gougou");
  key_to_letter["j"].push_back("jiajia");
  key_to_letter["l"].push_back("laolao");
  key_to_letter["l"].push_back("laoye");
  key_to_letter["m"].push_back("mama");
  key_to_letter["n"].push_back("nainai");
  key_to_letter["y"].push_back("yeye");

  letter_to_picture["baba"] = "baba_picture";
  letter_to_picture["baobao"] = "baobao_picture";
  letter_to_picture["gougou"] = "gougou_picture";
  letter_to_picture["jiajia"] = "jiajia_picture";
  letter_to_picture["laolao"] = "laolao_picture";
  letter_to_picture["laoye"] = "laoye_picture";
  letter_to_picture["mama"] = "mama_picture";
  letter_to_picture["nainai"] = "nainai_picture";
  letter_to_picture["yeye"] = "yeye_picture";

  letter_to_sound["baba"] = "baba";
  letter_to_sound["baobao"] = "baobao";
  letter_to_sound["gougou"] = "gougou";
  letter_to_sound["jiajia"] = "jiajia";
  letter_to_sound["laolao"] = "laolao";
  letter_to_sound["laoye"] = "laoye";
  letter_to_sound["mama"] = "mama";
  letter_to_sound["nainai"] = "nainai";
  letter_to_sound["yeye"] = "yeye";
}

void ABearCs::logic() {
  // Check and load configuration every 2 seconds
  hot_config.checkAndUpdate();

  // Set a bunch of variables from configuration
  screen_color = hot_config.getString("layout", "screen_color");
  picture_x = hot_config.getInt("layout", "abearcs_picture_x");
  picture_y = hot_config.getInt("layout", "abearcs_picture_y");
  text_x = hot_config.getInt("layout", "abearcs_text_x");
  text_y = hot_config.getInt("layout", "abearcs_text_y");
  tween_type = hot_config.getInt("animation", "tween_type");
  float animation_duration = hot_config.getFloat("animation", "animation_duration");
  float shake_duration = hot_config.getFloat("animation", "shake_duration");
  int shake_width = hot_config.getInt("animation", "shake_width");
  float key_swap_duration = hot_config.getFloat("input", "key_swap_duration");
  float sound_volume = hot_config.getFloat("sound", "sound_volume");
  float sound_lock_duration = hot_config.getFloat("sound", "sound_lock_duration");

  sound.setSoundVolume(sound_volume);

  if (current_letter == "") {
    timing.remove("sound_lock");
    timing.lock("key_swap", key_swap_duration);
    timing.lock("animation", animation_duration);
    effects.makeTween("slide_last_letter", screen_height, 0, animation_duration);
    effects.start("slide_last_letter");
    last_letter = current_letter;
    current_letter = letters[math_utils.randomInt(0,25)];
  }

  // do stuff with input
  for (int i = 0; i < 26; i++) {
    if (input.keyDown(letters[i]) && !timing.locked("key_swap")) {
      //if (key_to_letter[letters[i]].size() > 1 || letters[i] != current_letter) {
      timing.remove("sound_lock");
      timing.lock("key_swap", key_swap_duration);
      timing.lock("animation", animation_duration);
      effects.makeTween("slide_last_letter", screen_height, 0, animation_duration);
      effects.start("slide_last_letter");
      last_letter = current_letter;
      int size = key_to_letter[letters[i]].size();
      printf("The size is %lu\n", key_to_letter[letters[i]].size());
      current_letter = key_to_letter[letters[i]][math_utils.randomInt(0, size)];
      printf("The letter is %s\n", current_letter.c_str());
      //}
    }
  }

  if (timing.locked("key_swap") && !timing.locked("animation") && !timing.locked("sound_lock")) {
    timing.lock("sound_lock", sound_lock_duration);
    sound.playSound(letter_to_sound[current_letter], 1);
    effects.makeShake("sound_shake", shake_width, shake_duration);
    effects.start("sound_shake");
  }

  if (input.threeQuickKey("escape")) {
    screenmanager.setQuit();
  }
}

void ABearCs::render() {
  // Clear the screen to a soft white color
  graphics.clearScreen(screen_color);

  // Switch to 2D drawing mode
  graphics.draw2D();


  // draw stuff
  float x, y;

  // Current picture
  if (current_letter != "") {
    // string picture = "";
    // if (current_letter.length() == 1) {
    //   picture = key_to_picture[current_letter][0];
    // } else {
    //   picture = current_letter + "_picture";
    // }
    x = picture_x;
    y = picture_y;
    if (timing.locked("animation")) {
      y += effects.tween("slide_last_letter", tween_type);
    }
    graphics.drawImage(letter_to_picture[current_letter], x, y);
    x = text_x;
    y = text_y;
    if (timing.locked("animation")) {
      y += effects.tween("slide_last_letter", tween_type);
    }
    if (timing.locked("key_swap")) {
      x += effects.shake("sound_shake");
      y += effects.shake("sound_shake");
    }
    graphics.drawImage(current_letter, x, y);
  }

  // Last picture, if animating
  if (timing.locked("animation") && last_letter != "") {
    // string picture = "";
    // if (last_letter.length() == 1) {
    //   picture = key_to_picture[last_letter][0] + "_" + pictures[last_letter];
    // } else {
    //   picture = last_letter + "_picture";
    // }
    x = picture_x;
    y = picture_y - screen_height + effects.tween("slide_last_letter", tween_type);
    graphics.drawImage(letter_to_picture[last_letter], x, y);
    x = text_x;
    y = text_y - screen_height + effects.tween("slide_last_letter", tween_type);
    graphics.drawImage(last_letter, x, y);
  }
}

ABearCs::~ABearCs() {

}