/*
  BearsLoveHoney
  Matthew Carlin
  Copyright 2018
*/

#include "puzzle.h"

using namespace std;
using namespace Honey;

Puzzle::Puzzle() {
}

void Puzzle::loop() {
  logic();
  render();
}

void Puzzle::initialize() {
  screen_width = hot_config.getInt("layout", "screen_width");
  screen_height = hot_config.getInt("layout", "screen_height");

  // int margin_x = 30;
  // int margin_y = 50;

  sound.addSound("puzzle_success", "Sound/Keyboard/C_BellRiser.wav");

  generate(math_utils.randomInt(1, 13));
  jumble();
}

void Puzzle::generate(int number) {

  original_positions = {};
  puzzle_pieces = {};
  shuffled_puzzle_pieces = {};

  string puzzle_path = "Art/Puzzle/puzzle_" + to_string(number) + ".png";

  // Use the SDL to load the image
  SDL_Surface* image = IMG_Load(puzzle_path.c_str());
  if (image == nullptr) {
    printf("IMG_Load failed for %s with error: %s\n", puzzle_path.c_str(), IMG_GetError());
    exit(1);
  }

  Uint32 rmask, gmask, bmask, amask;
  rmask = 0x00ff0000;
  gmask = 0x0000ff00;
  bmask = 0x000000ff;
  amask = 0xff000000;

  piece_width = image->w / puzzle_width;
  piece_height = image->h / puzzle_height;
  int margin_x = (screen_width - image->w) / 2.0;
  int margin_y = (screen_height - image->h) / 2.0;

  for (int i = 0; i < puzzle_height; i++) {
    for (int j = 0; j < puzzle_width; j++) {
      SDL_Rect source;
      source.x = j * piece_width;
      source.y = i * piece_height;
      source.w = piece_width;
      source.h = piece_height;

      SDL_Rect dest;
      dest.x = 0;
      dest.y = 0;
      dest.w = piece_width;
      dest.h = piece_height;

      SDL_Surface* slice = SDL_CreateRGBSurface(0, piece_width + 50, piece_height + 50, 32, rmask, gmask, bmask, amask);;
      SDL_BlitSurface(image, &source, slice, &dest);

      SDL_LockSurface(slice);
      SDL_LockSurface(image);
      unsigned int *slice_ptr = (unsigned int*)slice->pixels;
      unsigned int *image_ptr = (unsigned int*)image->pixels;

      for (int k = (piece_width / 2.0 - cut_size / 2.0); k < (piece_width / 2.0 + cut_size / 2.0); k++) {
        for (int l = piece_height; l < piece_height + cut_size; l++) {
          int sliceoffset = l * slice->w;
          int imageoffset = (i * piece_height + l) * image->w;
          int antisliceoffset = sliceoffset - piece_height * slice->w;
          int antiimageoffset = ((i - 1) * piece_height + l) * image->w;
          
          Uint32 mask = 0xffffffff;
          Uint32 antimask = 0x00ffffff;
          if (k < piece_width / 2.0) {
            int x = k - (piece_width / 2.0 - cut_size / 2.0);
            int y = l - piece_height;
            float param = ((2.0 * x / cut_size) * 2.0) - 1.0;
            float height = cut_size * (1.0 / (1 + exp(-1 * 5 * param)));
            if (y > height) {
              mask = 0x00ffffff;
              antimask = 0xffffffff;
            }
          } else if (k > piece_width / 2.0) {
            int x = piece_width / 2.0 + cut_size / 2.0 - k;
            int y = l - piece_height;
            float param = ((2.0 * x / cut_size) * 2.0) - 1.0;
            float height = cut_size * (1.0 / (1 + exp(-1 * 5 * param)));
            if (y > height) {
              mask = 0x00ffffff;
              antimask = 0xffffffff;
            }
          }
          int radius = 20;
          int x = k - piece_width / 2.0;
          int y = l - piece_height - 20;
          if (x*x + y*y < radius*radius) {
            mask = 0xffffffff;
            antimask = 0x00ffffff;
          }

          if (i < puzzle_height - 1) {
            slice_ptr[sliceoffset + k] = image_ptr[imageoffset + (j * piece_width) + k] & mask;
          }
          if (i > 0) {
            slice_ptr[antisliceoffset + k] = image_ptr[antiimageoffset + (j * piece_width) + k] & antimask;
          }
        }
      }

      for (int k = piece_width; k < piece_width + cut_size; k++) {
        for (int l = (piece_height / 2.0 - cut_size / 2.0); l < (piece_height / 2.0 + cut_size / 2.0); l++) {
          int sliceoffset = l * slice->w;
          int imageoffset = (i * piece_height + l) * image->w;
          
          Uint32 mask = 0xffffffff;
          Uint32 antimask = 0x00ffffff;
          if (l < piece_height / 2.0) {
            int x = k - piece_width;
            int y = l - (piece_height / 2.0 - cut_size / 2.0);
            float param = ((2.0 * y / cut_size) * 2.0) - 1.0;
            float width = cut_size * (1.0 / (1 + exp(-1 * 5 * param)));
            if (x > width) {
              mask = 0x00ffffff;
              antimask = 0xffffffff;
            }
          } else if (l > piece_height / 2.0) {
            int x = k - piece_width;
            int y = piece_height / 2.0 + cut_size / 2.0 - l;
            float param = ((2.0 * y / cut_size) * 2.0) - 1.0;
            float width = cut_size * (1.0 / (1 + exp(-1 * 5 * param)));
            if (x > width) {
              mask = 0x00ffffff;
              antimask = 0xffffffff;
            }
          }
          int radius = 20;
          int x = k - piece_width - 20;
          int y = l - piece_height / 2.0;
          if (x*x + y*y < radius*radius) {
            mask = 0xffffffff;
            antimask = 0x00ffffff;
          }

          if (j < puzzle_width - 1) {
            slice_ptr[sliceoffset + k] = image_ptr[imageoffset + (j * piece_width) + k] & mask;
          }
          if (j > 0) {
            slice_ptr[sliceoffset - piece_width + k] = image_ptr[imageoffset + ((j-1) * piece_width) + k] & antimask;
          }
        }
      }

      SDL_UnlockSurface(slice);
      SDL_UnlockSurface(image);

      graphics.addImageFromSurface("puzzle_piece_" + to_string(i) + "_" + to_string(j), slice);

      original_positions["puzzle_piece_" + to_string(i) + "_" + to_string(j)] = (position) {
        margin_x + j * piece_width + (int) (slice->w / 2.0),
        margin_y + i * piece_height + (int) (slice->h / 2.0)
      };
      printf("Values %d %d give %d %d\n", i, j,
        margin_x + j * piece_width + (int) (slice->w / 2.0),
        margin_y + i * piece_height + (int) (slice->h / 2.0)
      );

      puzzle_pieces.push_back(
        new Sprite(
          "puzzle_piece_" + to_string(i) + "_" + to_string(j),
          original_positions["puzzle_piece_" + to_string(i) + "_" + to_string(j)],
          "#ffffff", 1.0, 0.0, 1.0
        )
      );

      SDL_FreeSurface(slice);
    }
  }

  SDL_FreeSurface(image);
}

