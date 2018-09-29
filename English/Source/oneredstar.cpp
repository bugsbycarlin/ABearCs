/*
  BearsLoveHoney
  Matthew Carlin
  Copyright 2018
*/

#include "oneredstar.h"

using namespace std;
using namespace Honey;

OneRedStar::OneRedStar() {
}

void OneRedStar::loop() {
  hot_config.checkAndUpdate();
  logic();
  render();
}

void OneRedStar::initialize() {
  screen_width = hot_config.getInt("layout", "screen_width");
  screen_height = hot_config.getInt("layout", "screen_height");
  screen_color = hot_config.getString("layout", "screen_color");

  initializeAssets();
  initializeLogic();
}

void OneRedStar::initializeLogic() {

  fade_in_duration = hot_config.getFloat("animation", "fade_in_duration");
  word_duration = hot_config.getFloat("animation", "word_duration");
  fade_out_duration = hot_config.getFloat("animation", "fade_out_duration");
  fade_out_stagger = hot_config.getFloat("animation", "fade_out_stagger");
  shake_width = hot_config.getFloat("animation", "oneredstar_shake_width");

  for (int i = 0; i < color_names.size(); i++) {
    color_values[i] = hot_config.getString("colors", color_names[i]);
  }

  shuffle(1);
}

void OneRedStar::initializeAssets() {

  // Load shape images
  for (int i = 0; i < shapes.size(); i++) {
    graphics.addImage(shapes[i], "Art/" + shapes[i] + ".png");
  }

  // Make shape sprites
  for (int i = 0; i < shapes.size(); i++) {
    shape_sprites[i] = new Sprite(
      shapes[i],
      origin,
      "#ffffff", 1.0, 0.0, 1.0
    );
    shape_sprites[i]->setOpacity(0);
  }

  // Load shape word sounds
  for (int i = 0; i < shapes.size(); i++) {
    sound.addSound(shapes[i], "Sound/" + shapes[i] + ".wav");
    sound.addSound(shapes[i] + "s", "Sound/" + shapes[i] + "s.wav");
  }

  // Load color word sounds
  for (int i = 0; i < color_names.size(); i++) {
    sound.addSound(color_names[i], "Sound/" + color_names[i] + ".wav");
  }

  // Load number word sounds
  for (int i = 1; i <= max_number; i++) {
    sound.addSound(to_string(i), "Sound/" + to_string(i) + ".wav");
  }

  // Load extra sounds
  sound.addSound("dink_0", "Sound/C_Square2.wav");
  sound.addSound("dink_1", "Sound/C_Square1.wav");

  // Set volume from config
  sound.setSoundVolume(hot_config.getFloat("sound", "sound_volume"));
  // sound.setMusicVolume(hot_config.getFloat("sound", "music_volume"));

  // Make textboxers with default values
  string font = hot_config.getString("layout", "font");
  number_text = new Textbox(
    font, hot_config.getInt("layout", "number_text_font_size"),
    "a", origin, "#77dd77");
  color_text = new Textbox(
    font, hot_config.getInt("layout", "color_text_font_size"),
    "b", origin, "#77dd77");
  shape_text = new Textbox(
    font, hot_config.getInt("layout", "shape_text_font_size"),
    "c", origin, "#77dd77");
}



void OneRedStar::logic() {
  inputLogic();
  if (timing.locked("animating")) {
    timing.doSequence("animation_sequence");
  }
}



void OneRedStar::inputLogic() {
  if (input.threeQuickKey("escape")) {
    screenmanager.setQuit();
  }

  for (int i = 1; i < max_number; i++) {
    if (input.keyDown(to_string(i)) && !timing.locked("animating")) {
      shuffle(i);
    }
  }
  if (input.keyDown("0") && !timing.locked("animating")) {
    shuffle(max_number);
  }
  array<string, max_number - 1> rando_letters = {"a", "s", "d", "f", "g", "h", "j", "k", "l"};
  for (int i = 0; i < rando_letters.size(); i++) {
    if (input.keyDown(rando_letters[i]) && !timing.locked("animating")) {
      shuffle(-1);
    }
  }
  array<string, max_number> color_letters = {"q", "w", "e", "r", "t", "y", "u", "i", "o", "p"};
  for (int i = 0; i < color_letters.size(); i++) {
    if (input.keyDown(color_letters[i]) && !timing.locked("animating")) {
      int color_num = i;
      color_value = color_values[color_num];
      color_name = color_names[color_num];

      remake();
    }
  }
  if(input.keyDown("up") && !timing.locked("animating")) {
    number = (number + max_number) % max_number + 1;
    remake();
  }
  if(input.keyDown("down") && !timing.locked("animating")) {
    number = (number + max_number - 2) % max_number + 1;
    remake();
  }
}

