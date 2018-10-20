/*
  BearsLoveHoney
  Matthew Carlin
  Copyright 2018
*/

#include "keyboard.h"

using namespace std;
using namespace Honey;

Keyboard::Keyboard() {
}

void Keyboard::loop() {
  logic();
  render();
}

void Keyboard::initialize() {
  screen_width = hot_config.getInt("layout", "screen_width");
  screen_height = hot_config.getInt("layout", "screen_height");

  graphics.addImages("Art/Keyboard/", {
    "keyboard",
    "red_keyboard",
    "orange_keyboard",
    "yellow_keyboard",
    "green_keyboard",
    "blue_keyboard",
    "drum_1",
    "drum_2"
  });

  graphics.addImages("Art/", {
    "magnet_bear",
    "grim_bear",
    "boss_bear",
    "emo_bear",
    "lawn_dart_bear",
    "explorer_bear"
  });

  sound.addSounds("Sound/Keyboard/", {
    "C_BellRiser",
    "C_tone",
    "D_BellRiser",
    "D_tone",
    "daydreaming",
    "E_BellRiser",
    "E_tone",
    "F_BellRiser",
    "F_tone",
    "G_BellRiser",
    "G_tone",
    "tremendous"
  });

  for (int i = 1; i <= num_keyboard_drums; i++) {
    sound.addSound("drums_" + to_string(i), "Sound/drums_" + to_string(i) + ".wav");
  }

  key_num = 0;
  last_drum = -4;
  drum_num = 0;
  timing.mark("drum_timing");

  setDrums();
}

void Keyboard::logic() {
  // Check and load configuration every 2 seconds
  hot_config.checkAndUpdate();

  setDrums();

  // Set a bunch of variables from configuration
  screen_color = hot_config.getString("layout", "screen_color");

  sound.setSoundVolume(hot_config.getFloat("sound", "sound_volume"));

  if (input.keyDown("a") && !timing.locked("note")) {
    timing.lock("note", 0.5);
    sound.playSound("C_tone", 1);
    if (key_num == 0) {
      sound.playSound("C_BellRiser", 1);

      timing.lock("reward", 1.0);
    }
  }

  if (input.keyDown("s") && !timing.locked("note")) {
    timing.lock("note", 0.5);
    sound.playSound("D_tone", 1);
    if (key_num == 1) {
      sound.playSound("D_BellRiser", 1);

      timing.lock("reward", 1.0);
    }
  }

  if (input.keyDown("d") && !timing.locked("note")) {
    timing.lock("note", 0.5);
    sound.playSound("E_tone", 1);
    if (key_num == 2) {
      sound.playSound("E_BellRiser", 1);

      timing.lock("reward", 1.0);
    }
  }

  if (input.keyDown("f") && !timing.locked("note")) {
    timing.lock("note", 0.5);
    sound.playSound("F_tone", 1);
    if (key_num == 3) {
      sound.playSound("F_BellRiser", 1);

      timing.lock("reward", 1.0);
    }
  }

  if (input.keyDown("g") && !timing.locked("note")) {
    timing.lock("note", 0.5);
    sound.playSound("G_tone", 1);
    if (key_num == 4) {
      sound.playSound("G_BellRiser", 1);

      timing.lock("reward", 1.0);
    }
  }

  if (input.threeQuickKey("escape")) {
    screenmanager.setQuit();
  }
}

void Keyboard::setDrums() {
  if (timing.since("drum_timing") - last_drum >= 4) {
    drum_num = math_utils.randomInt(1, num_keyboard_drums);
    sound.playSound("drums_" + to_string(drum_num), 1);
    last_drum += 4;

    key_num = math_utils.randomInt(0, 5);
    sound.playSound(key_names[key_num] + "_tone", 1);

    timing.remove("reward");
  }
}

void Keyboard::render() {
  // Clear the screen to a soft white color
  graphics.clearScreen(screen_color);

  int phase = (int) (timing.since("drum_timing") * 2);
  int rotation = 45;
  int dance = 15;
  if (phase % 2 == 0) {
    rotation = -45;
    dance = -15;
  }
  graphics.drawImage("drum_1", 860 + dance, 120, true, rotation, 0.35);
  graphics.drawImage("drum_2", 110 + dance, 150, true, rotation, 0.25);

  if (((int) (timing.since("drum_timing") * 8)) % 2 == 1) {
    graphics.drawImage(key_colors[key_num] + "_keyboard", 0, 0);
  } else {
    graphics.drawImage("keyboard", 0, 0);
  }

  if (timing.locked("reward")) {
    dance = 20;
    if (((int) (timing.since("drum_timing") * 8)) % 2 == 1) {
      dance = -20;
    }
    for (int i = 120; i < 880; i += 165) {
      graphics.drawImage(reward_bears[key_num], i + dance, 490, true, 0, 1);
    }
  }

  // Switch to 2D drawing mode
  graphics.draw2D();
}

Keyboard::~Keyboard() {
}