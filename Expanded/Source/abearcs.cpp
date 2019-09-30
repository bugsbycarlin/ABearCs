/*
  BearsLoveHoney
  Matthew Carlin
  Copyright 2018
*/

#include "abearcs.h"

using namespace std;
using namespace Honey;

void debug(int x) {
  //if (false) {
  printf("Here %d\n", x);
  //}
}

ABearCs::ABearCs() {
}

void ABearCs::initialize() {
  screen_width = hot_config.getInt("layout", "screen_width");
  screen_height = hot_config.getInt("layout", "screen_height");
  screen_color = hot_config.getString("layout", "screen_color");
  picture_x = hot_config.getInt("layout", "abearcs_picture_x");
  picture_y = hot_config.getInt("layout", "abearcs_picture_y");
  letter_x = hot_config.getInt("layout", "abearcs_letter_x");
  letter_y = hot_config.getInt("layout", "abearcs_letter_y");
  word_x = hot_config.getInt("layout", "abearcs_word_x");
  word_y = hot_config.getInt("layout", "abearcs_word_y");
  tween_type = hot_config.getInt("animation", "tween_type");

  current_letter = "";
  last_letter = "";

  letters = {
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

  pictures = {};
  // "balloon"
  // "dog"
  // "flowers"
  // "grapes"
  pictures["a"] = {"apple", "airplane"};
  pictures["b"] = {"bee", "bear", "ball", "bus", "bike"};
  pictures["c"] = {"carrot", "cloud", "crayons", "car", "cat", "cow"};
  pictures["d"] = {"drum", "dinosaur", "ducks"};
  pictures["e"] = {"elephant", "eggs"};
  pictures["f"] = {"feather", "flowers"};
  pictures["g"] = {"goat", "grapes"};
  pictures["h"] = {"hamburger"};
  pictures["i"] = {"iguana", "icecream"};
  pictures["j"] = {"juice"};
  pictures["k"] = {"koala", "key"};
  pictures["l"] = {"lamp"};
  pictures["m"] = {"monkey"};
  pictures["n"] = {"noodles"};
  pictures["o"] = {"orange", "octopus"};
  pictures["p"] = {"phone"};
  pictures["q"] = {"quilt"};
  pictures["r"] = {"rhino", "rainbow"};
  pictures["s"] = {"sad"};
  pictures["t"] = {"truck"};
  pictures["u"] = {"umbrella"};
  pictures["v"] = {"vulture"};
  pictures["w"] = {"watermelon", "wolf"};
  pictures["x"] = {"xylophone"};
  pictures["y"] = {"yoga"};
  pictures["z"] = {"zebra"};

  pictures["shapes"] = {
    "arrow",
    "circle",
    "diamond",
    "heart",
    "hexagon",
    "octagon",
    "pentagon",
    "square",
    "star",
    "triangle"
  };

  colors = {
    "#77dd77",
    "#ff4941",
    "#c0c0c0",
    "#5284bd",
    "#917394",
    "#ffa82e",
    "#ffd700",
    "#444444",
    "#ffb6c1",
    "#835c3b"
  };

  initializeMaze();

  for (int i = 0; i < 26; i++) {
    string letter = letters[i];
    sound.addSound(letter, "Sound/Letters/" + letter + ".wav");
    for (int j = 0; j < pictures[letter].size(); j++) {
      string picture = pictures[letter][j];
      graphics.addImage(picture, "Art/Pictures/" + picture + ".png");
      sound.addSound(picture, "Sound/Words/" + picture + ".wav");
    }
  }
  sound.addSound("maze", "Sound/Words/maze.wav");

  graphics.addImages("Art/Shapes/", pictures["shapes"]);
  sound.addSounds("Sound/Shapes/", pictures["shapes"]);

  graphics.addImages("Art/Maze/", {
    "tile_1",
    "tile_2",
    "tile_3",
    "tile_4",
    "wall",
  });

  sound.addSounds("Sound/", {
    "button_sound",
    "is_for",
    "can_you_say",
    "plink",
  });

  // sound.addMusic("gypsy_jazz", "Sound/gypsy_jazz.wav");
  // sound.playMusic("gypsy_jazz", -1);

  effects.makeOscillation("squish", hot_config.getFloat("animation", "squish_size"), hot_config.getFloat("animation", "squish_cycle"));
  effects.start("squish");

  letter_box = new Textbox(
    hot_config.getString("layout", "font_name"),
    hot_config.getInt("layout", "letter_size"),
    "X",
    (position) {letter_x, letter_y},
    "#000000"
  );

  word_box = new Textbox(
    hot_config.getString("layout", "font_name"),
    hot_config.getInt("layout", "word_size"),
    "X",
    (position) {word_x, word_y},
    "#000000"
  );

  // mode = full_keyboard or number_pad.
  // Number pad is a wireless number pad board:
  // https://www.amazon.com/gp/product/B07H25BVQJ/
  // https://www.google.com/search?q=wireless+keypad&tbm=isch
  mode = "full_keyboard";
}

void ABearCs::initializeMaze() {
  maze = {};
  runner_visited = {};
  runner_positions = {};
  vector<vector<int>> visited;
  vector<wall_position> walls;
  for (int i = 0; i < 16; i++) {
    maze.push_back({});
    visited.push_back({});
    runner_visited.push_back({});
    for (int j = 0; j < 16; j++) {
      maze[i].push_back(-1);
      visited[i].push_back(0);
      runner_visited[i].push_back(0);
    }
  }
  walls = {};
  maze[0][15] = 1;
  walls.push_back({1, 15, 0, 15});
  walls.push_back({0, 14, 0, 15});
  runner_positions.push_back({0, 15});

  runner_sprite = new Sprite("car", {0 * 24 + 12, 15 * 24 + 12}, "#FFFFFF", 1.0, 0.0, 1/16.66);
  
  while (walls.size() > 0) {
    wall_position w = walls[math_utils.randomInt(0, walls.size())];
    vector<wall_position> new_walls = {};
    for (wall_position p : walls) {
      if (p.x != w.x || p.y != w.y) {
        new_walls.push_back(p);
      } else {
        printf("Removed %d,%d\n", p.x, p.y);
      }
    }
    walls = new_walls;
    vector<position> adjustments = {
      {1, -1},
      {1, 0},
      {1, 1},
      {0, -1},
      {0, 1},
      {-1, -1},
      {-1, 0},
      {-1, 1},
    };
    if (visited[w.x][w.y] == 0) {
      visited[w.x][w.y] = 1;

      bool tunnel = true;
      int ax = 2 * w.x - w.last_x;
      int ay = 2 * w.y - w.last_y;

      for (position a : adjustments) {
        if (w.x + a.x >= 0 && w.x + a.x <= 15 && w.y + a.y >= 0 && w.y + a.y <= 15
          && maze[w.x + a.x][w.y + a.y] > 0) {
          
          if (!(w.x == w.last_x && w.y + a.y == w.last_y)
            && !(w.x + a.x == w.last_x && w.y == w.last_y)) {
            tunnel = false;
          }

        }
      }

      if (tunnel) {
        printf("Tunneling\n");
        maze[w.x][w.y] = math_utils.randomInt(1, 5);
        vector<position> more_walls = {
          {1, 0},
          {0, -1},
          {0, 1},
          {-1, 0},
        };
        for (position m : more_walls) {
          if (w.x + m.x >= 0 && w.x + m.x <= 15 && w.y + m.y >= 0 && w.y + m.y <= 15
            && maze[w.x + m.x][w.y + m.y] < 0
            && visited[w.x + m.x][w.y + m.y] == 0) {
            walls.push_back({w.x + m.x, w.y + m.y, w.x, w.y});
          }
        }
      }
    }
  }

  timing.mark("maze_step");
}

void ABearCs::logic() {
  float animation_duration = hot_config.getFloat("animation", "animation_duration");
  float shake_duration = hot_config.getFloat("animation", "shake_duration");
  int shake_width = hot_config.getInt("animation", "shake_width");
  float key_swap_duration = hot_config.getFloat("input", "key_swap_duration");
  float sound_volume = hot_config.getFloat("sound", "sound_volume");
  float music_volume = hot_config.getFloat("sound", "music_volume");

  sound.setSoundVolume(sound_volume);
  sound.setSoundVolume(music_volume);

  int input_val = -1;

  float maze_speed = 0.75;
  if (current_letter == "maze" && timing.since("maze_step") > maze_speed) {
    timing.mark("maze_step");
    vector<position> moves = {
      {1, 0},
      {0, -1},
      {0, 1},
      {-1, 0},
    };

    position current_position = runner_positions[runner_positions.size() - 1];

    bool moved = false;
    for (position p : moves) {
      int x = current_position.x + p.x;
      int y = current_position.y + p.y;
      if (x >= 0 && x <= 15 && y >= 0 && y <= 15) {
        if (runner_visited[x][y] == 0 && maze[x][y] > 0 && !moved) {
          printf("I moved forward\n");
          runner_positions.push_back({x, y});
          runner_visited[x][y] = 1;
          moved = true;
          sound.playSound("plink", 1);
          runner_sprite->setPosition({x * 24 + 12, y * 24 + 12}, maze_speed);
        }
      }
    }

    if (!moved && runner_positions.size() > 1) {
      printf("I moved back\n");
      runner_positions.pop_back();
      current_position = runner_positions[runner_positions.size() - 1];
      runner_sprite->setPosition({current_position.x * 24 + 12, current_position.y * 24 + 12}, maze_speed);
      sound.playSound("plink", 1);
      moved = true;
    }

    if (!moved) {
      runner_visited = {};
      runner_positions = {};
      for (int i = 0; i < 16; i++) {
        runner_visited.push_back({});
        for (int j = 0; j < 16; j++) {
          runner_visited[i].push_back(0);
        }
      }
      runner_positions.push_back({0, 15});
    }
  }

  if (current_letter == "") {
    input_val = math_utils.randomInt(0, 25);
    //input_val = 50;
  }

  if (mode == "number_pad" && 
      !timing.locked("key_swap") &&
      !input.keyDown("escape") &&
      input.anyKeyPressed()) {
    input_val = math_utils.randomInt(0, 40);
    if (input_val > 25 && input_val < 35) input_val = 40;
    if (input_val > 35 && input_val < 45) input_val = 50;
    if (input_val > 12 && input_val <= 25) input_val -= 12;
  }
  // this should include the extra modes for maze and shapes and numbers

  if (mode == "full_keyboard") {
    // if letters pressed, do letters
    for (int i = 0; i < 26; i++) {
      if (input.keyDown(letters[i]) && !timing.locked("key_swap")) {
        input_val = i;
      }
    }

    // if 1-0 pressed, do numbers

    // if space pressed, do shapes
    if (input.keyDown("space") && !timing.locked("key_swap")) {
      input_val = 40;
    }

    // if up/down/left/right pressed, do maze
    if (!timing.locked("key_swap") &&
      (input.keyDown("up") ||
       input.keyDown("down") ||
       input.keyDown("left") ||
       input.keyDown("right"))) {
      input_val = 50;
    }

    // music!
    if (!timing.locked("key_swap") 
      && (input.keyDown("left_shift") || input.keyDown("right_shift"))) {
      sound.playSound("plink", 1);
      //input_val = 60;
    }
  }

  if (input_val > -1) {
    sound.playSound("button_sound", 1);
    timing.lock("key_swap", key_swap_duration);
    timing.lock("animation", animation_duration);
    effects.makeTween("slide_last_letter", screen_height, 0, animation_duration);
    effects.start("slide_last_letter");
    last_letter = current_letter;
    last_word = current_word;
    last_color = current_color;
  }

  if (input_val >= 0 && input_val <= 25) {
    current_letter = letters[input_val];
    current_word = pictures[letters[input_val]][math_utils.randomInt(0, pictures[letters[input_val]].size())];
    printf("The letter is %s\n", current_letter.c_str());
    timing.makeSequenceWithFunction("speech", {(float)(animation_duration + 0.25), 0.75, 1.5, 1.5, 2.0}, [this](int i, float f) { 
      if (i == 1) {
        sound.playSound(current_letter, 1);
        effects.makeShake("letter_shake",
          hot_config.getInt("animation", "shake_width"),
          hot_config.getFloat("animation", "shake_duration"));
        effects.start("letter_shake");
      } else if (i == 2) {
        sound.playSound("is_for", 1);
      } else if (i == 3) {
        sound.playSound(current_word, 1);
        effects.makeShake("word_shake",
          hot_config.getInt("animation", "shake_width"),
          hot_config.getFloat("animation", "shake_duration"));
        effects.start("word_shake");
      } else if (i == 4) {
        sound.playSound("can_you_say", 1);
      } else if (i == 5) {
        sound.playSound(current_word, 1);
        effects.makeShake("word_shake",
          hot_config.getInt("animation", "shake_width"),
          hot_config.getFloat("animation", "shake_duration"));
        effects.start("word_shake");
      }
    });
  }

  if (input_val == 40) { // shapes!
    current_letter = "shapes";
    current_word = pictures["shapes"][math_utils.randomInt(0, pictures["shapes"].size())];
    current_color = colors[math_utils.randomInt(0, colors.size())];
    timing.makeSequenceWithFunction("speech", {(float)(animation_duration + 0.25), 1.5, 2.0}, [this](int i, float f) { 
      if (i == 1) {
        sound.playSound(current_word, 1);
        effects.makeShake("word_shake",
          hot_config.getInt("animation", "shake_width"),
          hot_config.getFloat("animation", "shake_duration"));
        effects.start("word_shake");
      } else if (i == 2) {
        sound.playSound("can_you_say", 1);
      } else if (i == 3) {
        sound.playSound(current_word, 1);
        effects.makeShake("word_shake",
          hot_config.getInt("animation", "shake_width"),
          hot_config.getFloat("animation", "shake_duration"));
        effects.start("word_shake");
      }
    });
  }

  if (input_val == 50) { // maze!
    current_letter = "maze";
    current_word = "maze";
    timing.makeSequenceWithFunction("speech", {(float)(animation_duration + 0.25), 0.75, 1.5, 1.5, 2.0}, [this](int i, float f) { 
    if (i == 1) {
      sound.playSound("m", 1);
      effects.makeShake("letter_shake",
        hot_config.getInt("animation", "shake_width"),
        hot_config.getFloat("animation", "shake_duration"));
      effects.start("letter_shake");
    } else if (i == 2) {
      sound.playSound("is_for", 1);
    } else if (i == 3) {
      sound.playSound(current_word, 1);
      effects.makeShake("word_shake",
        hot_config.getInt("animation", "shake_width"),
        hot_config.getFloat("animation", "shake_duration"));
      effects.start("word_shake");
    } else if (i == 4) {
      sound.playSound("can_you_say", 1);
    } else if (i == 5) {
      sound.playSound(current_word, 1);
      effects.makeShake("word_shake",
        hot_config.getInt("animation", "shake_width"),
        hot_config.getFloat("animation", "shake_duration"));
      effects.start("word_shake");
    }
  });
  }

  if (input.threeQuickKey("return")) {
    if (mode == "full_keyboard") {
      mode = "number_pad";
    } else {
      mode = "full_keyboard";
    }
  }

  if (input.threeQuickKey("escape")) {
    screenmanager.setQuit();
  }

  timing.doSequence("speech");
}

void ABearCs::render() {
  graphics.clearScreen(screen_color);
  graphics.draw2D();

  int x, y;

  // Current picture
  if (current_letter != "") {
    x = picture_x;
    y = picture_y;
    if (timing.locked("animation")) {
      y += effects.tween("slide_last_letter", tween_type);
    }

    if (current_letter != "maze") {
      if (current_letter == "shapes") {
        graphics.setColor(current_color, 1.0);
      } else {
        graphics.setColor("#FFFFFF", 1.0);
      }
      graphics.pushModelMatrix();
      graphics.scale(1.0 + effects.oscillation("squish"), 1.0 - effects.oscillation("squish"), 1.0);
      graphics.translate(-400 * effects.oscillation("squish"), 600 * effects.oscillation("squish"), 0);
      graphics.drawImage(current_word, x, y);
      graphics.popModelMatrix();
    } else {
      renderMaze(x, y);
    }

    x = letter_x;
    y = letter_y;
    if (timing.locked("animation")) {
      y += effects.tween("slide_last_letter", tween_type);
    }
    if (timing.locked("key_swap")) {
      x += effects.shake("letter_shake");
      y += effects.shake("letter_shake");
    }
    if (current_letter.length() == 1) {
      letter_box->setText(boost::to_upper_copy(current_letter));
      letter_box->setPosition({x, y});
      letter_box->draw();
    } else if (current_letter == "maze") {
      letter_box->setText("M");
      letter_box->setPosition({x, y});
      letter_box->draw();
    }

    x = word_x;
    y = word_y;
    if (timing.locked("animation")) {
      y += effects.tween("slide_last_letter", tween_type);
    }
    x += effects.shake("word_shake");
    y += effects.shake("word_shake");
    string cap_word = boost::to_upper_copy(current_word.substr(0,1)) + current_word.substr(1);
    word_box->setText(cap_word);
    word_box->setPosition({x, y});
    word_box->draw();
  }

  // Last picture, if animating
  if (timing.locked("animation") && last_letter != "") {
    x = picture_x;
    y = picture_y - screen_height + effects.tween("slide_last_letter", tween_type);
    if (last_letter != "maze") {
      if (last_letter == "shapes") {
        graphics.setColor(last_color, 1.0);
      } else {
        graphics.setColor("#FFFFFF", 1.0);
      }
      graphics.drawImage(last_word, x, y);
    } else {
      renderMaze(x, y);
    }

    x = letter_x;
    y = letter_y - screen_height + effects.tween("slide_last_letter", tween_type);
    if (last_letter.length() == 1) {
      letter_box->setText(boost::to_upper_copy(last_letter));
      letter_box->setPosition({x, y});
      letter_box->draw();
    } else if (current_letter == "maze") {
      letter_box->setText("M");
      letter_box->setPosition({x, y});
      letter_box->draw();
    }

    x = word_x;
    y = word_y - screen_height + effects.tween("slide_last_letter", tween_type);
    string cap_word = boost::to_upper_copy(last_word.substr(0,1)) + last_word.substr(1);
    word_box->setText(cap_word);
    word_box->setPosition({x, y});
    word_box->draw();
  }
}

void ABearCs::renderMaze(int x, int y) {
  graphics.pushModelMatrix();
  graphics.setColor("#FFFFFF", 1.0);
  graphics.translate(x, y, 0);
  graphics.translate(-180, -230, 0);
  graphics.scale(2.0, 2.0, 1.0);
  for (int i = 0; i < 16; i++) {
    for (int j = 0; j < 16; j++) {
      if (maze[i][j] < 0) graphics.drawImage("wall", 24 * i, 24 * j);
      if (maze[i][j] > 0) graphics.drawImage("tile_" + std::to_string(maze[i][j]), 24 * i, 24 * j);
    }
  }

  graphics.translate(0, 40 * effects.oscillation("squish"), 0);
  runner_sprite->draw();
  graphics.popModelMatrix();
}

ABearCs::~ABearCs() {
  delete letter_box;
  delete word_box;
  delete runner_sprite;
}