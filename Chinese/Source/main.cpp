#include <string>
#include <array>

#include "honey.h"

using namespace Honey;
using namespace std;

const int character_set_size = 4;

array<string, character_set_size> letters = {
  "b",
  "p",
  "m",
  "f",
}

array<string, character_set_size> pictures = {
  "waves",
  "slope",
  "touch",
  "buddha",
}

// string letters[] = {
//   "a",
//   "b",
//   "c",
//   "d",
//   "e",
//   "f",
//   "g",
//   "h",
//   "i",
//   "j",
//   "k",
//   "l",
//   "m",
//   "n",
//   "o",
//   "p",
//   "q",
//   "r",
//   "s",
//   "t",
//   "u",
//   "v",
//   "w",
//   "x",
//   "y",
//   "z"
// };

// string pictures[] = {
//   "apple",
//   "bear",
//   "cat",
//   "dog",
//   "eggs",
//   "flowers",
//   "grapes",
//   "hamburger",
//   "icecream",
//   "juicebox",
//   "key",
//   "lemon",
//   "monkey",
//   "nixon",
//   "orange",
//   "pizza",
//   "queen",
//   "rainbow",
//   "strawberry",
//   "trees",
//   "umbrella",
//   "viola",
//   "watermelon",
//   "xylophone",
//   "yoga",
//   "zebra"
// };

int main(int argc, char* args[]) {
  StartHoney("ABearCs");

  int current_letter = -1;
  int last_letter = -1;

  for (int i = 0; i < 26; i++) {
    graphics.addImage(letters[i], "Art/" + letters[i] + ".png");
    graphics.addImage(pictures[i], "Art/" + pictures[i] + ".png");
    sound.addSound(letters[i], "Sound/" + letters[i] + ".wav");
  }

  bool quit = false;
  while (!quit) {
    // Check and load configuration every 2 seconds
    hot_config.checkAndUpdate();

    // Set a bunch of variables from configuration
    string screen_color = hot_config.getString("layout", "screen_color");
    int screen_height = hot_config.getInt("layout", "screen_height");
    int picture_x = hot_config.getInt("layout", "picture_x");
    int picture_y = hot_config.getInt("layout", "picture_y");
    int text_x = hot_config.getInt("layout", "text_x");
    int text_y = hot_config.getInt("layout", "text_y");
    int tween_type = hot_config.getInt("animation", "tween_type");
    float animation_duration = hot_config.getFloat("animation", "animation_duration");
    float shake_duration = hot_config.getFloat("animation", "shake_duration");
    int shake_width = hot_config.getInt("animation", "shake_width");
    float key_swap_duration = hot_config.getFloat("input", "key_swap_duration");
    float sound_volume = hot_config.getFloat("sound", "sound_volume");
    float sound_lock_duration = hot_config.getFloat("sound", "sound_lock_duration");

    sound.setSoundVolume(sound_volume);

    input.processInput();

    // do stuff with input
    for (int i = 0; i < 26; i++) {
      if (input.keyDown(letters[i]) && !timing.locked("key_swap") && i != current_letter) {
        timing.remove("sound_lock");
        timing.lock("key_swap", key_swap_duration);
        timing.lock("animation", animation_duration);
        effects.makeTween("slide_last_letter", screen_height, 0, animation_duration);
        last_letter = current_letter;
        current_letter = i;
      }
    }

    if (timing.locked("key_swap") && !timing.locked("animation") && !timing.locked("sound_lock")) {
      timing.lock("sound_lock", sound_lock_duration);
      sound.playSound(letters[current_letter], 1);
      effects.makeShake("sound_shake", shake_width, shake_duration);
    }

    if (input.threeQuickKey("escape")) {
      quit = true;
    }

    // Clear the screen to a soft white color
    graphics.clearScreen(screen_color);

    // Switch to 2D drawing mode
    graphics.draw2D();

    // draw stuff
    float x, y;

    // Current picture
    if (current_letter != -1) {
      x = picture_x;
      y = picture_y;
      if (timing.locked("animation")) {
        y += effects.tween("slide_last_letter", tween_type);
      }
      graphics.drawImage(pictures[current_letter], x, y);
      x = text_x;
      y = text_y;
      if (timing.locked("animation")) {
        y += effects.tween("slide_last_letter", tween_type);
      }
      if (timing.locked("key_swap")) {
        x += effects.shake("sound_shake");
        y += effects.shake("sound_shake");
      }
      graphics.drawImage(letters[current_letter], x, y);
    }

    // Last picture, if animating
    if (timing.locked("animation") && last_letter != -1) {
      x = picture_x;
      y = picture_y - screen_height + effects.tween("slide_last_letter", tween_type);
      graphics.drawImage(pictures[last_letter], x, y);
      x = text_x;
      y = text_y - screen_height + effects.tween("slide_last_letter", tween_type);
      graphics.drawImage(letters[last_letter], x, y);
    }

    // Put everything we've drawn on screen
    graphics.updateDisplay();
  }
}

