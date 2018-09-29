/*
  BearsLoveHoney
  Matthew Carlin
  Copyright 2018
*/

#include "switcher.h"

using namespace std;
using namespace Honey;

Switcher::Switcher() {
  graphics.addImage("abearcs_icon", "Art/abearcs_icon.png");
  graphics.addImage("oneredstar_icon", "Art/oneredstar_icon.png");

  number_text = new Textbox(
    hot_config.getString("layout", "font"), 30,
    "a", origin, "#000000");

  makeOrSet("abearcs");
}

void Switcher::logic() {
  if(screenmanager.getCurrentScreen() == "abearcs") {
    if (input.keyDown("1")) {
      makeOrSet("oneredstar");
    }
  } else {
    if (input.keyDown("z")) {
      makeOrSet("abearcs");
    }
  }
}

void Switcher::render() {
  int screen_width = hot_config.getInt("layout", "screen_width");
  int screen_height = hot_config.getInt("layout", "screen_height");
  graphics.setColor(1.0, 1.0, 1.0, 1.0);

  if(screenmanager.getCurrentScreen() == "abearcs") {
    graphics.drawImage("oneredstar_icon", screen_width - 74, 0, false, 0, 0.5);
    number_text->setText("1");
    number_text->draw((position) {screen_width - 74 - 22, 2});
  } else {
    graphics.drawImage("abearcs_icon", screen_width - 74, 0, false, 0, 0.5);
    number_text->setText("z");
    number_text->draw((position) {screen_width - 74 - 22, 2});
  }
}

void Switcher::makeOrSet(string screen_name) {
  if (!screenmanager.checkScreen(screen_name)) {
    if (screen_name == "abearcs") {
      // Create the screen
      unique_ptr<Screen> abearcs(new ABearCs());
      screenmanager.addScreen("abearcs", abearcs);
      screenmanager.setCurrentScreen("abearcs");

      // Initialize the selection screen
      screenmanager.initialize();
    } else if (screen_name == "oneredstar") {
      // Create the screen
      unique_ptr<Screen> oneredstar(new OneRedStar());
      screenmanager.addScreen("oneredstar", oneredstar);
      screenmanager.setCurrentScreen("oneredstar");

      // Initialize the selection screen
      screenmanager.initialize();
    
    }
  }

  screenmanager.setCurrentScreen(screen_name);
}

Switcher::~Switcher() {
  delete number_text;
}