/**
 * Copyright (C) 2014 The Motel On Jupiter
 */
#ifndef GLOT_H_
#define GLOT_H_

/**
 * Original OpenGL extension
 */

#include "util/wrapper/glgraphics_wrap.h"

extern void glotRectangle(int facevalid = 0xFF);
extern void glotCylinder(int sides);
extern void glotWireBox(GLfloat width, GLfloat height, GLfloat depth);
extern void glotSolidBox(GLfloat width, GLfloat height, GLfloat depth);
extern void glotSolidBoxFace(GLfloat width, GLfloat height, GLfloat depth,
                             GLuint face);

#endif /* GLOT_H_ */
