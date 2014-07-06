/**
 * Copyright (C) 2014 The Motel On Jupiter
 */
#ifndef COLORBAR_H_
#define COLORBAR_H_

#include "util/wrapper/spark_wrap.h"

class BitmapTexture;
struct SDL_Surface;

class ColorBar {
 public:
  ColorBar();
  virtual ~ColorBar();

  int Initialize(glm::vec2 pos, glm::vec2 scale);
  int Finalize();

  void Render(glm::vec2 windowSize);

  GLuint texname() const {
    return texname_;
  }

 private:
  SDL_Surface *texture_;
  GLuint texname_;
  glm::vec2 pos_;
  glm::vec2 scale_;
};

#endif /* COLORBAR_H_ */
