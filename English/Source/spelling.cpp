/*
  BearsLoveHoney
  Matthew Carlin
  Copyright 2018
*/

#include "spelling.h"

using namespace std;
using namespace Honey;

Spelling::Spelling() {
}

void Spelling::loop() {
  logic();
  render();
}

void Spelling::initialize() {
  screen_width = hot_config.getInt("layout", "screen_width");
  screen_height = hot_config.getInt("layout", "screen_height");

  for (int i = 0; i < 26; i++) {
    sound.addSound(letters[i], "Sound/Spelling/" + letters[i] + ".wav");
  }

  for (int i = 0; i < num_spelling_words; i++) {
    sound.addSound(words[i], "Sound/Spelling/" + words[i] + ".wav");
  }

  for (int i = 1; i <= num_drums; i++) {
    sound.addSound("drums_" + to_string(i), "Sound/drums_" + to_string(i) + ".wav");
  }

  for (int i = 0; i < color_names.size(); i++) {
    color_values[i] = hot_config.getString("colors", color_names[i]);
  }

  word_text = new Textbox(
    hot_config.getString("layout", "spelling_font"),
    hot_config.getInt("layout", "spelling_font_size"),
    "a",
    {hot_config.getInt("layout", "spelling_text_x"),
    hot_config.getInt("layout", "spelling_text_y")},
    "#77dd77");

  last_drum = -4;
  drum_num = 0;
  timing.mark("drum_timing");

  setDrumsAndRhymes();
}

void Spelling::logic() {
  // Check and load configuration every 2 seconds
  hot_config.checkAndUpdate();

  setDrumsAndRhymes();
  timing.doSequence("rhyme");

  // Set a bunch of variables from configuration
  screen_color = hot_config.getString("layout", "screen_color");

  sound.setSoundVolume(hot_config.getFloat("sound", "sound_volume"));

  if (input.threeQuickKey("escape")) {
    screenmanager.setQuit();
  }
}

void Spelling::setDrumsAndRhymes() {
  if (timing.since("drum_timing") - last_drum >= 4) {
    drum_num = math_utils.randomInt(1, num_drums);
    sound.playSound("drums_" + to_string(drum_num), 1);
    last_drum += 4;

    word_num = math_utils.randomInt(1, num_spelling_words / 2);

    word_text->setColor(color_values[math_utils.randomInt(1, 10)]);

    vector<float> sequence_timing = {0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5};
    //vector<float> sequence_timing = {0.5, 0.25, 0.25, 0.5, 0.5, 0.5, 0.25, 0.25, 0.5, 0.5};
    timing.makeSequenceWithFunction("rhyme", sequence_timing, [this](int i, float f) { return rhyme(i, f); });
  }
}

void Spelling::rhyme(int i, float f) {
  printf("New call ");
  printf("Word %s ", words[word_num * 2].c_str());
  printf("Sequence %d ", i);
  printf("Drum_num %d ", drum_num);
  printf("Timing %0.2f\n", timing.since("drum_timing"));
  if (i < 3) {
    sound.playSound(words[word_num * 2].substr(i, 1), 1);
    // string t = "   ";
    // t[i] = words[word_num * 2].c_str()[i];
    // word_text->setText(t);
    word_text->setText(words[word_num * 2].substr(0, i + 1));
  } else if (i == 3) {
    sound.playSound(words[word_num * 2], 1);
    word_text->setText(words[word_num * 2]);
    word_text->shakePosition(10, f);
  } else if (i < 7) {
    sound.playSound(words[word_num * 2 + 1].substr(i - 4, 1), 1);
    string t = "   ";
    t[i - 4] = words[word_num * 2 + 1].c_str()[i - 4];
    word_text->setText(t);
  } else if (i == 7) {
    sound.playSound(words[word_num * 2 + 1], 1);
    word_text->setText(words[word_num * 2 + 1]);
    word_text->shakePosition(10, f);
  }

  // if (i >= 1 && i <= 3) {
  //   // sound.playSound(words[word_num * 2].substr(i, 1 - 1), 1);
  //   // word_text->setText(words[word_num * 2].substr(0, i));
  //   sound.playSound(words[word_num * 2].substr(i - 1, 1), 1);
  //   string t = "   ";
  //   t[i - 1] = words[word_num * 2].c_str()[i - 1];
  //   word_text->setText(t);
  // } else if (i == 4 || i == 0) {
  //   sound.playSound(words[word_num * 2], 1);
  //   word_text->setText(words[word_num * 2]);
  //   if (i == 4) word_text->shakePosition(10, f);
  // } else if (i >= 6 && i <= 8) {
  //   sound.playSound(words[word_num * 2 + 1].substr(i - 6, 1), 1);
  //   string t = "   ";
  //   t[i - 6] = words[word_num * 2 + 1].c_str()[i - 6];
  //   word_text->setText(t);
  // } else if (i == 9 || i == 5) {
  //   sound.playSound(words[word_num * 2 + 1], 1);
  //   word_text->setText(words[word_num * 2 + 1]);
  //   if (i == 9) word_text->shakePosition(10, f);
  // }
}

void Spelling::render() {
  // Clear the screen to a soft white color
  graphics.clearScreen(screen_color);

  word_text->draw();

  // Switch to 2D drawing mode
  graphics.draw2D();
}

Spelling::~Spelling() {
  delete word_text;
}