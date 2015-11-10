#include <iostream>
#include <SDL.h>
#include <chrono>
#include <thread>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <GL/glew.h>
#include <sstream>

#include "Game.h"


using namespace std;

string getShaderCompilationLog(GLuint shader) {
  GLint msgSize = 0;
  glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &msgSize);
  if (msgSize < 0) {
    cerr << "shaderiv returned wrong param!" << endl;
    return string("shader returned wrong parameter");
  }
  std::vector<GLchar> log((unsigned long) msgSize);
  glGetShaderInfoLog(shader, msgSize, NULL, &log[0]);
  string msg(log.begin(), log.end());
  return msg;
}

GLuint loadShaderFromFile(GLenum type, const std::string &filePath) {
  using namespace boost::filesystem;
  GLuint shader = glCreateShader(type);
  path file(filePath);
  boost::system::error_code error;
  if (!exists(file, error)) {
    cerr << "Error: " << error.message() << endl;
    return 0;
  }

  boost::filesystem::ifstream fstream(file);
  std::stringstream buffer;
  buffer << fstream.rdbuf();
  std::string result = buffer.str();

  const char *tmp = result.c_str();
  glShaderSource(shader, 1, &tmp, nullptr);

  return shader;
}

GLint compileShader(GLenum shaderType, GLuint shader) {
  glCompileShader(shader);
  GLint status;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
  if (status == GL_FALSE) {
    stringstream ss;
    ss << "Failed to compile ";
    switch (shaderType) {
      case GL_VERTEX_SHADER:
        ss << "vertex shader";
        break;
      case GL_FRAGMENT_SHADER:
        ss << "fragment shader";
        break;
      default:
        ss << "unknown shader";
        break;
    }
    ss << "\n" << getShaderCompilationLog(shader);
    cerr << ss.str() << endl;
  }
  return status;
}

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

  GLuint fragmentShader = loadShaderFromFile(GL_FRAGMENT_SHADER, fragmentFilePath);
  GLuint vertexShader = loadShaderFromFile(GL_VERTEX_SHADER, vertexFilePath);

  GLint status = compileShader(GL_FRAGMENT_SHADER, fragmentShader);
  if (status == GL_FALSE) {
    cerr << "Failed to initialize shaders" << endl;
    return -1;
  }
  status = compileShader(GL_VERTEX_SHADER, vertexShader);
  if (status == GL_FALSE) {
    cerr << "Failed to initialize shaders" << endl;
    return -1;
  }

  std::vector<GLuint> shaders;
  shaders.push_back(fragmentShader);
  shaders.push_back(vertexShader);

  status = createShaderProgram(shaders);
  if (status == GL_FALSE) {
    cerr << "Failed to create shader program" << endl;
    return -2;
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

  return 0;
}