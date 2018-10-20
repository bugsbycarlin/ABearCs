/*
  BearsLoveHoney
  Matthew Carlin
  Copyright 2018
*/

#include "countup.h"

using namespace std;
using namespace Honey;

CountUp::CountUp() {
}

void CountUp::loop() {
  logic();
  render();
}

void CountUp::initialize() {
  screen_width = hot_config.getInt("layout", "screen_width");
  screen_height = hot_config.getInt("layout", "screen_height");

  current_number = 0;
  modulus = math_utils.randomInt(3,11);

  for (int i = 1; i <= 100; i++) {
    sound.addSound("countup_" + to_string(i), "Sound/CountUp/" + to_string(i) + ".wav");
  }

  graphics.addImage("takei", "Art/takei.png");
  graphics.addImage("boss_bear", "Art/boss_bear.png");

  sound.addSound("countup_success", "Sound/Keyboard/C_BellRiser.wav");

  takei = new Sprite(
    "takei",
    (position) {
      hot_config.getInt("layout", "countup_picture_x"),
      hot_config.getInt("layout", "countup_picture_y")
    },
    "#ffffff", 1.0, 0.0, 1.0
  );

  for (int i = 0; i < 3; i++) {
    bear[i] = new Sprite(
      "boss_bear",
      (position) {
        100 + 200 * i,
        500
      },
      "#ffffff", 1.0, 0.0, 1.0
    );
  }


  number_text = new Textbox(
    hot_config.getString("layout", "countup_font"),
    hot_config.getInt("layout", "countup_font_size"),
    "0",
    (position) {
      hot_config.getInt("layout", "countup_number_x"),
      hot_config.getInt("layout", "countup_number_y")
    },
    "#000000"
  );
}

void CountUp::logic() {
  // Check and load configuration every 2 seconds
  hot_config.checkAndUpdate();

  // Set a bunch of variables from configuration
  screen_color = hot_config.getString("layout", "screen_color");

  sound.setSoundVolume(hot_config.getFloat("sound", "sound_volume"));

  if (input.anyKeyPressed() && !timing.locked("countup_key_lock")) {
    current_number++;
    if (current_number == 101) {
      current_number = 1;
    }
    timing.lock("countup_key_lock", 0.5);
    takei->shakePosition(10, 0.25);
    number_text->setText(to_string(current_number));
    number_text->setColor("#000000");
    sound.playSound("countup_" + to_string(current_number), 1);

    int x_position = hot_config.getInt("layout", "countup_number_x");
    if (current_number >= 10 && current_number < 100) {
      x_position -= hot_config.getInt("layout", "countup_number_spacing");
    } else if (current_number == 100) {
      x_position -= 2 * hot_config.getInt("layout", "countup_number_spacing");
    }
    number_text->setPosition({
      x_position,
      hot_config.getInt("layout", "countup_number_y")
    });

    for (int i = 0; i < 3; i++) {
      bear[i]->setPosition({
        100 + 250 * i + 300,
        500
      }, 0.5);
    }

    if (current_number % modulus == 0) {
      number_text->setColor("#6495ED");
      sound.playSound("countup_success", 1);
    }
  }

  if (!timing.locked("countup_key_lock")) {
    for (int i = 0; i < 3; i++) {
      bear[i]->setPosition({
        100 + 200 * i,
        500
      });
      bear[i]->setRotation(0);
    }
  }

  if (input.threeQuickKey("escape")) {
    screenmanager.setQuit();
  }
}

void CountUp::render() {
  // Clear the screen to a soft white color
  graphics.clearScreen(screen_color);

  // Switch to 2D drawing mode
  graphics.draw2D();

  number_text->draw();
  takei->draw();

  if (timing.locked("countup_key_lock") && current_number % modulus == 0) {
    for (int i = 0; i < 3; i++) {
      bear[i]->setRotation(360.0 * (timing.since("countup_key_lock") * 1.7));
      bear[i]->draw();
    }
  }
}

CountUp::~CountUp() {
  delete takei; // live long and prosper
  delete number_text;
  for (int i = 0; i < 3; i++) {
    delete bear[i];
  }
}