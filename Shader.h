//
// Created by mhl on 11.11.15.
//

#ifndef PONGL_SHADER_H
#define PONGL_SHADER_H

#include <string>
#include <GL/glew.h>

class Shader {

 public:
  Shader(GLenum type);
  int loadFromFile(const std::string &path);
  GLint compile();
  std::string getCompilationLog();
 private:
  GLint mStatus; // Last status.
  GLuint mShader;
  GLenum mType;
};


#endif //PONGL_SHADER_H
