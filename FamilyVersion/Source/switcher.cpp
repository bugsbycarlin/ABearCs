/*
  BearsLoveHoney
  Matthew Carlin
  Copyright 2018
*/

#include "switcher.h"

using namespace std;
using namespace Honey;

Switcher::Switcher() {
  makeOrSet("abearcs");

  switch_counter = 0;
  max_screen = 0;
}

void Switcher::logic() {
}

void Switcher::render() {
  int screen_width = hot_config.getInt("layout", "screen_width");
  int screen_height = hot_config.getInt("layout", "screen_height");
  graphics.setColor(1.0, 1.0, 1.0, 1.0);
}

void Switcher::makeOrSet(string screen_name) {
  if (!screenmanager.checkScreen(screen_name)) {
    if (screen_name == "abearcs") {
      unique_ptr<Screen> abearcs(new ABearCs());
      screenmanager.addScreen("abearcs", abearcs);
      screenmanager.setCurrentScreen("abearcs");
    }

    // Initialize the selection screen
    screenmanager.initialize();
  }

  screenmanager.setCurrentScreen(screen_name);
}

Switcher::~Switcher() {
}