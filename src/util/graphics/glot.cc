/**
 * Copyright (C) 2014 The Motel On Jupiter
 */
#include "util/graphics/glot.h"

void glotRectangle(int facevalid) {
  static const GLdouble kVertices[8][3] = { { -0.5, -0.5, 0.5 },  //0
      { 0.5, -0.5, 0.5 },  //1
      { 0.5, 0.5, 0.5 },  //2
      { -0.5, 0.5, 0.5 },  //3
      { 0.5, -0.5, -0.5 },  //4
      { -0.5, -0.5, -0.5 },  //5
      { -0.5, 0.5, -0.5 },  //6
      { 0.5, 0.5, -0.5 }  //7
  };

  static const GLint kIndices[6][4] = { { 0, 1, 2, 3 }, { 4, 5, 6, 7 }, { 1, 4,
      7, 2 }, { 3, 6, 5, 0 }, { 3, 2, 7, 6 }, { 1, 0, 5, 4 }, };

  static const GLdouble kNormals[6][3] = { { 0.0, 0.0, 1.0 },
      { 0.0, 0.0, -1.0 }, { 1.0, 0.0, 0.0 }, { -1.0, 0.0, 0.0 }, { 0.0, -1.0,
          0.0 }, { 0.0, 1.0, 0.0 }, };

  int i, j;
  GLint index;

  glBegin(GL_QUADS);
  for (i = 0; i < 6; ++i) {
    if (0 == (facevalid & (1 << i))) {
      continue;
    }
    glNormal3dv(kNormals[i]);
    for (j = 0; j < 4; ++j) {
      index = kIndices[i][j];
      glVertex3dv(kVertices[index]);
    }
  }
  glEnd();

  return;
}

void glotCylinder(int sides) {
  // top face
  glBegin(GL_POLYGON);
  glNormal3f(0.0f, 1.0f, 0.0f);
  for (int i = 0; i <= sides; ++i) {
    float t = i * 2.0f * glm::pi<float>() / static_cast<float>(sides);
    glVertex3f(0.5f * glm::cos(t), 0.5f, 0.5f * glm::sin(t));
  }
  glEnd();

  // side faces
  glBegin(GL_QUAD_STRIP);
  for (int i = 0; i <= sides; ++i) {
    float t = i * 2.0f * glm::pi<float>() / static_cast<float>(sides);
    glNormal3f(1.0f * glm::cos(t), 0.0f, 1.0f * glm::sin(t));
    glVertex3f(0.5f * glm::cos(t), -0.5f, 0.5f * glm::sin(t));
    glVertex3f(0.5f * glm::cos(t), 0.5f, 0.5f * glm::sin(t));
  }
  glEnd();

  // bottom face
  glBegin(GL_POLYGON);
  glNormal3f(0.0f, -1.0f, 0.0f);
  for (int i = sides; i >= 0; --i) {
    float t = i * 2.0f * glm::pi<float>() / static_cast<float>(sides);
    glVertex3f(0.5f * glm::cos(t), -0.5f, 0.5f * glm::sin(t));
  }
  glEnd();
}

static void glotBoxFace(GLfloat width, GLfloat height, GLfloat depth,
                        GLuint face) {
  static GLfloat kNormals[6][3] = { { -1.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f },
      { 1.0f, 0.0f, 0.0f }, { 0.0f, -1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f,
          0.0f, -1.0f } };
  static GLuint kFaces[6][4] = { { 0, 1, 2, 3 }, { 3, 2, 6, 7 }, { 7, 6, 5, 4 },
      { 4, 5, 1, 0 }, { 5, 6, 2, 1 }, { 7, 4, 0, 3 } };

  GLfloat vertices[8][3];
  vertices[0][0] = vertices[1][0] = vertices[2][0] = vertices[3][0] = -width
      / 2.0f;
  vertices[4][0] = vertices[5][0] = vertices[6][0] = vertices[7][0] = width
      / 2.0f;
  vertices[0][1] = vertices[1][1] = vertices[4][1] = vertices[5][1] = -height
      / 2.0f;
  vertices[2][1] = vertices[3][1] = vertices[6][1] = vertices[7][1] = height
      / 2.0f;
  vertices[0][2] = vertices[3][2] = vertices[4][2] = vertices[7][2] = -depth
      / 2.0f;
  vertices[1][2] = vertices[2][2] = vertices[5][2] = vertices[6][2] = depth
      / 2.0f;

  glNormal3fv(kNormals[face]);
  glVertex3fv(vertices[kFaces[face][0]]);
  glVertex3fv(vertices[kFaces[face][1]]);
  glVertex3fv(vertices[kFaces[face][2]]);
  glVertex3fv(vertices[kFaces[face][3]]);
}

void glotWireBox(GLfloat width, GLfloat height, GLfloat depth) {
  for (int i = 5; i >= 0; --i) {
    glBegin(GL_LINE_LOOP);
    glotBoxFace(width, height, depth, i);
    glEnd();
  }
}

void glotSolidBox(GLfloat width, GLfloat height, GLfloat depth) {
  for (int i = 5; i >= 0; --i) {
    glBegin(GL_QUADS);
    glotBoxFace(width, height, depth, i);
    glEnd();
  }
}

void glotSolidBoxFace(GLfloat width, GLfloat height, GLfloat depth,
                      GLuint face) {
  glBegin(GL_QUADS);
  glotBoxFace(width, height, depth, face);
  glEnd();
}