void OneRedStar::animationSequence(int sequence_counter, float duration) {
  if (sequence_counter >= sequence_action.size()) {
    return;
  }
  
  string action = sequence_action[sequence_counter];

  if (action.find("fade_in") != string::npos) {
    number_text->setOpacity(1, fade_in_duration);
    color_text->setOpacity(1, fade_in_duration);
    shape_text->setOpacity(1, fade_in_duration);
    for(int i = 0; i < number; i++) {
      shape_sprites[i]->setOpacity(1, fade_in_duration);
      shape_sprites[i]->shakePosition(shake_width, fade_in_duration);
    }
  } else if (action == "number_word") {
    number_text->shakePosition(shake_width, word_duration);
    sound.playSound(to_string(number), 1);
  } else if (action == "color_word") {
    color_text->shakePosition(shake_width, word_duration);
    sound.playSound(color_name, 1);
  } else if (action == "shape_word") {
    shape_text->shakePosition(shake_width, word_duration);
    if (number < 2) {
      sound.playSound(shape, 1);
    } else {
      sound.playSound(shape + "s", 1);
    }
  } else if (action.find("fade_out_") != string::npos) {
    int val = stoi(action.substr(9,1)) - 1;
    if (action.length() == 11) {
      val = stoi(action.substr(9,2)) - 1;
    }
    shape_sprites[val]->setOpacity(0, fade_out_duration);
    if (val == 0) {
      number_text->setOpacity(0, fade_out_duration);
      color_text->setOpacity(0, fade_out_duration);
      shape_text->setOpacity(0, fade_out_duration);
    }
  }
}


int OneRedStar::randomOutside(int size) {
  int x = math_utils.randomInt(0, size);
  if (x < (size / 2)) {
    return -x;
  } else {
    return size / 2 + x;
  }
}

void OneRedStar::shuffle(int new_number) {
  if (new_number == -1) {
    number = math_utils.randomInt(1, max_number + 1);
  } else {
    number = new_number;
  }
  shape = shapes[math_utils.randomInt(0, shapes.size())];
  int color_num = math_utils.randomInt(0, color_values.size());
  color_value = color_values[color_num];
  color_name = color_names[color_num];

  remake();
}

void OneRedStar::remake() {
  effects.removeAllEffects();

  size = 1.0 - number / 10.0;
  if (number > 2) {
    size = 0.8;
  }
  if (number > 4) {
    size = 0.6;
  }

  int margin = hot_config.getInt("layout", "shape_margin_" + to_string(number));
  int x = hot_config.getInt("layout", "shape_x_" + to_string(number));
  int y = hot_config.getInt("layout", "shape_y_" + to_string(number));

  if (number <= 4) {
    layouts.makeTileWrapLayout("shapes", x - margin * (number - 1) / 2.0, y, margin, margin, 4);
  } else if (number % 2 == 0) {
    layouts.makeTileWrapLayout("shapes", x - margin * (number / 2 - 1) / 2.0, y, margin, margin, number / 2);
  } else {
    layouts.makeStaggerWrapLayout("shapes", x - margin * ((number+1)/2 - 1) / 2.0, y, margin, margin, (number + 1) / 2);
  }

  number_text->setText(to_string(number));
  color_text->setText(color_name);
  if (number == 1) {
    shape_text->setText(shape);
  } else {
    shape_text->setText(shape + "s");
  }

  number_text->setPosition({
    (int) (
      0.5 * (hot_config.getInt("layout", "screen_width") 
        - number_text->getWidth() 
        - color_text->getWidth() 
        - shape_text->getWidth() 
        - 60)
    ),
    hot_config.getInt("layout", "number_text_y")
  });

  color_text->setPosition({
    number_text->getPosition().x + number_text->getWidth() + 30,
    hot_config.getInt("layout", "color_text_y")
  });

  shape_text->setPosition({
    number_text->getPosition().x + number_text->getWidth() + color_text->getWidth() + 60,
    hot_config.getInt("layout", "shape_text_y")
  });

  for (int i = 0; i < number; i++) {
    shape_sprites[i]->setLabel(shape);
    shape_sprites[i]->setScale(size);
    shape_sprites[i]->setColor(color_value);
  }

  sequence_timing = {};
  sequence_action = {};
  sequence_action.push_back("fade_in");
  sequence_timing.push_back(fade_in_duration);
  sequence_action.push_back("number_word");
  sequence_action.push_back("color_word");
  sequence_action.push_back("shape_word");
  sequence_timing.push_back(word_duration);
  sequence_timing.push_back(word_duration);
  sequence_timing.push_back(word_duration);
  for (int i = 1; i <= number; i++) {
    sequence_action.push_back("fade_out_" + to_string(i));
    sequence_timing.push_back(fade_out_stagger);
  }

  //timing.makeSequenceWithFunction("animation_sequence", sequence_timing, animationSequence);
  timing.makeSequenceWithFunction("animation_sequence", sequence_timing, [this](int i, float f) { return animationSequence(i, f); });

  float total_animating_duration = fade_in_duration + 3 * word_duration + (number - 1) * fade_out_stagger + fade_out_duration + 0.1;
  timing.lock("animating", total_animating_duration);

  number_text->setColor(color_value);
  color_text->setColor(color_value);
  shape_text->setColor(color_value);
}

void OneRedStar::render() {
  graphics.clearScreen(hot_config.getString("layout", "screen_color"));
  graphics.draw2D();

  if (timing.locked("animating")) { 
    float timing_since = timing.since("animating");
    for (int i = 0; i < number; i++) {
      position p;
      if (number % 2 == 0) {
        p = layouts.tileWrap("shapes", i);
      } else {
        p = layouts.staggerWrap("shapes", i);
      }
      shape_sprites[i]->draw(p);
    }
    graphics.setColor(color_value, 1);

    number_text->draw();
    color_text->draw();
    shape_text->draw();
  }
}

OneRedStar::~OneRedStar() {
  delete number_text;
  delete color_text;
  delete shape_text;

  for (int i = 0; i < shapes.size(); i++) {
    delete shape_sprites[i];
  }
}








