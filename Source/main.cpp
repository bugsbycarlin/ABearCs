#include <string>

#ifdef __APPLE__
#include "CoreFoundation/CoreFoundation.h"
#endif

#include "window.h"
#include "logic.h"
#include "hotconfig.h"
#include "input.h"
#include "graphics.h"
#include "sound.h"
#include "effects.h"

using namespace Honey;

std::string letters[] = {
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

std::string pictures[] = {
  "apple",
  "bear",
  "cat",
  "dog",
  "eggs",
  "flowers",
  "grapes",
  "hamburger",
  "icecream",
  "juicebox",
  "key",
  "lemon",
  "monkey",
  "nixon",
  "orange",
  "pizza",
  "queen",
  "rainbow",
  "strawberry",
  "trees",
  "umbrella",
  "viola",
  "watermelon",
  "xylophone",
  "yoga",
  "zebra"
};

// https://bits.mdminhazulhaque.io/cpp/find-and-replace-all-occurrences-in-cpp-string.html
void find_and_replace(std::string& source, std::string const& find, std::string const& replace)
{
  for(std::string::size_type i = 0; (i = source.find(find, i)) != std::string::npos;)
  {
    source.replace(i, find.length(), replace);
    i += replace.length();
  }
}

int main(int argc, char* args[]) {

  // Fix it for App Bundling
  #ifdef __APPLE__    
    CFBundleRef mainBundle = CFBundleGetMainBundle();
    CFURLRef resourcesURL = CFBundleCopyResourcesDirectoryURL(mainBundle);
    char path[PATH_MAX];
    if (!CFURLGetFileSystemRepresentation(resourcesURL, TRUE, (UInt8 *)path, PATH_MAX))
    {
        // error!
    }
    CFRelease(resourcesURL);

    std::string mod_path(path);
    find_and_replace(mod_path, "Contents/Resources", "Contents/MacOS");

    chdir(mod_path.c_str());
    printf("Current Path: %s\n", mod_path.c_str());
  #endif

  // Load configuration, after setting the right path with mod_path.
  hot_config->setPath(mod_path + "/config.txt");
  if(hot_config->checkAndUpdate() != hot_config->SUCCESS) {
    exit(1);
  }

  int screen_width = hot_config->getInt("layout", "screen_width");
  int screen_height = hot_config->getInt("layout", "screen_height");
  bool full_screen = hot_config->getBool("layout", "full_screen");

  Window* window = new Window("ABearCs", screen_width, screen_height, full_screen);

  graphics->initialize(window);
  sound->initialize();

  int current_letter = -1;
  int last_letter = -1;

  for (int i = 0; i < 26; i++) {
    graphics->addImage(letters[i], "Art/" + letters[i] + ".png");
    graphics->addImage(pictures[i], "Art/" + pictures[i] + ".png");
    sound->addSound(letters[i], "Sound/" + letters[i] + ".wav");
  }

  bool quit = false;
  while (!quit) {
    // Check and load configuration every 2 seconds
    hot_config->checkAndUpdate();

    // Set a bunch of variables from configuration
    std::string screen_color = hot_config->getString("layout", "screen_color");
    int picture_x = hot_config->getInt("layout", "picture_x");
    int picture_y = hot_config->getInt("layout", "picture_y");
    int text_x = hot_config->getInt("layout", "text_x");
    int text_y = hot_config->getInt("layout", "text_y");
    int tween_type = hot_config->getInt("animation", "tween_type");
    float animation_duration = hot_config->getFloat("animation", "animation_duration");
    float shake_duration = hot_config->getFloat("animation", "shake_duration");
    int shake_width = hot_config->getInt("animation", "shake_width");
    float key_swap_duration = hot_config->getFloat("input", "key_swap_duration");
    float sound_volume = hot_config->getFloat("sound", "sound_volume");
    float sound_lock_duration = hot_config->getFloat("sound", "sound_lock_duration");
    

    sound->setSoundVolume(sound_volume);

    input->processInput();

    // do stuff with input
    for (int i = 0; i < 26; i++) {
      if (input->keyDown(letters[i]) && !logic->isTimeLocked("key_swap") && i != current_letter) {
        logic->remove("sound_lock");
        logic->makeTimeLock("key_swap", key_swap_duration);
        logic->makeTimeLock("animation", animation_duration);
        effects->makeTween("slide_last_letter", screen_height, 0, animation_duration);
        last_letter = current_letter;
        current_letter = i;
      }
    }

    if (logic->isTimeLocked("key_swap") && !logic->isTimeLocked("animation") && !logic->isTimeLocked("sound_lock")) {
      logic->makeTimeLock("sound_lock", sound_lock_duration);
      sound->playSound(letters[current_letter], 1);
      effects->makeShake("sound_shake", shake_width, shake_duration);
    }

    if (input->threeQuickKey("escape")) {
      quit = true;
    }

    // Clear the screen to a soft white color
    graphics->clearScreen(screen_color);

    // Switch to 2D drawing mode
    graphics->draw2D();

    // draw stuff
    float x, y;

    // Current picture
    if (current_letter != -1) {
      x = picture_x;
      y = picture_y;
      if (logic->isTimeLocked("animation")) {
        y += effects->tween("slide_last_letter", tween_type);
      }
      graphics->drawImage(pictures[current_letter], x, y);
      x = text_x;
      y = text_y;
      if (logic->isTimeLocked("animation")) {
        y += effects->tween("slide_last_letter", tween_type);
      }
      if (logic->isTimeLocked("key_swap")) {
        x += effects->shake("sound_shake");
        y += effects->shake("sound_shake");
      }
      graphics->drawImage(letters[current_letter], x, y);
    }

    // Last picture, if animating
    if (logic->isTimeLocked("animation") && last_letter != -1) {
      x = picture_x;
      y = picture_y - screen_height + effects->tween("slide_last_letter", tween_type);
      graphics->drawImage(pictures[last_letter], x, y);
      x = text_x;
      y = text_y - screen_height + effects->tween("slide_last_letter", tween_type);
      graphics->drawImage(letters[last_letter], x, y);
    }

    // Put everything we've drawn on screen
    graphics->updateDisplay();
  }

  for (int i = 0; i < 26; i++) {
    graphics->destroyImage(letters[i]);
    graphics->destroyImage(pictures[i]);
    sound->destroySound(letters[i]);
  }

  window->destroy();
}

