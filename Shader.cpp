//
// Created by mhl on 11.11.15.
//

#include "Shader.h"

#include <iostream>
#include <sstream>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

int Shader::loadFromFile(const std::string &path) {
  using namespace boost::filesystem;
  boost::filesystem::path file(path);
  boost::system::error_code error;
  if (!exists(file, error)) {
    return -1;
  }
  boost::filesystem::ifstream fstream(file);
  std::stringstream buffer;
  buffer << fstream.rdbuf();
  std::string result = buffer.str();

  const char *tmp = result.c_str();
  glShaderSource(mShader, 1, &tmp, nullptr);

  return 0; // Success
}

GLint Shader::compile() {
  glCompileShader(mShader);
  glGetShaderiv(mShader, GL_COMPILE_STATUS, &mStatus);
  return mStatus;
}


Shader::Shader(GLenum type) {
  mShader = glCreateShader(type);
  mType = type;
}

std::string Shader::getCompilationLog() {
  GLint msgSize = 0;
  glGetShaderiv(mShader, GL_INFO_LOG_LENGTH, &msgSize);
  std::vector<GLchar> log((unsigned long) msgSize);
  glGetShaderInfoLog(mShader, msgSize, NULL, &log[0]);
  std::string msg(log.begin(), log.end());
  return msg;
}
