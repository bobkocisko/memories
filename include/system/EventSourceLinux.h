//
// Created by bob on 4/23/17.
//

#pragma once

#include "EventSource.h"

class DispatchEventsLinux;

class EventSourceLinux : public EventSource {
 public:
  EventSourceLinux(std::shared_ptr<DispatchEventsLinux> dispatchEventsLinux);

  virtual int GetFileDescriptor() const = 0;

 protected:
  /**
   * Must be called by derived classes' destructors before they close the file descriptor
   */
  void Unregister() const;

 private:
  std::shared_ptr<DispatchEventsLinux> dispatchEventsLinux_;
};
