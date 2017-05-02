//
// Created by bob on 4/24/17.
//

#include "system/_/EventSourceLinux.h"

#include "system/_/DispatchEventsLinux.h"

namespace memories {

EventSourceLinux::EventSourceLinux(std::shared_ptr<DispatchEventsLinux> dispatchEventsLinux)
    : dispatchEventsLinux_(dispatchEventsLinux) {}

void EventSourceLinux::Unregister() const {
  dispatchEventsLinux_->UnregisterEventSource(this);
}

}