//
// Created by bob on 4/23/17.
//

#pragma once

namespace memories {

class EventSource;

class DispatchEvents {
 public:
  virtual ~DispatchEvents() {}

  virtual void RegisterEventSource(const EventSource *eventSource) = 0;

  virtual void UnregisterEventSource(const EventSource *eventSource) = 0;

  virtual void Run() = 0;
};

}