#include <string>

#include "honey.h"

using namespace Honey;



#include <string>
#include <array>

#include "abearcs.h"

#include "honey.h"

using namespace std;
using namespace Honey;

int main(int argc, char* args[]) {
  StartHoney("Honey Engine");

  ABearCs* abearcs = new ABearCs();

  abearcs->initialize();

  while (!screenmanager.getQuit()) {
    input.processInput();

    abearcs->logic();
    abearcs->render();

    graphics.updateDisplay();
  }

  delete abearcs;
}

