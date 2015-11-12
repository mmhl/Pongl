#include <iostream>
#include <chrono>
#include <thread>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <GL/glew.h>
#include <sstream>

#include "Game.h"
#include "Shader.h"
using namespace std;

GLuint createShaderProgram(const std::vector<GLuint> &shaders) {
  GLuint program = glCreateProgram();
  for (int i = 0; i < shaders.size(); i++) {
    glAttachShader(program, shaders[0]);
  }
  glLinkProgram(program);
  GLint status;
  glGetProgramiv(program, GL_LINK_STATUS, &status);
  if (status == GL_FALSE) {
    cerr << "Failed to create program" << endl;
  }
  for (int i = 0; i < shaders.size(); i++) {
    glDetachShader(program, shaders[i]);
  }
  glUseProgram(program);
  return program;

}

int initializeShaders() {
  const string fragmentFilePath = "/home/mhl/Sources/Pongl/shaders/fragment.glsl";
  const string vertexFilePath = "/home/mhl/Sources/Pongl/shaders/vertex.glsl";

  Shader fragmentShader(GL_FRAGMENT_SHADER);
  fragmentShader.loadFromFile(fragmentFilePath);

  Shader vertexShader(GL_VERTEX_SHADER);
  vertexShader.loadFromFile(vertexFilePath);


  GLint status = fragmentShader.compile();
  if (status == GL_FALSE) {
    cerr << "Compilation of fragment shader failed" << endl;
    cout << fragmentShader.getCompilationLog() << endl;
    return -1;
  }

  status = vertexShader.compile();
  if (status == GL_FALSE) {
    cerr << "Compilation of vertex shader failed" << endl;
    cout << vertexShader.getCompilationLog() << endl;
    return -1;
  }

  return 0;
}

int main() {
  Game newGame;
  int ret = 0;
  ret = newGame.init("Pongl");
  if (ret) {
    cerr << "Failed to launch Pongl. Error code: " << ret << endl;
  }
  newGame.loop();
  newGame.cleanup();
  return 0;
}