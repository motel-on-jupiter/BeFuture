// Read an INI file into easy-to-access name/value pairs.

#include "GLINIReader.h"

#include "util/logging/Logger.h"
#include "util/wrapper/glm_wrap.h"

GLfloat GLINIReader::GetFloating(std::string section, std::string name,
                                 float default_value) {
  return static_cast<GLfloat>(GetReal(section, name,
                                      static_cast<double>(default_value)));
}

glm::vec2 GLINIReader::GetVector2(std::string section, std::string name,
                                  glm::vec2 default_value) {
  std::string valstr = Get(section, name, "");
  const char* value = valstr.c_str();
  float buf[2] = { 0 };
  int ret = sscanf_s(value, "(%f, %f)", &(buf[0]), &(buf[1]));
  if (2 != ret) {
    LOGGER.Error("Failed to parse the config setting (ret: %d, config: %s)",
                 ret, value);
    return default_value;
  }
  return glm::vec2(buf[0], buf[1]);
}

glm::vec3 GLINIReader::GetVector3(std::string section, std::string name,
                                  glm::vec3 default_value) {
  std::string valstr = Get(section, name, "");
  const char* value = valstr.c_str();
  float buf[3] = { 0 };
  int ret = sscanf_s(value, "(%f, %f, %f)", &(buf[0]), &(buf[1]), &(buf[2]));
  if (3 != ret) {
    LOGGER.Error("Failed to parse the config setting (ret: %d, config: %s)",
                 ret, value);
    return default_value;
  }
  return glm::vec3(buf[0], buf[1], buf[2]);
}

glm::vec4 GLINIReader::GetVector4(std::string section, std::string name,
                                  glm::vec4 default_value) {
  std::string valstr = Get(section, name, "");
  const char* value = valstr.c_str();
  float buf[4] = { 0 };
  int ret = sscanf_s(value, "(%f, %f, %f, %f)", &(buf[0]), &(buf[1]), &(buf[2]),
                     &(buf[3]));
  if (4 != ret) {
    LOGGER.Error("Failed to parse the config setting (ret: %d, config: %s)",
                 ret, value);
    return default_value;
  }
  return glm::vec4(buf[0], buf[1], buf[2], buf[3]);
}

glm::i32vec2 GLINIReader::GetIntVector2(std::string section, std::string name,
                                        glm::i32vec2 default_value) {
  std::string valstr = Get(section, name, "");
  const char* value = valstr.c_str();
  int buf[2] = { 0 };
  int ret = sscanf_s(value, "(%d, %d)", &(buf[0]), &(buf[1]));
  if (2 != ret) {
    LOGGER.Error("Failed to parse the config setting (ret: %d, config: %s)",
                 ret, value);
    return default_value;
  }
  return glm::i32vec2(buf[0], buf[1]);
}

glm::i32vec3 GLINIReader::GetIntVector3(std::string section, std::string name,
                                        glm::i32vec3 default_value) {
  std::string valstr = Get(section, name, "");
  const char* value = valstr.c_str();
  float buf[3] = { 0 };
  int ret = sscanf_s(value, "(%d, %d, %d)", &(buf[0]), &(buf[1]), &(buf[2]));
  if (3 != ret) {
    LOGGER.Error("Failed to parse the config setting (ret: %d, config: %s)",
                 ret, value);
    return default_value;
  }
  return glm::i32vec3(buf[0], buf[1], buf[2]);
}

glm::i32vec4 GLINIReader::GetIntVector4(std::string section, std::string name,
                                        glm::i32vec4 default_value) {
  std::string valstr = Get(section, name, "");
  const char* value = valstr.c_str();
  int buf[4] = { 0 };
  int ret = sscanf_s(value, "(%d, %d, %d, %d)", &(buf[0]), &(buf[1]), &(buf[2]),
                     &(buf[3]));
  if (4 != ret) {
    LOGGER.Error("Failed to parse the config setting (ret: %d, config: %s)",
                 ret, value);
    return default_value;
  }
  return glm::i32vec4(buf[0], buf[1], buf[2], buf[3]);
}
