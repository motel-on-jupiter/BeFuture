/**
 * Copyright (C) 2014 The Motel on Jupiter
 */
#ifndef CAMERA_H_
#define CAMERA_H_

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera {
 public:
  struct Perspective {
    float fovy;
    float aspect;
    float znear;
    float zfar;

    Perspective(float fovy, float aspect, float znear, float zfar)
        : fovy(fovy),
          aspect(aspect),
          znear(znear),
          zfar(zfar) {
    }
  };
  struct Viewport {
    glm::i32vec2 pos;
    glm::i32vec2 size;

    Viewport(glm::i32vec2 pos, glm::i32vec2 size)
        : pos(pos),
          size(size) {
    }
  };

  Camera(const glm::vec3& default_pos, const glm::vec3& default_at,
         const glm::vec3& default_up, const Perspective &default_pers,
         const Viewport &default_vport);
  virtual ~Camera();

  glm::mat4 CalcProjMtx() const {
    return glm::perspective(pers_.fovy, pers_.aspect, pers_.znear, pers_.zfar);
  }
  glm::mat4 CalcViewMtx() const {
    return glm::lookAt(pos_, at_, up_);
  }

  glm::vec3 pos() const {
    return pos_;
  }
  glm::vec3 at() const {
    return at_;
  }
  glm::vec3 up() const {
    return up_;
  }
  const Perspective &pers() const {
    return pers_;
  }
  const Viewport &vport() const {
    return vport_;
  }

  void set_pos(glm::vec3 pos) {
    pos_ = pos;
  }
  void set_at(glm::vec3 at) {
    at_ = at;
  }
  void set_up(glm::vec3 up) {
    up_ = up;
  }
  void set_pers(const Perspective &pers) {
    memcpy(&pers_, &pers, sizeof(Perspective));
  }
  void set_vport(const Viewport &vport) {
    memcpy(&vport_, &vport, sizeof(Perspective));
  }

  /* for Tweak Menu */
  float *pos_ptr() {
    return glm::value_ptr(pos_);
  }
  float *at_ptr() {
    return glm::value_ptr(at_);
  }
  float *up_ptr() {
    return glm::value_ptr(up_);
  }
  Perspective &mutable_pers() {
    return pers_;
  }
  Viewport &mutable_vport() {
    return vport_;
  }

 protected:
  glm::vec3 pos_;
  glm::vec3 at_;
  glm::vec3 up_;
  Camera::Perspective pers_;
  Viewport vport_;
};

#endif /* CAMERA_H_ */
