//
// Created by bob on 1/12/17.
//

#include "system/ApplicationMain.h"

#include "system/_/DispatchEventsLinux.h"

#include <memory>

void ApplicationMain::Run(std::vector<std::string> programArguments) {

  auto dispatchEvents = std::make_unique<DispatchEventsLinux>();

  dispatchEvents->Run();
}
