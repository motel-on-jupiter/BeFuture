#ifndef __GLINIREADER_H__
#define __GLINIREADER_H__

#include "INIReader.h"

class GLINIReader : public INIReader {
 public:
  GLINIReader(std::string filename)
      : INIReader(filename) {
  }

  GLfloat GetFloating(std::string section, std::string name,
                      float default_value);
  glm::vec2 GetVector2(std::string section, std::string name,
                       glm::vec2 default_value);
  glm::vec3 GetVector3(std::string section, std::string name,
                       glm::vec3 default_value);
  glm::vec4 GetVector4(std::string section, std::string name,
                       glm::vec4 default_value);
  glm::i32vec2 GetIntVector2(std::string section, std::string name,
                             glm::i32vec2 default_value);
  glm::i32vec3 GetIntVector3(std::string section, std::string name,
                             glm::i32vec3 default_value);
  glm::i32vec4 GetIntVector4(std::string section, std::string name,
                             glm::i32vec4 default_value);
};

#endif  // __GLINIREADER_H__