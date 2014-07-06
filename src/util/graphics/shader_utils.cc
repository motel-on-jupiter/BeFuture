#include "shader_utils.h"

#include <cstdio>
#include <cstdlib>
#include <GL/glew.h>

#include "util/logging/Logger.h"

#define GLSL(src) "#version 150 core\n" #src

/**
 * Store all the file's contents in memory, useful to pass shaders
 * source code to OpenGL
 */
static const char *file_read(const char *path) {
  FILE* in;
  errno_t error = fopen_s(&in, path, "rb");
  if (error != 0) {
    return NULL;
  }

  int res_size = BUFSIZ;
  char* res = static_cast<char *>(malloc(res_size));
  int nb_read_total = 0;

  while (!feof(in) && !ferror(in)) {
    if (nb_read_total + BUFSIZ > res_size) {
      if (res_size > 10 * 1024 * 1024) {
        break;
      }
      res_size = res_size * 2;
      res = static_cast<char *>(realloc(res, res_size));
    }
    char* p_res = res + nb_read_total;
    nb_read_total += fread(p_res, 1, BUFSIZ, in);
  }

  fclose(in);
  res = static_cast<char *>(realloc(res, nb_read_total + 1));
  res[nb_read_total] = '\0';
  return res;
}

/**
 * Display compilation errors from the OpenGL shader compiler
 */
static void print_log(GLuint object) {
  GLint log_length = 0;
  if (glIsShader(object)) {
    glGetShaderiv(object, GL_INFO_LOG_LENGTH, &log_length);
  } else if (glIsProgram(object)) {
    glGetProgramiv(object, GL_INFO_LOG_LENGTH, &log_length);
  } else {
    LOGGER.Error("printlog: Not a shader or a program");
    return;
  }

  char *log = static_cast<char*>(malloc(log_length));

  if (glIsShader(object)) {
    glGetShaderInfoLog(object, log_length, NULL, log);
  } else if (glIsProgram(object)) {
    glGetProgramInfoLog(object, log_length, NULL, log);
  }

  LOGGER.Error("%s", log);
  free(log);
}

/**
 * Compile the shader from file 'path', with error handling
 */
static GLuint create_shader(const char *path, GLenum type) {
  const GLchar* source = file_read(path);
  if (NULL == source) {
    LOGGER.Error("Error opening %s: ", path);
    return 0;
  }
  GLuint res = glCreateShader(type);
  glShaderSource(res, 1, &source, NULL);
  free((void*) source);

  glCompileShader(res);
  GLint compile_ok = GL_FALSE;
  glGetShaderiv(res, GL_COMPILE_STATUS, &compile_ok);
  if (GL_FALSE == compile_ok) {
    LOGGER.Error("%s:", path);
    print_log(res);
    glDeleteShader(res);
    return 0;
  }

  return res;
}

GLuint create_shader_program(const char *vspath, const char *fspath) {
  GLuint program = glCreateProgram();
  GLuint shader;

  if (vspath) {
    shader = create_shader(vspath, GL_VERTEX_SHADER);
    if (0 == shader) {
      return 0;
    }
    glAttachShader(program, shader);
  }

  if (fspath) {
    shader = create_shader(fspath, GL_FRAGMENT_SHADER);
    if (0 == shader) {
      return 0;
    }
    glAttachShader(program, shader);
  }

  glLinkProgram(program);
  GLint link_ok = GL_FALSE;
  glGetProgramiv(program, GL_LINK_STATUS, &link_ok);
  if (GL_FALSE == link_ok) {
    LOGGER.Error("glLinkProgram:");
    print_log(program);
    glDeleteProgram(program);
    return 0;
  }

  return program;
}

#ifdef GL_GEOMETRY_SHADER
static GLuint create_shader_program_impl(const char *vspath, const char *gspath,
                                         const char *fspath) {
  GLuint program = glCreateProgram();
  GLuint shader;

  if (vspath) {
    shader = create_shader(vspath, GL_VERTEX_SHADER);
    if (0 == shader) {
      return 0;
    }
    glAttachShader(program, shader);
  }

  if (gspath) {
    shader = create_shader(gspath, GL_GEOMETRY_SHADER);
    if (0 == shader) {
      return 0;
    }
    glAttachShader(program, shader);
  }

  if (fspath) {
    shader = create_shader(fspath, GL_FRAGMENT_SHADER);
    if (0 == shader) {
      return 0;
    }
    glAttachShader(program, shader);
  }

  glLinkProgram(program);
  GLint link_ok = GL_FALSE;
  glGetProgramiv(program, GL_LINK_STATUS, &link_ok);
  if (GL_FALSE == link_ok) {
    LOGGER.Error("glLinkProgram:");
    print_log(program);
    glDeleteProgram(program);
    return 0;
  }

  return program;
}
#else
static GLuint create_shader_program_impl(const char *vspath, const char *gspath, const char *fspath, GLint input, GLint output, GLint vertices) {
  LOGGER.Error("Missing support for geometry shaders.");
  return 0;
}
#endif

GLuint create_shader_program(const char *vspath, const char *gspath,
                             const char *fspath) {
  if (nullptr == gspath) {
    return create_shader_program(vspath, fspath);
  } else {
    return create_shader_program_impl(vspath, gspath, fspath);
  }
}

void delete_shader_program(GLuint name) {
  if (0 != name) {
    glDeleteProgram(name);
  }
}
