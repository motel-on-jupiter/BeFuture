/**
 * Copyright (C) 2014 The Motel On Jupiter
 */

#ifndef BEFUTUREGAME_H_
#define BEFUTUREGAME_H_

struct BeFutureContext;

class BeFutureGame {
 public:
  BeFutureGame();
  ~BeFutureGame();

  int Initialize(BeFutureContext &context);
  void Finalize(BeFutureContext &context);

  void Update(BeFutureContext &context, float elapsed_time);
  int Draw(BeFutureContext &context);

 private:
  bool initialized_;
  GLuint framebuf_;
  GLuint renderbuf_;
  GLuint shader_;
  GLuint colortexs_[2];
};

#endif /* BEFUTUREGAME_H_ */
