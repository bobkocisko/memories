//
// Created by bob on 4/23/17.
//

#include "system/_/DispatchEventsLinux.h"

#include "system/EventSourceLinux.h"

#include <cstring>
#include <sys/epoll.h>

DispatchEventsLinux::DispatchEventsLinux()
    : ignoreList_{},
      currentEventCount_(-1),
      currentEventIndex_(-1) {
  auto epfd = epoll_create1(EPOLL_CLOEXEC);

  if (-1 == epfd) {
    throw std::runtime_error(std::string("Could not call epoll_create1: ") + strerror(errno));
  }

  epollfd_ = epfd;
}

void DispatchEventsLinux::RegisterEventSource(const EventSource *eventSource) {
  auto esl = static_cast<const EventSourceLinux *>(eventSource);

  epoll_event details;
  details.events = EPOLLIN;
  details.data.ptr = (void *) esl;

  if (-1 == epoll_ctl(epollfd_, EPOLL_CTL_ADD, esl->GetFileDescriptor(), &details)) {
    throw std::runtime_error(std::string("Could not call epoll_ctl: ") + strerror(errno));
  }
}
void DispatchEventsLinux::UnregisterEventSource(const EventSource *eventSource) {
  auto esl = static_cast<const EventSourceLinux *>(eventSource);

  if (-1 == epoll_ctl(epollfd_, EPOLL_CTL_DEL, esl->GetFileDescriptor(), nullptr)) {
    throw std::runtime_error(std::string("Could not call epoll_ctl: ") + strerror(errno));
  }

  // We need to assume that this event source is about to be deleted and so we need to make
  // sure that we no longer refer to it or else we'll get a segfault.  It is likely that this
  // method is being called as a reentry under the Run() method since everything is single
  // threaded and all functionality in the app occurs as the result of an event of some sort.
  // So we look at the existing list of events left to be processed in Run()--that is, the
  // events list that is returned from epoll_wait, and if any of the not-yet-processed events
  // in the list match this pointer, then we signal to Run() that it should skip that one when
  // it continues processing.  For more details and motivation, see:
  // 'man epoll' => Possible pitfalls and ways to avoid them => If using an event cache...
  for (int i = currentEventIndex_ + 1; i < currentEventCount_; ++i) {
    if (eventsList_[i].data.ptr == esl) {
      ignoreList_[i] = true;
    }
  }
}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
void DispatchEventsLinux::Run() {
  while (true) {
    currentEventCount_ = epoll_wait(epollfd_, eventsList_, MaxEvents, -1);

    if (-1 == currentEventCount_) {
      throw std::runtime_error(std::string("Could not call epoll_wait: ") + strerror(errno));
    }

    for (currentEventIndex_ = 0; currentEventIndex_ < currentEventCount_; ++currentEventIndex_) {
      auto &event = eventsList_[currentEventIndex_];

      if (ignoreList_[currentEventIndex_]) {
        // This event has been unregistered since epoll_wait was called, so we need to ignore it
        // or else we are likely to get a segfault by trying to access a now-deleted object
        ignoreList_[currentEventIndex_] = false; // reset
      } else {
        int readyTypes = 0;
        if (event.events & EPOLLIN) {
          readyTypes &= EventSource::ReadyToRead;
        }
        if (event.events & EPOLLOUT) {
          readyTypes &= EventSource::ReadyToWrite;
        }
        static_cast<EventSource *>(event.data.ptr)->NotifyIsReady(EventSource::ReadyTypes(readyTypes));
      }
    }
  }
}
#pragma clang diagnostic pop
