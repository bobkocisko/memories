//
// Created by bob on 4/24/17.
//

#pragma once

#include <memory>

namespace memories {

class Window;

class CreateWindow {
 public:
  virtual ~CreateWindow() {
  }

  virtual std::shared_ptr<Window> Create() = 0;
};

}