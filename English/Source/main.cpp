#include <string>

#include "honey.h"

using namespace Honey;



#include <string>
#include <array>

#include "switcher.h"

#include "honey.h"

using namespace std;
using namespace Honey;

int main(int argc, char* args[]) {
  StartHoney("Honey Engine");

  Switcher* switcher = new Switcher(); 

  // Loop
  while (!screenmanager.getQuit()) {
    input.processInput();

    screenmanager.loop();

    switcher->logic();
    switcher->render();

    // Put everything we've drawn on screen
    graphics.updateDisplay();
  }

  delete switcher;
}

