/**
 * Copyright (C) 2014 The Motel on Jupiter
 */
#ifndef SKY_SPHERE_H_
#define SKY_SPHERE_H_

#include <GL/freeglut.h>

struct SDL_Surface;
class GLUquadric;

class SkySphere {
 public:
  SkySphere();
  virtual ~SkySphere();

  int Initialize();
  int Finalize();

  void Render();

 private:
  GLUquadric *sphere_;
  SDL_Surface *texture_;
  GLuint tex_name_;
};

#endif /* SKY_SPHERE_H_ */
