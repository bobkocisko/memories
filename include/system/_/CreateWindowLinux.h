//
// Created by bob on 4/24/17.
//

#pragma once

#include "../CreateWindow.h"

namespace memories {

class CreateWindowLinux : public CreateWindow {
 public:
  std::shared_ptr<Window> Create() override;
};

}