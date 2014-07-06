/**
 * Copyright (C) 2014 The Motel On Jupiter
 */

#ifndef BEFUTURETWEAKMENU_H_
#define BEFUTURETWEAKMENU_H_

#include <boost/serialization/singleton.hpp>
#include <AntTweakBar.h>

#define BEFUTURE_TWEAK_MENU (BeFutureTweakMenu::get_mutable_instance())

struct BeFutureContext;

class BeFutureTweakMenu : public boost::serialization::singleton<
    BeFutureTweakMenu> {
 public:
  int Initialize(BeFutureContext &context);
  int Finalize();

  int UpdateWindowSize(int width, int height);
  int Render();

 protected:
  BeFutureTweakMenu()
      : twbar_(nullptr) {
  }
  virtual ~BeFutureTweakMenu() {
    Finalize();
  }

 private:
  TwBar *twbar_;
};

#endif /* BEFUTURETWEAKMENU_H_ */
