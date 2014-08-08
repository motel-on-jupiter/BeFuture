/**
 * Copyright (C) 2014 The Motel on Jupiter
 */
#ifndef DEBRISSTORM_H_
#define DEBRISSTORM_H_

#include "util/wrapper/spark_wrap.h"

struct SDL_Surface;
class BitmapTexture;

class CharacterStorm {
 public:
  CharacterStorm();
  virtual ~CharacterStorm();

  int Initialize(int num_chars, float char_lifetime, float emit_interval,
                 glm::vec2 char_size);
  int Finalize();

  void Update(float elapsed_time);
  void Render();

  int GetNumCharacters() const;
  void SetMaxCharacters(int max);
  int GetMaxCharacters() const;
  int ClearCharacters();
  int SetEmissionInterval(float interval);
  int InactivateEmission(bool inactive);
  bool IsEmissionActive() const;
  void SetCharacterSize(glm::vec2 size);
  glm::vec2 GetCharacterSize() const;
  void SetCharacterLifetime(float time);
  float GetCharacterLifetime() const;

 private:
  enum {
    kNumberOfTexture = 10
  };

  glm::vec2 size_;
  SDL_Surface *texture_;
  GLuint texname_;
  SPK::Ref<SPK::System> system_;
  SPK::Ref<SPK::GL::GLQuadRenderer> renderer_;
  SPK::Ref<SPK::Group> group_;
  SPK::Ref<SPK::Emitter> emitter_;
};

#endif /* DEBRISSTORM_H_ */
