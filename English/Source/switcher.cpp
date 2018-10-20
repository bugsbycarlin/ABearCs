/*
  BearsLoveHoney
  Matthew Carlin
  Copyright 2018
*/

#include "switcher.h"

using namespace std;
using namespace Honey;

Switcher::Switcher() {
  nav_text = new Textbox(
    hot_config.getString("layout", "font"), 30,
    "a", origin, "#000000");

  makeOrSet("puzzle");

  switch_counter = 3;
  max_screen = 5;
}

void Switcher::logic() {
  bool switched = false;
  if (input.threeQuickKey("right") && !timing.locked("switcher_key") && switch_counter < max_screen) {
    switch_counter += 1;
    switched = true;
  } else if (input.threeQuickKey("left") && !timing.locked("switcher_key") && switch_counter > 0) {
    switch_counter -= 1;
    switched = true;
  }

  if (switched) {
    sound.stopMusic();
    sound.stopSound();
    timing.lock("switcher_key", 0.25);

    if (switch_counter == 0) {
      makeOrSet("abearcs");
    } else if (switch_counter == 1) {
      makeOrSet("oneredstar");
    } else if (switch_counter == 2) {
      makeOrSet("spelling");
    } else if (switch_counter == 3) {
      makeOrSet("puzzle");
    } else if (switch_counter == 4) {
      makeOrSet("keyboard");
    } else if (switch_counter == 5) {
      makeOrSet("countup");
    }
  }
}

void Switcher::render() {
  int screen_width = hot_config.getInt("layout", "screen_width");
  int screen_height = hot_config.getInt("layout", "screen_height");
  graphics.setColor(1.0, 1.0, 1.0, 1.0);

  if (switch_counter > 0) {
    nav_text->setText("<<<");
    nav_text->draw((position) {10, 10});
  }

  if (switch_counter < max_screen) {
    nav_text->setText(">>>");
    nav_text->draw((position) {screen_width - 85, 10});
  }
}

void Switcher::makeOrSet(string screen_name) {
  if (!screenmanager.checkScreen(screen_name)) {
    if (screen_name == "abearcs") {
      unique_ptr<Screen> abearcs(new ABearCs());
      screenmanager.addScreen("abearcs", abearcs);
      screenmanager.setCurrentScreen("abearcs");
    } else if (screen_name == "oneredstar") {
      unique_ptr<Screen> oneredstar(new OneRedStar());
      screenmanager.addScreen("oneredstar", oneredstar);
      screenmanager.setCurrentScreen("oneredstar");
    } else if (screen_name == "puzzle") {
      unique_ptr<Screen> puzzle(new Puzzle());
      screenmanager.addScreen("puzzle", puzzle);
      screenmanager.setCurrentScreen("puzzle");
    } else if (screen_name == "spelling") {
      unique_ptr<Screen> spelling(new Spelling());
      screenmanager.addScreen("spelling", spelling);
      screenmanager.setCurrentScreen("spelling");
    } else if (screen_name == "keyboard") {
      unique_ptr<Screen> keyboard(new Keyboard());
      screenmanager.addScreen("keyboard", keyboard);
      screenmanager.setCurrentScreen("keyboard");
    } else if (screen_name == "countup") {
      unique_ptr<Screen> countup(new CountUp());
      screenmanager.addScreen("countup", countup);
      screenmanager.setCurrentScreen("countup");
    }

    // Initialize the selection screen
    screenmanager.initialize();
  }

  screenmanager.setCurrentScreen(screen_name);
}

Switcher::~Switcher() {
  delete nav_text;
}