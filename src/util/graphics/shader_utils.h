#ifndef SHADER_UTILS_H_
#define SHADER_UTILS_H_

#include <GL/glew.h>

extern GLuint create_shader_program(const char *vspath, const char *fspath);
extern GLuint create_shader_program(const char *vspath, const char *gspath,
                                    const char *fspath);
extern void delete_shader_program(GLuint name);

#endif /* SHADER_UTILS_H_ */
