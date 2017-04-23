//
// Created by bob on 4/23/17.
//

#pragma once

#include "DispatchEvents.h"

#include <sys/epoll.h>

class DispatchEventsLinux : public DispatchEvents {
 public:
  DispatchEventsLinux();

  void RegisterEventSource(const EventSource *eventSource) override;
  void UnregisterEventSource(const EventSource *eventSource) override;

  void Run() override;

 private:
  constexpr static int MaxEvents = 25;

 private:
  int epollfd_;

  epoll_event eventsList_[MaxEvents];
  bool ignoreList_[MaxEvents];

  int currentEventCount_;
  int currentEventIndex_;
};
