//
// Created by mhl on 11/10/15.
//

#ifndef PONGL_GAME_H
#define PONGL_GAME_H

#include <SDL.h>
#include <string>


class Game {
 public:
  int init(const std::string &title);
  void cleanup();
  int handleEvents();
  int render();
  void loop();

 private:
  int setupGL();
  int setupGLEW();
  bool mRunning;
  SDL_Window *mSDLWindow;
  SDL_Renderer *mSDLRednerer;
  SDL_GLContext mGLContext;
};


#endif //PONGL_GAME_H
