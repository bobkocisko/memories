//
// Created by bob on 1/12/17.
//

#include "system/ApplicationMain.h"

#include "system/_/DispatchEventsLinux.h"
#include "system/_/CreateWindowLinux.h"

#include <memory>

namespace memories {

void ApplicationMain::Run(std::vector<std::string> programArguments) {

  CreateWindowLinux createWindowLinux;
  createWindowLinux.Create();

  auto dispatchEvents = std::make_unique<DispatchEventsLinux>();

  dispatchEvents->Run();
}

}