void Puzzle::logic() {
  // Check and load configuration every 2 seconds
  hot_config.checkAndUpdate();

  // Set a bunch of variables from configuration
  screen_color = hot_config.getString("layout", "screen_color");

  if (input.anyKeyPressed() && !timing.locked("puzzle_key_lock")) {
    if (shuffled_puzzle_pieces.size() == 0) {
      generate(math_utils.randomInt(1, 13));
      jumble();
    } else {
      Sprite* sprite = shuffled_puzzle_pieces.back();
      shuffled_puzzle_pieces.pop_back();
      string label = sprite->getLabel();
      sprite->setPosition(original_positions[label], 0.3);
      sprite->setRotation(0, 0.3);
      if (shuffled_puzzle_pieces.size() == 0) {
        sound.playSound("puzzle_success", 1);
      }
    }
    timing.lock("puzzle_key_lock", 0.3);
  }

  if (input.threeQuickKey("escape")) {
    screenmanager.setQuit();
  }
}

void Puzzle::jumble() {
  for(Sprite* sprite : puzzle_pieces) {
    shuffled_puzzle_pieces.push_back(sprite);
    random_shuffle(shuffled_puzzle_pieces.begin(), shuffled_puzzle_pieces.end());
    for(Sprite* sprite : shuffled_puzzle_pieces) {
      sprite->setPosition((position) {
        math_utils.randomInt(50, screen_width - 50),
        math_utils.randomInt(50, screen_height - 50)
      }, 0.3);
      sprite->setRotation(math_utils.randomInt(0, 180), 0.3);
    }
  }
}

void Puzzle::render() {
  // Clear the screen to a soft white color
  graphics.clearScreen(screen_color);

  // for (int i = 0; i < puzzle_height; i++) {
  //   for (int j = 0; j < puzzle_width; j++) {
  //     graphics.drawImage("puzzle_piece_" + to_string(i) + "_" + to_string(j), 30 + j * (piece_width+5), 50 + i * (piece_height+10));
  //   }
  // }

  for(Sprite* sprite : puzzle_pieces) {
    sprite->draw();
  }

  // Switch to 2D drawing mode
  graphics.draw2D();
}

Puzzle::~Puzzle() {

}