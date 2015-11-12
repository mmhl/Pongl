//
// Created by mhl on 11/10/15.
//

#include <GL/glew.h>
#include "Game.h"
#include "Video.h"
/*
 * Initialize Window, contexts, OpenGL, etc.
 */
int Game::init(const std::string &title) {
  if (SDL_Init(SDL_INIT_EVERYTHING)) {
	return -1;
  }

  mSDLWindow = SDL_CreateWindow(title.c_str(), 0, 0, Video::VIDEO_RES_HOR, Video::VIDEO_RES_VERT,
								SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
  if (mSDLWindow == nullptr) {
	return -1;
  }

  mSDLRednerer = SDL_CreateRenderer(mSDLWindow, -1, SDL_RENDERER_ACCELERATED);
  SDL_SetRenderDrawColor(mSDLRednerer, 255, 255, 255, 255);
  SDL_RenderClear(mSDLRednerer);
  if (mSDLRednerer == nullptr) {
	return -2;
  }

  if (setupGL() != 0) {
	return -3;
  }
  mGLContext = SDL_GL_CreateContext(mSDLWindow);

  if (setupGLEW()) {
	return -5;
  }

  if (setupGL()) {
	return -6;
  }

  mRunning = true;
  return 0;
}

void Game::cleanup() {
  SDL_GL_DeleteContext(mGLContext);
  SDL_DestroyRenderer(mSDLRednerer);
  SDL_DestroyWindow(mSDLWindow);
}

int Game::handleEvents() {
  SDL_Event event;
  if (SDL_PollEvent(&event)) {
    switch (event.type) {
      case SDL_QUIT:
        mRunning = false;
        break;
    }
  }
  return 0;
}

int Game::render() {
  SDL_RenderClear(mSDLRednerer);
  // Things to draw
  SDL_RenderPresent(mSDLRednerer);
  return 0;
}

void Game::loop() {
  while (mRunning) {
    handleEvents();
    render();
  }
}

int Game::setupGL() {
  int ret;
  ret = SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
  ret |= SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 4);
  ret |= SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

  ret |= SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
  ret |= SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  ret |= SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

  if (ret != 0) {
	return -1;
  }
  return 0;
}
int Game::setupGLEW() {
  glewExperimental = GL_TRUE;
  GLenum res = glewInit();
  if (res != 0) {
	return -1;
  }
  return 0;
}
